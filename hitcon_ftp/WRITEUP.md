# HITCON FTP Writeup

## 1. Challenge Overview

Bài cho một server kiểu TFTP chạy bằng UDP, nhưng packet format không phải TFTP chuẩn mà là MessagePack. TCP cổng `10309` chỉ dùng để trả banner chứa UDP port mà worker hiện tại đang lắng nghe. Muốn nói chuyện đúng với server cần:

1. kết nối TCP tới `chall.pwnable.tw:10309`
2. parse UDP port từ banner
3. bind UDP socket vào đúng local port của TCP socket
4. gửi packet MessagePack tới UDP port đó

Protocol chính:

- `RRQ`: `[1, "filename", "mode", {opts}]`
- `WRQ`: `[2, "filename", "mode", {opts}]`
- `DATA`: `[3, blockno, crc32, <str|bin>]`
- `ACK`: `[4, blockno]`
- `ERROR`: `[5, code, "msg"]`
- `OACK`: `[6, {opt: value}]`

Mục tiêu là đọc flag tại:

```text
/home/hitcon_ftp/flag
```

Nhưng đường file trực tiếp không dùng được:

- path tuyệt đối bị chặn nếu ký tự đầu là `'/'`
- path traversal `../` bị chặn
- `WRQ` còn bị redirect sang `"/dev/null"`

Do đó hướng giải phải là memory corruption, không phải path trick.

## 2. Initial Recon

Đọc lại binary cho thấy có hai điểm đáng chú ý:

1. parser option map trong `process_new()` có type confusion
2. `check_crc32()` có stack overflow trong frame của `main`

Hai bug này riêng lẻ chưa đủ để lấy flag, nhưng ghép lại thành một chain hoàn chỉnh:

- type confusion để xây arbitrary read mạnh
- leak đầy đủ `heap`, `libc`, `PIE`, `stack`, `canary`
- overflow để sculpt stack frame của `main`
- chờ `alarm -> select failed -> return`
- delayed ROP để `open/read/send_data`

## 3. Bug 1: Type Confusion in Option Parsing

### 3.1 Root Cause

Trong `main`, packet `RRQ/WRQ` chỉ check:

- phần tử 0 là integer
- phần tử 1 là string
- phần tử 2 là string
- nếu outer array có đúng `size == 4` thì phần tử thứ 4 phải là `map`

Nhưng `process_new()` lại luôn dùng phần tử thứ 4 như `msgpack map` miễn là `size > 3`, cụ thể đọc:

- `count = *(ptr_pack + 0x50)`
- `data  = *(ptr_pack + 0x58)`

Nói cách khác, với packet có `size >= 5`, ta có thể đưa object thứ 4 là một type bất kỳ, nhưng code vẫn cưỡng ép diễn giải layout của nó thành `msgpack_map_object`.

### 3.2 Why This Matters

Trong nhánh unknown option, code làm:

```c
strncpy(dest, key.ptr, key.size);
add_oack_opt(dest, value);
```

Sau đó `send_oack()` reflect lại option name trong response.

Nếu ta forge được một fake `msgpack_map_kv`, ta điều khiển được:

- `key.type = str`
- `key.size = số byte muốn đọc`
- `key.ptr = địa chỉ muốn leak`
- `val.type = int`
- `val.value = giá trị bất kỳ`

Kết quả là OACK trả lại dữ liệu ở địa chỉ tùy ý như một key string.

### 3.3 Building an Arbitrary Read

Payload thực tế dùng `msgpack.ExtType` để chứa một fake `msgpack_map_kv` có layout:

- key object ở `+0x00`
- value object ở `+0x18`

Với cách này ta có arbitrary read qua OACK reflection đủ ổn định để leak các pointer quan trọng.

## 4. Leak Chain

Sau khi có primitive đọc, chain leak ổn định là:

1. leak 5 byte heap qua OACK reflection
2. dựng:

```text
request = heap_base + 0x13b10
```

3. leak `request + 0x110` để lấy `FILE *`
4. leak `FILE + 0xd8` để lấy `_IO_file_jumps`
5. suy ra:

```text
libc_base = _IO_file_jumps - 0x3e82a0
```

6. leak `libc + __environ`
7. đi từ `environ[]` tới `auxv`
8. leak `AT_RANDOM`
9. lấy canary:

```text
canary = u64(AT_RANDOM[0:8]) & ~0xff
```

10. leak `AT_PHDR` để suy ra `PIE`

Điểm quan trọng là ta không còn cần đoán canary từ stack mơ hồ; canary được leak đúng từ `AT_RANDOM`, nên rất ổn định.

## 5. Bug 2: Stack Overflow in `check_crc32()`

### 5.1 Root Cause

Packet `DATA` đi vào `check_crc32()`. Hàm này stringify object cuối cùng vào một buffer local nằm trong frame stack của `main`.

Buffer bắt đầu tại:

```text
rbp - 0x224
```

Các offset quan trọng:

- canary tại output offset `0x20c`
- saved `rbp` tại `0x224`
- saved `rip` tại `0x22c`

Vấn đề là object được in ra không phải raw bytes, mà qua `msgpack_object_print_buffer()`.

### 5.2 Why It Is Still Exploitable

Lúc đầu bug này nhìn khá khó khai thác vì:

- `DATA` chỉ cho object cuối là `str` hoặc `bin`
- output bị stringify
- phải giữ nguyên canary trước khi chạm RIP

Tuy nhiên control-flow của `main` tạo một điểm rất thuận lợi:

- `init()` đặt `signal(SIGALRM, handler)`
- `init()` đặt `alarm(0x3c)`

Sau 60 giây, `SIGALRM` làm `select()` fail. `main` đi vào nhánh `select failed`, rồi chạy epilogue và `ret`.

Nghĩa là nếu có thể sculpt stack frame trước đó, ta không cần hijack execution ngay tại lúc overflow. Ta chỉ cần chuẩn bị sẵn stack, rồi chờ `main` tự return.

Đây là delayed control-flow hijack.

## 6. Turning the Overflow into a Stack Sculpt Primitive

### 6.1 The Core Difficulty

`msgpack_object_print_buffer(str)` không ghi raw bytes kiểu memcpy. Nó ghi theo pattern:

```text
" + data[:-1] + NUL
```

Do đó muốn chạm RIP thì sẽ phá cả canary nằm trước đó. Muốn qua canary, byte thấp của canary phải trở lại `0x00`.

Một lần overflow không đủ để vừa overwrite RIP vừa giữ canary đúng. Nhưng nhiều lần overflow trên cùng stack frame thì đủ.

### 6.2 Multi-stage Sculpting

Ý tưởng là:

1. stage dài nhất overwrite toàn bộ vùng tới saved RIP và fake stack
2. các stage ngắn hơn lần lượt restore những byte cần là `0x00`
3. stage cuối restore byte thấp canary về `0x00`

Vì tất cả stage đều viết từ đầu buffer, mỗi stage ngắn hơn sẽ chỉ sửa phần đầu và để nguyên phần cuối đã sculpt từ stage dài hơn.

### 6.3 The Off-by-one Pitfall

Phần khó nhất của exploit thực tế là mapping giữa:

- độ dài raw string gửi đi
- `a8` mà printer thấy
- offset output thật trên stack

Sau khi debug local bằng GDB và so với dump stack thật, mapping đúng là:

- muốn control output offset `k` cần stage length `k + 3`
- muốn đặt lại zero ở output offset `k` cần stage length `k + 2`

Đây là điểm quyết định khiến stack sculpt trở nên ổn định. Trước khi sửa mapping này, saved RIP và canary đều bị lệch một byte và bị nhiễm `0x41`.

## 7. Recovering the Exact Stack Frame

Leaking canary là chưa đủ; còn phải biết chính xác `rbp` của frame `main`.

Thay vì đoán `rbp = envp - 0x100`, cách đúng là:

1. leak `envp`
2. scan vùng stack quanh `envp`
3. tìm slot chứa 7 byte cao của canary đã leak
4. suy ra:

```text
canary_slot = [rbp - 0x18]
rbp = canary_slot + 0x18
```

Cách này làm cho sculpting không phụ thuộc vào assumption local.

## 8. Confirming RIP Control

Trước khi chạy ORW, cần xác nhận delayed ROP thật sự hoạt động.

Marker chain dùng:

```text
send_data(request, "MARK", 4)
```

Sau khi chờ alarm, remote trả về packet:

```text
[3, 1, ..., b'MARK']
```

Điều này xác nhận toàn bộ chain sau:

1. stack sculpt thành công
2. canary hợp lệ
3. `main` đi tới epilogue sau `select failed`
4. `ret` qua RIP đã bị overwrite
5. `pop rsp ; ret` pivot vào fake stack
6. gọi được hàm trong ROP chain

Từ đây bài toán không còn là “có RIP control không”, mà chỉ còn là viết đúng payload ORW.

## 9. Final ROP Chain

### 9.1 Why `send_data()` Is Better Than `connect()+write()`

Thay vì gọi `connect()` rồi `write()` thủ công, exploit cuối dùng luôn helper của binary:

```text
send_data(request, buf, len)
```

Ưu điểm:

- không cần suy đoán socket fd runtime
- không cần tự chuẩn bị `sockaddr`
- binary tự gửi packet `DATA` về đúng peer đã lưu trong `request`

Đây là cách tự nhiên nhất để đẩy flag ra ngoài.

### 9.2 Final Chain

Chain cuối:

1. `open("/home/hitcon_ftp/flag", 0)`
2. `read(3, buf, 0x80)`
3. `send_data(request, buf, 0x80)`
4. `exit(0)`

Gadget/hàm dùng:

- `pop rdi ; ret = libc + 0x2155f`
- `pop rsi ; ret = libc + 0x23e6a`
- `pop rdx ; pop r10 ; ret = libc + 0x1306b4`
- `pop rsp ; ret = libc + 0x23993`
- `open = libc + 0x10fc40`
- `read = libc + 0x110070`
- `exit = libc + 0x43120`
- `send_data = pie + 0x257e`

Lưu ý: trong quá trình debug, hai gadget từng dùng trước đó bị phát hiện sai:

- `pop rsp ; ret`
- `pop rdx ; ret`

Việc sửa lại gadget thật trong `.text` là bước rất quan trọng để exploit hoạt động.

## 10. Final Result

Khi chạy exploit cuối, remote trả về:

```text
[3, 1, 2624524445, b'FLAG{c4ptur3_th3_f0rtun3_by_h1tc0n_ftp}\n...']
```

Flag:

```text
FLAG{c4ptur3_th3_f0rtun3_by_h1tc0n_ftp}
```

## 11. Files

Các file chính trong workspace:

- [exploit_final.py](/home/ngobao259/pwnabletw/hitcon_ftp/exploit_final.py): bản exploit cuối chỉ giữ chain lấy flag
- [remote_rop.py](/home/ngobao259/pwnabletw/hitcon_ftp/remote_rop.py): script exploit/debug đầy đủ dùng trong quá trình tuning
- [WRITEUP.md](/home/ngobao259/pwnabletw/hitcon_ftp/WRITEUP.md): tài liệu này

## 12. Closing Notes

Đây là một bài điển hình của kiểu exploit “ghép nhiều primitive yếu thành một chain mạnh”:

- type confusion chỉ cho arbitrary read
- overflow ban đầu nhìn không ROP được vì canary và stringify
- nhưng delayed return qua `alarm` biến nó thành một stack sculpt primitive rất mạnh

Điểm đáng giá nhất của bài là không nằm ở gadget nào đặc biệt, mà ở việc hiểu chính xác semantics của string printer, stack layout của `main`, và cách dùng nhiều stage để phục hồi canary rồi mới hijack control flow.
