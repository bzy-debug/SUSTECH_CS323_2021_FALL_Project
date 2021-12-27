# SPL compiler generated assembly
.data
_prmpt: .asciiz "Enter an integer: "
_eol: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prmpt
  syscall
  li $v0, 5
  syscall
  jr $ra
write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _eol
  syscall
  move $v0, $0
  jr $ra
main:
  li $t0, 110
  li $t1, 97
  li $t2, 3
  sub $t3, $t0, $t1
  li $t4, 2
  mul $t5, $t2, $t4
  add $t6, $t3, $t5
  move $t7, $t6
  move $s0, $t7
  move $a0, $s0
  addi $sp, $sp, -4
  sw $ra, 0($sp)
  jal write
  lw $ra, 0($sp)
  addi $sp, $sp, 4
  li $s1, 0
  move $v0, $s1
  jr $ra
