

#----------------------------------------
# Generate ASM CODE ...
#----------------------------------------
# Beginning of the data section

.data
A: 		.space 	20
num_2_5: 	.float 	2.5
B: 		.float 	0.0
num_0_0: 	.float 	0.0
num_5_0: 	.float 	5.0
f0: 		.float 	0.0
num_1_5: 	.float 	1.5
f1: 		.float 	0.0
num_5: 		.word 	5
i: 		.word 	0
num_0: 		.word 	0
n: 		.word 	0
num_4: 		.word 	4
num_1: 		.word 	1

# Beginning of the code section

.text
lib_printi:
    # Callee Convention (entering printi):
    sw $s0, -4($sp)
    sw $s1, -8($sp)
    sw $s2, -12($sp)
    sw $s3, -16($sp)
    sw $s4, -20($sp)
    sw $s5, -24($sp)
    sw $s6, -28($sp)
    sw $s7, -32($sp)
    addi $sp, $sp, -32
    sw $ra, -4($sp)
    addi $sp, $sp, -4
    li $v0, 1
    syscall
    addi $sp, $sp, 4
    lw $ra, -4($sp)
    addi $sp, $sp, 32
    lw $s0, -4($sp)
    lw $s1, -8($sp)
    lw $s2, -12($sp)
    lw $s3, -16($sp)
    lw $s4, -20($sp)
    lw $s5, -24($sp)
    lw $s6, -28($sp)
    lw $s7, -32($sp)
    jr $ra
lib_exit:
    # Callee Convention (entering exit):
    sw $s0, -4($sp)
    sw $s1, -8($sp)
    sw $s2, -12($sp)
    sw $s3, -16($sp)
    sw $s4, -20($sp)
    sw $s5, -24($sp)
    sw $s6, -28($sp)
    sw $s7, -32($sp)
    addi $sp, $sp, -32
    sw $ra, -4($sp)
    addi $sp, $sp, -4
    li $v0, 17
    syscall
    addi $sp, $sp, 4
    lw $ra, -4($sp)
    addi $sp, $sp, 32
    lw $s0, -4($sp)
    lw $s1, -8($sp)
    lw $s2, -12($sp)
    lw $s3, -16($sp)
    lw $s4, -20($sp)
    lw $s5, -24($sp)
    lw $s6, -28($sp)
    lw $s7, -32($sp)
    jr $ra
lib_not:
    sw $s0, -4($sp)
    sw $s1, -8($sp)
    sw $s2, -12($sp)
    sw $s3, -16($sp)
    sw $s4, -20($sp)
    sw $s5, -24($sp)
    sw $s6, -28($sp)
    sw $s7, -32($sp)
    addi $sp, $sp, -32
    sw $ra, -4($sp)
    addi $sp, $sp, -4

    # IR: assign, var0, 0,
    la $t0, num_0
    lw $t1, 0($t0)
    move $s8, $t1

    # IR: brneq, i1, 0, _if_label0
    li $t1, 0
    bne $a0, $t1, _if_label0

    # IR: assign, var0, 1,
    li $t1, 1
    move $s8, $t1
_if_label0:

    # IR: breq, var0, 0, _if_label1
    li $t1, 0
    beq $s8, $t1, _if_label1

    # IR: assign, i1, 1,
    li $t1, 1
    move $a0, $t1

    # IR: goto, _if_label2, ,
    j _if_label2
_if_label1:
    # IR: assign, i1, 0,
    li $t1, 0
    move $a0, $t1
_if_label2:
lib_not_end:

    # IR: return, i1, ,
    # Callee Convention (exiting not):
    move $v0, $a0
    addi $sp, $sp, 4
    lw $ra, -4($sp)
    addi $sp, $sp, 32
    lw $s0, -4($sp)
    lw $s1, -8($sp)
    lw $s2, -12($sp)
    lw $s3, -16($sp)
    lw $s4, -20($sp)
    lw $s5, -24($sp)
    lw $s6, -28($sp)
    lw $s7, -32($sp)
    jr $ra

main:

    sw $s0, -4($sp)
    sw $s1, -8($sp)
    sw $s2, -12($sp)
    sw $s3, -16($sp)
    sw $s4, -20($sp)
    sw $s5, -24($sp)
    sw $s6, -28($sp)
    sw $s7, -32($sp)
    addi $sp, $sp, -32
    sw $ra, -4($sp)
    addi $sp, $sp, -4

    # IR:    assign, A, 5, 2.5
    la $t0, num_2_5
    lwc1 $f1, 0($t0)
    la $t0, A
    swc1 $f1, 4($t0)
    swc1 $f1, 8($t0)
    swc1 $f1, 12($t0)
    swc1 $f1, 16($t0)
    swc1 $f1, 20($t0)

    # IR:    assign, B, 0.0,
    la $t0, num_0_0
    lwc1 $f1, 0($t0)
    la $t0, B
    swc1 $f1, 0($t0)

    # IR:    assign, i, 0,
    la $t0, num_0
    lw $t1, 0($t0)
    la $t0, i
    sw $t1, 0($t0)

    # IR:printf:
printf0:

    # IR:main:

    # IR:    array_store, A, 4, 1.5
    la $t0, num_1_5
    lwc1 $f1, 0($t0)
    la $t0, num_4
    lw $t2, 0($t0)
    sll $t2, $t2, 2
    la $t0, A
    add $t0, $t0, $t2
    srl $t2, $t2, 2
    swc1 $f1, 0($t0)

    # IR:    assign, i, 0,
    la $t0, num_0
    lw $t1, 0($t0)
    la $t0, i
    sw $t1, 0($t0)

    # IR:loop_label0:
loop_label0:

    # IR:    brgt, i, 5, loop_label1
    la $t0, i
    lw $t1, 0($t0)
    la $t0, num_5
    lw $t2, 0($t0)
    bgt, $t1, $t2, loop_label1

    # IR:    array_load, f1, A, i
    la $t0, i
    lw $t1, 0($t0)
    sll $t1, $t1, 2
    la $t0, A
    add $t0, $t0, $t1
    srl $t1, $t1, 2
    lwc1 $f1, 0($t0)
    la $t0, f1
    swc1 $f1, 0($t0)

    # IR:    call, printf, f1

    # IR:    add, i, 1, i
    la $t0, i
    lw $t1, 0($t0)
    la $t0, num_1
    lw $t2, 0($t0)
    add $t1, $t1, $t2
    la $t0, i
    sw $t1, 0($t0)

    # IR:    goto, loop_label0, ,
    j loop_label0

    # IR:loop_label1:
loop_label1:

    # IR:    return, , ,
    addi $sp, $sp, 4
    lw $ra, -4($sp)
    addi $sp, $sp, 32
    lw $s0, -4($sp)
    lw $s1, -8($sp)
    lw $s2, -12($sp)
    lw $s3, -16($sp)
    lw $s4, -20($sp)
    lw $s5, -24($sp)
    lw $s6, -28($sp)
    lw $s7, -32($sp)
    jr $ra
#----------------------------------------

