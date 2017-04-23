

# [ RUN ] parsing code... 

# let var id : int := intlit ; var id : int ; var id : int := intlit ; in for id := intlit to id do id := id * id ; enddo ; id ( id ) ; end  

# ----------------------------------------
# Table: # Variables
# Name: loopCounter
# ----------------------------------------
# Scope: 0
# Type: int
# Dimension: 0
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Variables
# Name: number
# ----------------------------------------
# Scope: 0
# Type: int
# Dimension: 0
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Variables
# Name: result
# ----------------------------------------
# Scope: 0
# Type: int
# Dimension: 0
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Variables
# Name: t0
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
#     assign, number, 8,
#     assign, loopCounter, 0,
#     assign, result, 1,
# main:
#     assign, loopCounter, 1,
# loop_label0:
#     brgt, loopCounter, number, loop_label1
#     mult, result, loopCounter, t0
#     assign, result, t0,
#     add, loopCounter, 1, loopCounter
#     goto, loop_label0, ,
# loop_label1:
#     call, printi, result
#     return, , ,
# ----------------------------------------


# Default select cfg technique to optimize IR code ...

# Detect block 0 IR line : 4 ~ 5

# Detect block 1 IR line : 6 ~ 11

# Detect block 2 IR line : 12 ~ 14


#----------------------------------------
# Generate ASM CODE ...
#----------------------------------------
# Beginning of the data section

.data
number: 		.word 	8
num_8: 		.word 	8
loopCounter: 		.word 	0
num_0: 		.word 	0
result: 		.word 	1
num_1: 		.word 	1
t0: 		.word 	0

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

    # IR:    assign, number, 8,
    la $t9, num_8
    lw $t8, 0($t9)
    la $t9, number
    sw $t8, 0($t9)

    # IR:    assign, loopCounter, 0,
    la $t9, num_0
    lw $t8, 0($t9)
    la $t9, loopCounter
    sw $t8, 0($t9)

    # IR:    assign, result, 1,
    la $t9, num_1
    lw $t8, 0($t9)
    la $t9, result
    sw $t8, 0($t9)

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

    la $t9, loopCounter
    lw $t0, 0($t9)

    # IR:    assign, loopCounter, 1,
    la $t9, num_1
    lw $t8, 0($t9)
    move $t0, $t8

    # Leave block and save registers into vars ... 

    la $t9, loopCounter
    sw $t0, 0($t9)
loop_label0:

    # Enter block and load vars into registers ... 

    la $t9, loopCounter
    lw $t3, 0($t9)
    la $t9, number
    lw $t2, 0($t9)
    la $t9, result
    lw $t1, 0($t9)
    la $t9, t0
    lw $t0, 0($t9)

    # IR:    brgt, loopCounter, number, loop_label1
    bgt, $t3, $t2, loop_label1

    # IR:    mult, result, loopCounter, t0
    mult $t1, $t3
    mflo $t1
    move $t0, $t1

    # IR:    assign, result, t0,
    move $t1, $t0

    # IR:    add, loopCounter, 1, loopCounter
    la $t9, num_1
    lw $t9, 0($t9)
    add $t8, $t3, $t9
    move $t3, $t8

    # Leave block and save registers into vars ... 

    la $t9, loopCounter
    sw $t3, 0($t9)
    la $t9, number
    sw $t2, 0($t9)
    la $t9, result
    sw $t1, 0($t9)
    la $t9, t0
    sw $t0, 0($t9)
    j loop_label0
loop_label1:

    # Enter block and load vars into registers ... 

    la $t9, result
    lw $t0, 0($t9)

    # IR:    call, printi, result
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
    move $a0, $t0
    jal lib_printi
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

    # Leave block and save registers into vars ... 

    la $t9, result
    sw $t0, 0($t9)

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

