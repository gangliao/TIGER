

# [ RUN ] parsing code... 

# let type id = array [ intlit ] of int ; var id , id : id := intlit ; var id , id : int := intlit ; in for id := intlit to intlit do id := id + id [ id ] * id [ id ] ; enddo ; id ( id ) ; end  

# ----------------------------------------
# Table: # Variables
# Name: X
# ----------------------------------------
# Scope: 0
# Type: int
# Dimension: 100
# Parameters: -
# Parameter types: -
# Parameter dimensions: -
# Return type: -


# ----------------------------------------
# Table: # Variables
# Name: Y
# ----------------------------------------
# Scope: 0
# Type: int
# Dimension: 100
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
# Table: # Variables
# Name: sum
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
# Table: # Types
# Name: ArrayInt
# ----------------------------------------
# Scope: 0
# Type: int
# Dimension: 100
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
#     assign, X, 100, 10
#     assign, Y, 100, 10
#     assign, i, 0,
#     assign, sum, 0,
# main:
#     assign, i, 0,
# loop_label0:
#     brgt, i, 100, loop_label1
#     array_load, t0, X, i
#     array_load, t1, Y, i
#     mult, t0, t1, t2
#     add, sum, t2, t3
#     assign, sum, t3,
#     add, i, 1, i
#     goto, loop_label0, ,
# loop_label1:
#     call, printi, sum
#     return, , ,
# ----------------------------------------


# Default select cfg technique to optimize IR code ...

# Detect block 0 IR line : 5 ~ 6

# Detect block 1 IR line : 7 ~ 15

# Detect block 2 IR line : 16 ~ 18


#----------------------------------------
# Generate ASM CODE ...
#----------------------------------------
# Beginning of the data section

.data
X: 		.space 	400
Y: 		.space 	400
num_100: 		.word 	100
num_10: 		.word 	10
i: 		.word 	0
num_0: 		.word 	0
sum: 		.word 	0
t0: 		.word 	0
t1: 		.word 	0
t2: 		.word 	0
t3: 		.word 	0
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

    # IR:    assign, X, 100, 10
    la $t0, num_10
    lw $t1, 0($t0)
    la $t0, X
    lw $t1, 4($t0)
    lw $t1, 8($t0)
    lw $t1, 12($t0)
    lw $t1, 16($t0)
    lw $t1, 20($t0)
    lw $t1, 24($t0)
    lw $t1, 28($t0)
    lw $t1, 32($t0)
    lw $t1, 36($t0)
    lw $t1, 40($t0)
    lw $t1, 44($t0)
    lw $t1, 48($t0)
    lw $t1, 52($t0)
    lw $t1, 56($t0)
    lw $t1, 60($t0)
    lw $t1, 64($t0)
    lw $t1, 68($t0)
    lw $t1, 72($t0)
    lw $t1, 76($t0)
    lw $t1, 80($t0)
    lw $t1, 84($t0)
    lw $t1, 88($t0)
    lw $t1, 92($t0)
    lw $t1, 96($t0)
    lw $t1, 100($t0)
    lw $t1, 104($t0)
    lw $t1, 108($t0)
    lw $t1, 112($t0)
    lw $t1, 116($t0)
    lw $t1, 120($t0)
    lw $t1, 124($t0)
    lw $t1, 128($t0)
    lw $t1, 132($t0)
    lw $t1, 136($t0)
    lw $t1, 140($t0)
    lw $t1, 144($t0)
    lw $t1, 148($t0)
    lw $t1, 152($t0)
    lw $t1, 156($t0)
    lw $t1, 160($t0)
    lw $t1, 164($t0)
    lw $t1, 168($t0)
    lw $t1, 172($t0)
    lw $t1, 176($t0)
    lw $t1, 180($t0)
    lw $t1, 184($t0)
    lw $t1, 188($t0)
    lw $t1, 192($t0)
    lw $t1, 196($t0)
    lw $t1, 200($t0)
    lw $t1, 204($t0)
    lw $t1, 208($t0)
    lw $t1, 212($t0)
    lw $t1, 216($t0)
    lw $t1, 220($t0)
    lw $t1, 224($t0)
    lw $t1, 228($t0)
    lw $t1, 232($t0)
    lw $t1, 236($t0)
    lw $t1, 240($t0)
    lw $t1, 244($t0)
    lw $t1, 248($t0)
    lw $t1, 252($t0)
    lw $t1, 256($t0)
    lw $t1, 260($t0)
    lw $t1, 264($t0)
    lw $t1, 268($t0)
    lw $t1, 272($t0)
    lw $t1, 276($t0)
    lw $t1, 280($t0)
    lw $t1, 284($t0)
    lw $t1, 288($t0)
    lw $t1, 292($t0)
    lw $t1, 296($t0)
    lw $t1, 300($t0)
    lw $t1, 304($t0)
    lw $t1, 308($t0)
    lw $t1, 312($t0)
    lw $t1, 316($t0)
    lw $t1, 320($t0)
    lw $t1, 324($t0)
    lw $t1, 328($t0)
    lw $t1, 332($t0)
    lw $t1, 336($t0)
    lw $t1, 340($t0)
    lw $t1, 344($t0)
    lw $t1, 348($t0)
    lw $t1, 352($t0)
    lw $t1, 356($t0)
    lw $t1, 360($t0)
    lw $t1, 364($t0)
    lw $t1, 368($t0)
    lw $t1, 372($t0)
    lw $t1, 376($t0)
    lw $t1, 380($t0)
    lw $t1, 384($t0)
    lw $t1, 388($t0)
    lw $t1, 392($t0)
    lw $t1, 396($t0)
    lw $t1, 400($t0)

    # IR:    assign, Y, 100, 10
    la $t0, num_10
    lw $t1, 0($t0)
    la $t0, Y
    lw $t1, 4($t0)
    lw $t1, 8($t0)
    lw $t1, 12($t0)
    lw $t1, 16($t0)
    lw $t1, 20($t0)
    lw $t1, 24($t0)
    lw $t1, 28($t0)
    lw $t1, 32($t0)
    lw $t1, 36($t0)
    lw $t1, 40($t0)
    lw $t1, 44($t0)
    lw $t1, 48($t0)
    lw $t1, 52($t0)
    lw $t1, 56($t0)
    lw $t1, 60($t0)
    lw $t1, 64($t0)
    lw $t1, 68($t0)
    lw $t1, 72($t0)
    lw $t1, 76($t0)
    lw $t1, 80($t0)
    lw $t1, 84($t0)
    lw $t1, 88($t0)
    lw $t1, 92($t0)
    lw $t1, 96($t0)
    lw $t1, 100($t0)
    lw $t1, 104($t0)
    lw $t1, 108($t0)
    lw $t1, 112($t0)
    lw $t1, 116($t0)
    lw $t1, 120($t0)
    lw $t1, 124($t0)
    lw $t1, 128($t0)
    lw $t1, 132($t0)
    lw $t1, 136($t0)
    lw $t1, 140($t0)
    lw $t1, 144($t0)
    lw $t1, 148($t0)
    lw $t1, 152($t0)
    lw $t1, 156($t0)
    lw $t1, 160($t0)
    lw $t1, 164($t0)
    lw $t1, 168($t0)
    lw $t1, 172($t0)
    lw $t1, 176($t0)
    lw $t1, 180($t0)
    lw $t1, 184($t0)
    lw $t1, 188($t0)
    lw $t1, 192($t0)
    lw $t1, 196($t0)
    lw $t1, 200($t0)
    lw $t1, 204($t0)
    lw $t1, 208($t0)
    lw $t1, 212($t0)
    lw $t1, 216($t0)
    lw $t1, 220($t0)
    lw $t1, 224($t0)
    lw $t1, 228($t0)
    lw $t1, 232($t0)
    lw $t1, 236($t0)
    lw $t1, 240($t0)
    lw $t1, 244($t0)
    lw $t1, 248($t0)
    lw $t1, 252($t0)
    lw $t1, 256($t0)
    lw $t1, 260($t0)
    lw $t1, 264($t0)
    lw $t1, 268($t0)
    lw $t1, 272($t0)
    lw $t1, 276($t0)
    lw $t1, 280($t0)
    lw $t1, 284($t0)
    lw $t1, 288($t0)
    lw $t1, 292($t0)
    lw $t1, 296($t0)
    lw $t1, 300($t0)
    lw $t1, 304($t0)
    lw $t1, 308($t0)
    lw $t1, 312($t0)
    lw $t1, 316($t0)
    lw $t1, 320($t0)
    lw $t1, 324($t0)
    lw $t1, 328($t0)
    lw $t1, 332($t0)
    lw $t1, 336($t0)
    lw $t1, 340($t0)
    lw $t1, 344($t0)
    lw $t1, 348($t0)
    lw $t1, 352($t0)
    lw $t1, 356($t0)
    lw $t1, 360($t0)
    lw $t1, 364($t0)
    lw $t1, 368($t0)
    lw $t1, 372($t0)
    lw $t1, 376($t0)
    lw $t1, 380($t0)
    lw $t1, 384($t0)
    lw $t1, 388($t0)
    lw $t1, 392($t0)
    lw $t1, 396($t0)
    lw $t1, 400($t0)

    # IR:    assign, i, 0,
    la $t9, num_0
    lw $t8, 0($t9)
    la $t9, i
    sw $t8, 0($t9)

    # IR:    assign, sum, 0,
    la $t9, num_0
    lw $t8, 0($t9)
    la $t9, sum
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

    la $t9, i
    lw $t0, 0($t9)

    # IR:    assign, i, 0,
    la $t9, num_0
    lw $t8, 0($t9)
    move $t0, $t8

    # Leave block and save registers into vars ... 

    la $t9, i
    sw $t0, 0($t9)
loop_label0:

    # Enter block and load vars into registers ... 

    la $t9, i
    lw $t3, 0($t9)
    la $t9, sum
    lw $t5, 0($t9)
    la $t9, t0
    lw $t2, 0($t9)
    la $t9, t1
    lw $t4, 0($t9)
    la $t9, t2
    lw $t1, 0($t9)
    la $t9, t3
    lw $t0, 0($t9)

    # IR:    brgt, i, 100, loop_label1
    la $t9, num_100
    lw $t9, 0($t9)
    bgt, $t3, $t9, loop_label1

    # IR:    array_load, t0, X, i
    sll $t3, $t3, 2
    la $t8, X
    add $t8, $t8, $t3
    srl $t3, $t3, 2
    lw  $t8, 0($t8)
    move $t2, $t8

    # IR:    array_load, t1, Y, i
    sll $t3, $t3, 2
    la $t8, Y
    add $t8, $t8, $t3
    srl $t3, $t3, 2
    lw  $t8, 0($t8)
    move $t4, $t8

    # IR:    mult, t0, t1, t2
    mult $t2, $t4
    mflo $t2
    move $t1, $t2

    # IR:    add, sum, t2, t3
    add $t5, $t5, $t1
    move $t0, $t5

    # IR:    assign, sum, t3,
    move $t5, $t0

    # IR:    add, i, 1, i
    la $t9, num_1
    lw $t9, 0($t9)
    add $t3, $t3, $t9
    move $t3, $t3

    # Leave block and save registers into vars ... 

    la $t9, i
    sw $t3, 0($t9)
    la $t9, sum
    sw $t5, 0($t9)
    la $t9, t0
    sw $t2, 0($t9)
    la $t9, t1
    sw $t4, 0($t9)
    la $t9, t2
    sw $t1, 0($t9)
    la $t9, t3
    sw $t0, 0($t9)
    j loop_label0
loop_label1:

    # Enter block and load vars into registers ... 

    la $t9, sum
    lw $t0, 0($t9)

    # IR:    call, printi, sum
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

    la $t9, sum
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

