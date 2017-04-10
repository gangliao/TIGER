

#----------------------------------------
# Generate ASM CODE ...
#----------------------------------------
# Beginning of the data section

.data
m: 		.word 	5
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

    # IR:    assign, m, 5,
    la $t0, num_5
    lw $t1, 0($t0)
    la $t0, m
    sw $t1, 0($t0)

    # IR:print:
print0:

    # IR:main:

    # IR:    call, print, m

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

