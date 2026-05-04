# hitcon_ftp Notes

## Tổng quan

- Challenge là một TFTP-like server dùng UDP nhưng packet format là MessagePack.
- TCP cổng `10309` chỉ dùng để trả banner chứa `udp port: <port>`.
- Để nói chuyện đúng với server:
  - mở TCP tới `chall.pwnable.tw:10309`
  - parse UDP port từ banner
  - bind UDP socket vào cùng local port với TCP socket đó
  - gửi packet MessagePack tới UDP port vừa nhận

## Protocol

- `RRQ`: `[1, "filename", "mode", {opts}]`
- `WRQ`: `[2, "filename", "mode", {opts}]`
- `DATA`: `[3, blockno, crc32, <str|bin>]`
- `ACK`: `[4, blockno]`
- `ERROR`: `[5, code, "msg"]`
- `OACK`: `[6, {opt: value}]`

Các file chính đã đọc:

- [decompiled/87_00003DC1.c](/home/ngobao259/pwnabletw/hitcon_ftp/decompiled/87_00003DC1.c)
- [decompiled/83_00002FBB.c](/home/ngobao259/pwnabletw/hitcon_ftp/decompiled/83_00002FBB.c)
- [decompiled/84_00003B8E.c](/home/ngobao259/pwnabletw/hitcon_ftp/decompiled/84_00003B8E.c)
- [decompiled/80_00002BAC.c](/home/ngobao259/pwnabletw/hitcon_ftp/decompiled/80_00002BAC.c)
- [decompiled/85_00003C41.c](/home/ngobao259/pwnabletw/hitcon_ftp/decompiled/85_00003C41.c)
- [decompiled/86_00003D44.c](/home/ngobao259/pwnabletw/hitcon_ftp/decompiled/86_00003D44.c)
- [decompiled/110_0000660B.c](/home/ngobao259/pwnabletw/hitcon_ftp/decompiled/110_0000660B.c)

## Hành vi file

- `RRQ` mở file thật bằng `fopen(path, "rb"/"rt")`.
- `WRQ` bị vô hiệu hóa: path bị đổi thành `"/dev/null"` trước khi `fopen`.
- Path tuyệt đối bị chặn nếu ký tự đầu là `'/'`.
- Path traversal bị chặn bởi `strnstr(path, "../", 0x100)`.

Remote xác nhận:

- `python3 solve.py REMOTE` trả:

```text
here is /tmp. flag is under /home/hitcon_ftp/flag.
```

- Thử các path tương đối như `home/hitcon_ftp/flag`, `./home/hitcon_ftp/flag`, `proc/self/maps` đều trả `invalid path or no access`.

## Type confusion trong option map

Trong `main`, packet `RRQ/WRQ` chỉ check:

- element 0 là int
- element 1 là string
- element 2 là string
- nếu `size == 4` thì element 3 phải là `map`

Nhưng `process_new()` luôn lấy:

- `a3[10]` làm `count`
- `a3[11]` làm `ptr`

và xử lý element thứ 4 như `map`, kể cả khi outer array có `size >= 5`. Đây là type confusion.

### Layout `msgpack_object`

`msgpack_object` dài `0x18`:

- `+0x00`: `type`
- `+0x08`: `size/count/value`
- `+0x10`: `ptr`

Kiểu quan trọng:

- `str`: type `5`, `size @ +8`, `ptr @ +16`
- `array`: type `6`, `count @ +8`, `elems @ +16`
- `map`: type `7`, `count @ +8`, `kvs @ +16`
- `bin`: type `8`, `size @ +8`, `ptr @ +16`
- `ext`: type `9`, low byte của `+8` là ext type, high dword là size

`msgpack_map_kv` là hai object liên tiếp, dài `0x30`:

- key object tại `+0x00`
- value object tại `+0x18`

### Primitive đọc

Nhánh unknown option trong `process_new()`:

- `strncpy(dest, key.ptr, key.size)`
- `add_oack_opt(dest, value)`
- `send_oack()` reflect lại option name

Do đó nếu forge được fake `map_kv`, ta có primitive đọc bộ nhớ qua OACK option name.

## Xác nhận remote primitive

### Reflection cơ bản

Packet:

```python
[1, "flag", "octet", ["AAAA", 123], 0]
```

Remote trả:

```python
[6, {'AAAA': 123, 'tsize': 51}]
```

### Arbitrary read bằng `ExtType`

Forge fake `map_kv` bên trong `msgpack.ExtType`.

Đọc `vsyscall` page tại `0xffffffffff600000` đã xác nhận primitive đọc theo địa chỉ là thật. Một response raw từng thấy:

```text
920682a448c7c060cd5151a57473697a6533
```

`48 c7 c0 60` là bytes code, không phải string bình thường.

## Heap leak remote

### Kỹ thuật

1. Prime bằng WRQ với `map` có nhiều key 5 byte:

```python
[2, "x", "octet", {"k0000":0, ..., "k0019":19}, 0]
```

2. Exploit bằng WRQ với `array`:

```python
[2, "x", "octet", ["pad", 1], 0]
```

3. OACK phản xạ thêm một key rác 5 byte là heap leak.

### Một số leak đã thấy

- `e2f96c4056`
- `52145fe155`
- `a2e63eb455`
- `92624db055`
- `628d266b55`
- `e2a69e0b56`
- `82fb2c1c56`
- `4214755a55`

Reconstruct:

```python
heap_base = int.from_bytes(leak5, "little") << 8
```

## Libc leak remote

Chain đã xác nhận:

1. `heap_base` từ leak 5 byte
2. `request_guess = heap_base + 0x13b10`
3. đọc `request_guess + 0x110` ra `heap2`
4. đọc `heap2 + 0xd8` ra `_IO_file_jumps`
5. `libc_base = libc_ptr - 0x3e82a0`

Ví dụ một run:

- `heap_base = 0x564ee430c200`
- `heap2 = 0x564ee431ead0`
- `libc_ptr(_IO_file_jumps) = 0x7ff549ba92a0`
- `libc_base = 0x7ff5497c1000`

Ví dụ run khác:

- `heap_base = 0x55d4827a3200`
- `request = 0x55d4827b6d10`
- `heap2 = 0x55d4827b5ad0`
- `libc_base = 0x7fec4865a000`

## Leak stack / environ / PIE

Từ libc:

- `__environ` offset là `0x3ee098`

Remote đã leak được:

- `envp = 0x7ffef228cd90`
- `envp = 0x7ffe9dd13e10`
- `envp = 0x7ffca90fff50`

Từ stack auxv đọc được `AT_PHDR`, từ đó suy ra:

- `pie_base = 0x56241cb1c000`

## Symbols / offsets quan trọng

### Binary

- `sockfd = pie + 0x20d010`
- `init_reqs = pie + 0x20d060`
- `oack_opts = pie + 0x20d160`
- `datain = pie + 0x20d370`
- `opt_cnt = pie + 0x20d378`

### Libc 2.27

- `_IO_file_jumps = 0x3e82a0`
- `__environ = 0x3ee098`
- `system = 0x4f440`
- `exit = 0x43120`
- `open = 0x10fc40`
- `read = 0x110070`
- `write = 0x110140`
- `dup2 = 0x1109a0`
- `connect = 0x1228a0`
- `sendto = 0x122d90`

Gadget offsets:

- `pop rdi ; ret = 0x2155f`
- `pop rsi ; ret = 0x23e6a`
- `pop rdx ; ret = 0x1b96`
- `pop rsp ; ret = 0x3960`
- `ret = 0x8aa`

## Request / heap layout thật

Đã leak trực tiếp `init_reqs[]` remote:

- `init_reqs[0] = 0x56241d50fd10`
- `init_reqs[1] = 0x56241d5112a0`
- delta `= 0x1590`

Với `init_reqs[0]`:

- `FILE*` tại `request0 + 0x110 = 0x56241d50ead0`
- tức `FILE*` nằm trước request0 khoảng `0x1240`

Với `init_reqs[1]`:

- đã có một run đọc ra `file=0x60`, chứng tỏ primitive đọc pointer ở request1 chưa ổn định mọi lần

Nhưng điểm chắc chắn:

- request chunk dài `0x340`
- request1 bắt đầu ở `request0 + 0x1590`

## Lỗi `check_crc32()`

`main` gọi:

```c
check_crc32((const char *)&v34, ..., v21[9], v21[10], v21[11])
```

Trong `check_crc32()`:

```c
msgpack_object_print_buffer(a1, a8 + 1, a8 + 1, ..., a7, a8, a9);
```

Nên đây là stack overflow thực sự, vì `&v34` chỉ là buffer stack nhỏ nhưng `a8+1` có thể lớn tới `blksize+1`.

### Offset stack trong `main`

Từ disassembly:

- canary ở `[rbp-0x18]`
- `&v34` ở `[rbp-0x224]`
- saved RIP ở `[rbp+0x8]`

Suy ra:

- tới canary: `0x20c`
- tới saved rbp: `0x224`
- tới saved rip: `0x22c`

### Tại sao chưa dùng được cho ROP

`msgpack_object_print_buffer()` chỉ ghi dữ liệu đã stringify:

- số được in thành ASCII
- string được bọc dấu `"`
- bin được escape
- ext được format thành `(ext: n)"..."`

Nó không cho ghi raw bytes tùy ý vào stack.

Do canary có byte thấp `0x00`, hiện chưa có cách:

- giữ nguyên canary hợp lệ
- rồi tiếp tục ghi saved RIP thành ROP chain

Vì vậy hướng `check_crc32() -> ret2libc -> ORW` chưa đi hết được.

## Sai số cũ về canary

Từ local GDB với binary tương thích:

- `*__environ` trên stack
- `rbp` của `main`
- `canary_addr = __environ - 0x110`

Remote scan byte quanh `envp - 0x110` cho thấy vùng đó không khớp canary thật:

```text
byte[-0x8] .. byte[-0x1]  = 00 e3 be e5 02 40 e7 f5
byte[+0x0] .. byte[+0x8]  = 00 00 00 00 00 00 00 00 00
byte[+0x18].. byte[+0x1f] = 97 eb 3f cb 2c 7f 00 00
```

Tức offset stack trên remote chưa được khóa hoàn toàn, nhưng ngay cả khi khóa được thì vấn đề stringify của overflow vẫn còn.

## Leak canary đúng qua `AT_RANDOM`

Không cần đoán offset `envp - 0x110`.

Chuỗi ổn định hơn:

1. leak `libc_base`
2. leak `__environ`
3. đọc mảng `envp[]` trên stack tới phần tử `NULL`
4. ngay sau `NULL` là `auxv` dưới dạng cặp `(type, value)`
5. tìm `AT_RANDOM` (`type = 25`)
6. đọc 16 byte tại địa chỉ `AT_RANDOM`
7. `stack_canary = u64(random[0:8]) & ~0xff`

Remote đã xác nhận bằng `exploit.py`:

```text
envp      = 0x7ffded7e5820
envc      = 4
auxv      = 0x7ffded7e5848
AT_PHDR   = 0x560d981b7040
AT_RANDOM = 0x7ffded7e5979
random    = 7b1c92a2274e48d685493524939827b5
canary    = 0xd6484e27a2921c00
```

Điểm quan trọng:

- `AT_RANDOM` nằm trong `auxv`, không phải đoán từ khoảng cách giữa `__environ` và frame của `main`
- canary có thể leak trực tiếp từ random blob của loader/glibc
- điều này giải quyết bài toán leak canary, nhưng chưa giải quyết bài toán ghi ROP chain do overflow vẫn bị stringify

## Nhánh `ERROR` packet

Với request đã tồn tại, opcode `5` copy message vào:

- `request + 0x140`

Code:

```c
memset(request + 0x140, 0, 0x200);
strncpy(request + 0x140, msg_ptr, msg_len);
```

Tưởng đây là heap overflow tốt, nhưng packet bị chặn:

- với request đã tồn tại, mọi packet trừ `DATA` đều phải có `v31 <= 512`

Nên message thực tế chỉ đủ chạm tới khoảng:

- `request + 0x33a`

Trong khi chunk kế tiếp bắt đầu tại:

- `request + 0x350`

Còn thiếu `0x16` byte, nên không tràn sang chunk kế tiếp.

Kết luận:

- chưa thể dùng `ERROR` để đè `FILE` / request lân cận.

## Trạng thái hiện tại

Đã làm được:

- read primitive từ type confusion
- leak heap
- leak libc
- leak stack / envp
- leak PIE
- leak `init_reqs[]`
- xác nhận layout heap thật giữa 2 request sống

Chưa làm được:

- leak canary usable cho ROP
- ret2libc/ORW từ `check_crc32()`
- heap overflow sang `FILE` / request kế tiếp bằng `ERROR`
- lấy flag thật tại `/home/hitcon_ftp/flag`

## Scripts trong workspace

- [solve.py](/home/ngobao259/pwnabletw/hitcon_ftp/solve.py): RRQ client tối giản, đọc được file mồi `flag`
- [remote_probe.py](/home/ngobao259/pwnabletw/hitcon_ftp/remote_probe.py): probe primitive cơ bản
- [exploit.py](/home/ngobao259/pwnabletw/hitcon_ftp/exploit.py): leak heap/libc/envp và scan stack bytes
- [layout_probe.py](/home/ngobao259/pwnabletw/hitcon_ftp/layout_probe.py): leak `PIE` và `init_reqs[]`

## Hướng hợp lý nếu tiếp tục

- tìm primitive ghi heap thật từ overlap parser zone / request / `FILE`
- hoặc lifecycle bug quanh `fclose` / `fopen` / `setvbuf`
- hoặc cách làm request chunk kế tiếp nằm gần hơn `0x350`

Không nên tiếp tục giả định:

- `check_crc32()` tự nó đủ để ORW
- `ERROR` packet đủ dài để chạm chunk kế tiếp
