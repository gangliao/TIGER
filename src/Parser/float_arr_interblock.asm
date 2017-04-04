.data
i: .word 0
A: .space 20

t0: .word 0
B: .word 0
n1: .word 0



t1: .word 0

.text
main:
lw $s0, i
li.s $f3, 2.5
la $t0, A
swc1 $f3, 4($t0)
swc1 $f3, 8($t0)
swc1 $f3, 12($t0)
swc1 $f3, 16($t0)
li $t0, 0
move $s0, $t0
sw $s0, i
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
lw $s1, t0
lw $s2, B
lw $t0, n1
li.s $f1, 5.0
mfc1 $t1 $f1
add.s $f2, $f0, $f1
mfc1 $s1, $f2
move $s2, $s1
sw $s1, t0
sw $s2, B
jr $ra
main0:
lw $s0, i
li $t1, 4
li.s $f2, 1.5
mfc1 $t0 $f2
li $t8, 4
mult $t8, $t1
mflo $t8
sw $t0, A($t8)
li $t0, 0
move $s0, $t0
sw $s0, i
loop_label0:
bgt $s0, 5, loop_label1
lw $s0, i
li $t8, 4
mult $t8, $s0
mflo $t8
lw $t0, A($t8)
sw $t0, t1
li $t1, 1
add $s0, $s0, $t1
sw $s0, i
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
