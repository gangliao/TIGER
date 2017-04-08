.data
B: .word 0
n1: .word 0
t0: .word 0

printi_arg0: .word 0
A: .word 0

printf_arg0: .word 0
.text
main:
j main0

li $v0, 10
syscall
printi:
li $v0, 1
lw $a0, printi_arg0
syscall
jr $ra
printf:
lwc1 $f0, 4($sp)
lw $s0, B
lw $s1, n1
lw $s2, t0
li.s $f1, 5.0
mfc1 $t1 $f1
add.s $f2, $f0, $f1
mfc1 $s2, $f2
move $s0, $s2
sw $s0, printi_arg0
lw $a0, printi_arg0
sw $s0, B
sw $s1, n1
sw $s2, t0
addi $sp, $sp, -8
sw $a0, 4($sp)
sw $ra, 0($sp)
jal printi
lw $a0, 4($sp)
lw $ra, 0($sp)
addi $sp, $sp, 8
jr $ra
main0:
lw $s3, A
li.s $f2, 1.5
mfc1 $t0 $f2
move $s3, $t0
sw $s3, printf_arg0
lw $a0, printf_arg0
sw $s3, A
addi $sp, $sp, -8
sw $a0, 4($sp)
sw $ra, 0($sp)
jal printf
lw $a0, 4($sp)
lw $ra, 0($sp)
addi $sp, $sp, 8
jr $ra
