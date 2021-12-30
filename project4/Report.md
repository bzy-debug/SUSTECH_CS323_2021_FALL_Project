# Report

### Overview

In this project, we use the provided framework to translate the three address code to MIPS assembly code. The most import parts of this project is instruction selection, register allocation, and stack managing. The following sections are the detail information about these three parts.

### Instruction Selection

We use the table in document with a little modifications.

| three-address-code | MIPS32 instruction                                                                                              |
|:------------------:| --------------------------------------------------------------------------------------------------------------- |
| `LABEL x:`         | `label x:`                                                                                                      |
| `x := k`           | `li reg_w(x), k`                                                                                                |
| `x := y`           | `move reg_w(x), reg(y)`                                                                                         |
| `x := y + #k`      | `addi reg_w(x), reg(y), k`                                                                                      |
| `x := y + z`       | `add reg_w(x), reg(y), reg(z)`                                                                                  |
| `x := y - #k`      | `addi reg_w(x), reg(y), -k`                                                                                     |
| `x := y - z`       | `sub reg_w(x), reg(y), reg(z)`                                                                                  |
| `x := y * z`       | `mul reg_w(x), reg(y), reg(z)`                                                                                  |
| `x := y / z`       | `mdiv reg(y), reg(z)`<br/>`mflo reg_w(x)`                                                                       |
| `x := *y`          | `lw reg_w(x), 0(reg(y))`                                                                                        |
| `*x := y`          | `sw reg(y), 0(reg(x))`                                                                                          |
| `GOTO x`           | `j x`                                                                                                           |
| `x := call f`      | `store arg registers, $fp, and $ra`<br/>`jal f`<br/>`load arg registers, $fp, and $ra`<br/>`move reg_w(x), $v0` |
| `arg x`            | `do nothing`                                                                                                    |
| `RETURN x`         | `move $v0, reg(x)`<br/>`move $sp, $fp`<br/>`jr $ra`                                                             |
| `FUNCTION f:`      | `move $fp, $sp`                                                                                                 |
| `PARAM x`          | `move $a0 reg(x)`  (0 can be other integers)                                                                    |
| `IF x == y GOTO z` | `beq reg(x), reg(y), z`                                                                                         |
| `IF x != y GOTO z` | `bne reg(x), reg(y), z`                                                                                         |
| `IF x > y GOTO z`  | `bgt reg(x), reg(y), z`                                                                                         |
| `IF x < y GOTO z`  | `blt reg(x), reg(y), z`                                                                                         |
| `IF x >= y GOTO z` | `bge reg(x). reg(y), z`                                                                                         |
| `IF x <= y GOTO x` | `ble reg(x), reg(y), z`                                                                                         |

`reg(x)` and `reg_w(x)` are two functions for register allocation. `reg(x)`  returns a register containing variable x, and `reg_w(x)` returns an empty register for writing.

### Register Allocation

We use the simplest algorithm for register allocation, which is storing all variables in memory and load them into register when needed. The algorithm is implemented by three functions `get_register()`, `get_register_w()`, and `spill_register()`. To deal with the memory position of each variable, we maintain an register `$fp` pointing to the bottom of current stack frame, which is the beginning of local variables.

```
get_register(x):
    r <= an empty register
    offset <= the memory offset of variable x
    emit MIPS32 code: lw r, -offset($fp)
    mark r as containing variable x
    return r
```

```
get_register_w(x):
    r <= an empty register
    mark r as containing variable x
```

```
spill_register(r):
    if r contains a variable already in memory:
        offset <= the memory offset of containing variable
        emit MIPS32 code: sw r, -offset($fp)
    else r contains a variable not in memory:
        offset <= biggest offset + 4
        emit MIPS32 code: sw r, -offset($fp)
        emit MIPS32 code: addi $sp, $fp, -offset
    mark r as empty
```

To make sure that `$fp` always points to the bottom of current stack and `$sp` always points to the top of current stack, we do the following:

- Every time enter a function, make `$fp` equal to `$sp` .

- Every time leave a function, make  `$sp` equal to `$fp`.

- Every time store a new variable into memory, grow the biggest offset and `$sp`.

### Stack Managing

In the section above, we introduce the way to maintain `$fp` and `$sp`. With those efforts, calling a function becomes easy:

1. grow `$sp`

2. store current arg registers, `$fp`, and `$ra` into memory

3. move arguments into `$a+` registers

4. `jal f`

5. load arg registers, `$fp`, and `$ra`

6. decrease `$sp`

Since all variables are stored in memory, we do not need to consider caller's or callee's reservation registers.


