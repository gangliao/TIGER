

#----------------------------------------
# Generate ASM CODE ...
#----------------------------------------
# Beginning of the data section

.data
A: 		.float 	0.0
num_0_0: 	.float 	0.0
B: 		.float 	0.0
C: 		.space 	20
num_2_5: 	.float 	2.5
num_5_0: 	.float 	5.0
f0: 		.float 	0.0
num_1_5: 	.float 	1.5
num_5: 		.word 	5
n: 		.word 	0

# Beginning of the code section

.text

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

    # IR:    assign, A, 0.0,
    la $t0, num_0_0
    lwc1 $f1, 0($t0)
    la $t0, A
    swc1 $f1, 0($t0)

    # IR:    assign, B, 0.0,
    la $t0, num_0_0
    lwc1 $f1, 0($t0)
    la $t0, B
    swc1 $f1, 0($t0)

    # IR:    assign, C, 5, 2.5
    la $t0, num_2_5
    lwc1 $f1, 0($t0)
    la $t0, C
    swc1 $f1, 4($t0)
    swc1 $f1, 8($t0)
    swc1 $f1, 12($t0)
    swc1 $f1, 16($t0)
    swc1 $f1, 20($t0)

    # IR:printf:
printf0:

    # IR:main:

    # IR:    assign, A, 1.5,
    la $t0, num_1_5
    lwc1 $f1, 0($t0)
    la $t0, A
    swc1 $f1, 0($t0)

    # IR:    call, printf, A

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

