.data
A: .space 20

i: .word 0
n1: .word 0
t0: .word 0

B: .word 0


t1: .word 0
printf_arg0: .word 0

.text
main:
li.s $f3, 2.5
la $t0, A
swc1 $f3, 4($t0)
swc1 $f3, 8($t0)
swc1 $f3, 12($t0)
swc1 $f3, 16($t0)
li $t0, 0
move $t1, $t0
sw $t1, i
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
lw $s0, n1
lw $s1, t0
li.s $f1, 5.0
mfc1 $t1 $f1
add.s $f2, $f0, $f1
mfc1 $s1, $f2
move $t1, $s1
sw $t1, B
sw $s0, n1
sw $s1, t0
jr $ra
main0:
li $t1, 4
li.s $f2, 1.5
mfc1 $t0 $f2
li $t8, 4
mult $t8, $t1
mflo $t8
sw $t0, A($t8)
li $t0, 0
move $t1, $t0
sw $t1, i
loop_label0:
lw $s2, i
bgt $s2, 5, loop_label1
sw $s2, i
lw $s3, i
lw $s4, t1
li $t8, 4
mult $t8, $s3
mflo $t8
lw $s4, A($t8)
sw $s4, printf_arg0
lw $a0, printf_arg0
li $t1, 1
add $s3, $s3, $t1
sw $s3, i
sw $s4, t1
addi $sp, $sp, -8
sw $a0, 4($sp)
sw $ra, 0($sp)
jal printf
lw $a0, 4($sp)
lw $ra, 0($sp)
addi $sp, $sp, 8
j loop_label0
loop_label1:
jr $ra
