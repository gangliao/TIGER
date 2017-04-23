

# [ RUN ] parsing code... 

# let var id , id : int := intlit ; in if ( id = id ) then id := ( id + intlit ) / intlit - id * id - intlit + id ; else id := intlit ; endif ; id ( id ) ; end  

# ----------------------------------------
# Table: # Variables
# Name: a
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
# Name: b
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
# Table: # Variables
# Name: t1
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
# Name: t2
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
# Name: t3
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
# Name: t4
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
# Name: t5
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
#     assign, a, 0,
#     assign, b, 0,
# main:
#     brneq, a, b, if_label0
#     add, b, 2, t0
#     div, t0, 5, t1
#     mult, a, b, t2
#     sub, t1, t2, t3
#     sub, t3, 5, t4
#     add, t4, b, t5
#     assign, a, t5,
#     goto, if_label1, ,
# if_label0:
#     assign, a, 2,
# if_label1:
#     call, printi, a
#     return, , ,
# ----------------------------------------


# Default select cfg technique to optimize IR code ...

# Detect block 0 IR line : 3 ~ 12

# Detect block 1 IR line : 13 ~ 14

# Detect block 2 IR line : 15 ~ 17


#----------------------------------------
# Generate ASM CODE ...
#----------------------------------------
# Beginning of the data section

.data
a: 		.word 	0
num_0: 		.word 	0
nb: 		.word 	0
num_2: 		.word 	2
t0: 		.word 	0
num_5: 		.word 	5
t1: 		.word 	0
t2: 		.word 	0
t3: 		.word 	0
t4: 		.word 	0
t5: 		.word 	0

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

    # IR:    assign, a, 0,
    la $t9, num_0
    lw $t8, 0($t9)
    la $t9, a
    sw $t8, 0($t9)

    # IR:    assign, nb, 0, 
    la $t9, num_0
    lw $t8, 0($t9)
    la $t9, nb
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

    la $t9, a
    lw $t5, 0($t9)
    la $t9, nb
    lw $t7, 0($t9)
    la $t9, t0
    lw $t4, 0($t9)
    la $t9, t1
    lw $t6, 0($t9)
    la $t9, t2
    lw $t2, 0($t9)
    la $t9, t3
    lw $t1, 0($t9)
    la $t9, t4
    lw $t0, 0($t9)
    la $t9, t5
    lw $t3, 0($t9)

    # IR:    brneq, a, nb, if_label0, 
    bne, $t5, $t7, if_label0

    # IR:    add, nb, 2, t0, 
    la $t9, num_2
    lw $t9, 0($t9)
    add $t8, $t7, $t9
    move $t4, $t8

    # IR:    div, t0, 5, t1
    la $t9, num_5
    lw $t9, 0($t9)
    div $t4, $t9
    mflo $t4
    move $t6, $t4

    # IR:    mult, a, nb, t2, 
    mult $t5, $t7
    mflo $t5
    move $t2, $t5

    # IR:    sub, t1, t2, t3
    sub $t8, $t6, $t2
    move $t1, $t8

    # IR:    sub, t3, 5, t4
    la $t9, num_5
    lw $t9, 0($t9)
    sub $t8, $t1, $t9
    move $t0, $t8

    # IR:    add, t4, nb, t5, 
    add $t8, $t0, $t7
    move $t3, $t8

    # IR:    assign, a, t5,
    move $t5, $t3

    # Leave block and save registers into vars ... 

    la $t9, a
    sw $t5, 0($t9)
    la $t9, nb
    sw $t7, 0($t9)
    la $t9, t0
    sw $t4, 0($t9)
    la $t9, t1
    sw $t6, 0($t9)
    la $t9, t2
    sw $t2, 0($t9)
    la $t9, t3
    sw $t1, 0($t9)
    la $t9, t4
    sw $t0, 0($t9)
    la $t9, t5
    sw $t3, 0($t9)
    j if_label1
if_label0:

    # Enter block and load vars into registers ... 

    la $t9, a
    lw $t0, 0($t9)

    # IR:    assign, a, 2,
    la $t9, num_2
    lw $t8, 0($t9)
    move $t0, $t8

    # Leave block and save registers into vars ... 

    la $t9, a
    sw $t0, 0($t9)
if_label1:

    # Enter block and load vars into registers ... 

    la $t9, a
    lw $t0, 0($t9)

    # IR:    call, printi, a
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

    la $t9, a
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

