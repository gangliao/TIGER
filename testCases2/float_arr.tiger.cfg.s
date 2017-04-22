

# [ RUN ] parsing code... 

# let type id = array [ intlit ] of float ; var id : id := floatlit ; var id : float ; var id : int := intlit ; function id ( id : float ) begin id := id + floatlit ; end ; in id [ intlit ] := floatlit ; for id := intlit to intlit do id ( id [ id ] ) ; enddo ; end  

# ----------------------------------------
# Table: # Variables
# Name: A
# ----------------------------------------
# Scope: 0
# Type: float
# Dimension: 5
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Variables
# Name: B
# ----------------------------------------
# Scope: 0
# Type: float
# Dimension: 0
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Variables
# Name: f1
# ----------------------------------------
# Scope: 0
# Type: float
# Dimension: 0
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Variables
# Name: i
# ----------------------------------------
# Scope: 0
# Type: int
# Dimension: 0
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Types
# Name: ArrayFloat
# ----------------------------------------
# Scope: 0
# Type: float
# Dimension: 5
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Types
# Name: float
# ----------------------------------------
# Scope: 0
# Type: float
# Dimension: 0
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Types
# Name: int
# ----------------------------------------
# Scope: 0
# Type: int
# Dimension: 0
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Functions
# Name: exit
# ----------------------------------------
# Scope: 0
# Type: -
# Dimension: -
# Parameters: [i]
# Parameter types: [int]
# Parameter dimensions: [0]
# Return type: -


# ----------------------------------------
# Table: # Functions
# Name: flush
# ----------------------------------------
# Scope: 0
# Type: -
# Dimension: -
# Parameters: []
# Parameter types: []
# Parameter dimensions: []
# Return type: -


# ----------------------------------------
# Table: # Functions
# Name: not
# ----------------------------------------
# Scope: 0
# Type: -
# Dimension: -
# Parameters: [i]
# Parameter types: [int]
# Parameter dimensions: [0]
# Return type: int


# ----------------------------------------
# Table: # Functions
# Name: printf
# ----------------------------------------
# Scope: 0
# Type: -
# Dimension: -
# Parameters: [n]
# Parameter types: [float]
# Parameter dimensions: [0]
# Return type: -


# ----------------------------------------
# Table: # Functions
# Name: printi
# ----------------------------------------
# Scope: 0
# Type: -
# Dimension: -
# Parameters: [i]
# Parameter types: [int]
# Parameter dimensions: [0]
# Return type: -
# ----------------------------------------



# [ OK ] successful parse...


# ----------------------------------------
# Generate IR CODE ...
# ----------------------------------------
#     assign, A, 5, 2.5
#     assign, B, 0.0,
#     assign, i, 0,
# printf:
#     add, n, 5.0, f0
#     assign, B, f0,
#     return, , ,
# main:
#     array_store, A, 4, 1.5
#     assign, i, 0,
# loop_label0:
#     brgt, i, 5, loop_label1
#     array_load, f1, A, i
#     call, printf, f1
#     add, i, 1, i
#     goto, loop_label0, ,
# loop_label1:
#     return, , ,
# ----------------------------------------


# Default select cfg technique to optimize IR code ...

# Detect block 0 IR line : 8 ~ 10

# Detect block 1 IR line : 11 ~ 16

# Detect block 2 IR line : 17 ~ 18


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
    la $t9, num_0_0
    lwc1 $f16, 0($t9)
    la $t9, B
    swc1 $f16, 0($t9)

    # IR:    assign, i, 0,
    la $t9, num_0
    lw $t8, 0($t9)
    la $t9, i
    sw $t8, 0($t9)

    # IR: goto, main0
    j main0

printf:

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

    # IR:    add, n, 5.0, f0
    la $t9, num_5_0
    lwc1 $f17, 0($t9)
    add.s  $f12, $f12, $f17
    la $t9, f0
    swc1 $f12, 0($t9)

    # IR:    assign, B, f0,
    la $t9, f0
    lwc1 $f16, 0($t9)
    la $t9, B
    swc1 $f16, 0($t9)

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

main0:

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

    # Enter block and load vars into registers ... 

    la $t9, i
    lw $t0, 0($t9)

    # IR:    array_store, A, 4, 1.5
    la $t9, num_4
    lw $t9, 0($t9)
    sll $t9, $t9, 2
    la $t8, A
    add $t8, $t8, $t9
    srl $t9, $t9, 2
    la $t9, num_1_5
    lwc1 $f16, 0($t9)
    swc1 $f16, 0($t8)

    # IR:    assign, i, 0,
    la $t9, num_0
    lw $t8, 0($t9)
    move $t0, $t8

    # Leave block and save registers into vars ... 

    la $t9, i
    sw $t0, 0($t9)
loop_label0:

    # Enter block and load vars into registers ... 

    la $t9, f1
    lwc1 $f0, 0($t9)
    la $t9, i
    lw $t0, 0($t9)

    # IR:    brgt, i, 5, loop_label1
    la $t9, num_5
    lw $t9, 0($t9)
    bgt, $t0, $t9, loop_label1

    # IR:    array_load, f1, A, i
    sll $t0, $t0, 2
    la $t8, A
    add $t8, $t8, $t0
    srl $t0, $t0, 2
    lwc1 $f16, 0($t8)
    mov.s $f0, $f16

    # IR:    call, printf, f1
    sw $t4, -4($sp)
    sw $t5, -8($sp)
    sw $t6, -12($sp)
    sw $t7, -16($sp)
    sw $t8, -20($sp)
    sw $t9, -24($sp)
    addi $sp, $sp, -24
    swc1 $f12, -4($sp)
    swc1 $f13, -8($sp)
    swc1 $f14, -12($sp)
    addi $sp, $sp, -12
    sw $a0, -4($sp)
    sw $a1, -8($sp)
    sw $a2, -12($sp)
    sw $a3, -16($sp)
    addi $sp, $sp, -16
    mov.s $f12, $f0
    jal printf
    addi $sp, $sp, 16
    lw $a0, -4($sp)
    lw $a1, -8($sp)
    lw $a2, -12($sp)
    lw $a3, -16($sp)
    addi $sp, $sp, 12
    lwc1 $f12, -4($sp)
    lwc1 $f13, -8($sp)
    lwc1 $f14, -12($sp)
    addi $sp, $sp, 24
    lw $t4, -4($sp)
    lw $t5, -8($sp)
    lw $t6, -12($sp)
    lw $t7, -16($sp)
    lw $t8, -20($sp)
    lw $t9, -24($sp)

    # IR:    add, i, 1, i
    la $t9, num_1
    lw $t9, 0($t9)
    add $t0, $t0, $t9
    move $t0, $t0

    # Leave block and save registers into vars ... 

    la $t9, f1
    swc1 $f0, 0($t9)
    la $t9, i
    sw $t0, 0($t9)
    j loop_label0
loop_label1:

    # Enter block and load vars into registers ... 


    # Leave block and save registers into vars ... 


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

