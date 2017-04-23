

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



#----------------------------------------
# Generate ASM CODE ...
#----------------------------------------
# Beginning of the data section

.data
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
X: 		.space 	400
Y: 		.space 	400

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
    sw $t1, 4($t0)
    sw $t1, 8($t0)
    sw $t1, 12($t0)
    sw $t1, 16($t0)
    sw $t1, 20($t0)
    sw $t1, 24($t0)
    sw $t1, 28($t0)
    sw $t1, 32($t0)
    sw $t1, 36($t0)
    sw $t1, 40($t0)
    sw $t1, 44($t0)
    sw $t1, 48($t0)
    sw $t1, 52($t0)
    sw $t1, 56($t0)
    sw $t1, 60($t0)
    sw $t1, 64($t0)
    sw $t1, 68($t0)
    sw $t1, 72($t0)
    sw $t1, 76($t0)
    sw $t1, 80($t0)
    sw $t1, 84($t0)
    sw $t1, 88($t0)
    sw $t1, 92($t0)
    sw $t1, 96($t0)
    sw $t1, 100($t0)
    sw $t1, 104($t0)
    sw $t1, 108($t0)
    sw $t1, 112($t0)
    sw $t1, 116($t0)
    sw $t1, 120($t0)
    sw $t1, 124($t0)
    sw $t1, 128($t0)
    sw $t1, 132($t0)
    sw $t1, 136($t0)
    sw $t1, 140($t0)
    sw $t1, 144($t0)
    sw $t1, 148($t0)
    sw $t1, 152($t0)
    sw $t1, 156($t0)
    sw $t1, 160($t0)
    sw $t1, 164($t0)
    sw $t1, 168($t0)
    sw $t1, 172($t0)
    sw $t1, 176($t0)
    sw $t1, 180($t0)
    sw $t1, 184($t0)
    sw $t1, 188($t0)
    sw $t1, 192($t0)
    sw $t1, 196($t0)
    sw $t1, 200($t0)
    sw $t1, 204($t0)
    sw $t1, 208($t0)
    sw $t1, 212($t0)
    sw $t1, 216($t0)
    sw $t1, 220($t0)
    sw $t1, 224($t0)
    sw $t1, 228($t0)
    sw $t1, 232($t0)
    sw $t1, 236($t0)
    sw $t1, 240($t0)
    sw $t1, 244($t0)
    sw $t1, 248($t0)
    sw $t1, 252($t0)
    sw $t1, 256($t0)
    sw $t1, 260($t0)
    sw $t1, 264($t0)
    sw $t1, 268($t0)
    sw $t1, 272($t0)
    sw $t1, 276($t0)
    sw $t1, 280($t0)
    sw $t1, 284($t0)
    sw $t1, 288($t0)
    sw $t1, 292($t0)
    sw $t1, 296($t0)
    sw $t1, 300($t0)
    sw $t1, 304($t0)
    sw $t1, 308($t0)
    sw $t1, 312($t0)
    sw $t1, 316($t0)
    sw $t1, 320($t0)
    sw $t1, 324($t0)
    sw $t1, 328($t0)
    sw $t1, 332($t0)
    sw $t1, 336($t0)
    sw $t1, 340($t0)
    sw $t1, 344($t0)
    sw $t1, 348($t0)
    sw $t1, 352($t0)
    sw $t1, 356($t0)
    sw $t1, 360($t0)
    sw $t1, 364($t0)
    sw $t1, 368($t0)
    sw $t1, 372($t0)
    sw $t1, 376($t0)
    sw $t1, 380($t0)
    sw $t1, 384($t0)
    sw $t1, 388($t0)
    sw $t1, 392($t0)
    sw $t1, 396($t0)
    sw $t1, 400($t0)

    # IR:    assign, Y, 100, 10
    la $t0, num_10
    lw $t1, 0($t0)
    la $t0, Y
    sw $t1, 4($t0)
    sw $t1, 8($t0)
    sw $t1, 12($t0)
    sw $t1, 16($t0)
    sw $t1, 20($t0)
    sw $t1, 24($t0)
    sw $t1, 28($t0)
    sw $t1, 32($t0)
    sw $t1, 36($t0)
    sw $t1, 40($t0)
    sw $t1, 44($t0)
    sw $t1, 48($t0)
    sw $t1, 52($t0)
    sw $t1, 56($t0)
    sw $t1, 60($t0)
    sw $t1, 64($t0)
    sw $t1, 68($t0)
    sw $t1, 72($t0)
    sw $t1, 76($t0)
    sw $t1, 80($t0)
    sw $t1, 84($t0)
    sw $t1, 88($t0)
    sw $t1, 92($t0)
    sw $t1, 96($t0)
    sw $t1, 100($t0)
    sw $t1, 104($t0)
    sw $t1, 108($t0)
    sw $t1, 112($t0)
    sw $t1, 116($t0)
    sw $t1, 120($t0)
    sw $t1, 124($t0)
    sw $t1, 128($t0)
    sw $t1, 132($t0)
    sw $t1, 136($t0)
    sw $t1, 140($t0)
    sw $t1, 144($t0)
    sw $t1, 148($t0)
    sw $t1, 152($t0)
    sw $t1, 156($t0)
    sw $t1, 160($t0)
    sw $t1, 164($t0)
    sw $t1, 168($t0)
    sw $t1, 172($t0)
    sw $t1, 176($t0)
    sw $t1, 180($t0)
    sw $t1, 184($t0)
    sw $t1, 188($t0)
    sw $t1, 192($t0)
    sw $t1, 196($t0)
    sw $t1, 200($t0)
    sw $t1, 204($t0)
    sw $t1, 208($t0)
    sw $t1, 212($t0)
    sw $t1, 216($t0)
    sw $t1, 220($t0)
    sw $t1, 224($t0)
    sw $t1, 228($t0)
    sw $t1, 232($t0)
    sw $t1, 236($t0)
    sw $t1, 240($t0)
    sw $t1, 244($t0)
    sw $t1, 248($t0)
    sw $t1, 252($t0)
    sw $t1, 256($t0)
    sw $t1, 260($t0)
    sw $t1, 264($t0)
    sw $t1, 268($t0)
    sw $t1, 272($t0)
    sw $t1, 276($t0)
    sw $t1, 280($t0)
    sw $t1, 284($t0)
    sw $t1, 288($t0)
    sw $t1, 292($t0)
    sw $t1, 296($t0)
    sw $t1, 300($t0)
    sw $t1, 304($t0)
    sw $t1, 308($t0)
    sw $t1, 312($t0)
    sw $t1, 316($t0)
    sw $t1, 320($t0)
    sw $t1, 324($t0)
    sw $t1, 328($t0)
    sw $t1, 332($t0)
    sw $t1, 336($t0)
    sw $t1, 340($t0)
    sw $t1, 344($t0)
    sw $t1, 348($t0)
    sw $t1, 352($t0)
    sw $t1, 356($t0)
    sw $t1, 360($t0)
    sw $t1, 364($t0)
    sw $t1, 368($t0)
    sw $t1, 372($t0)
    sw $t1, 376($t0)
    sw $t1, 380($t0)
    sw $t1, 384($t0)
    sw $t1, 388($t0)
    sw $t1, 392($t0)
    sw $t1, 396($t0)
    sw $t1, 400($t0)

    # IR:    assign, i, 0,
    la $t4, num_0
    lw $t5, 0($t4)
    la $t4, i
    sw $t5, 0($t4)

    # IR:    assign, sum, 0,
    la $t4, num_0
    lw $t5, 0($t4)
    la $t4, sum
    sw $t5, 0($t4)

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

    # IR:    assign, i, 0,
    la $t4, num_0
    lw $t5, 0($t4)
    la $t4, i
    sw $t5, 0($t4)
loop_label0:

    # IR:    brgt, i, 100, loop_label1
    la $t0, i
    lw $t1, 0($t0)
    la $t0, num_100
    lw $t2, 0($t0)
    bgt, $t1, $t2, loop_label1

    # IR:    array_load, t0, X, i
    la $t0, i
    lw $t1, 0($t0)
    sll $t1, $t1, 2
    la $t0, X
    add $t0, $t0, $t1
    srl $t1, $t1, 2
    lw  $t1, 0($t0)
    la $t0, t0
    sw $t1, 0($t0)

    # IR:    array_load, t1, Y, i
    la $t0, i
    lw $t1, 0($t0)
    sll $t1, $t1, 2
    la $t0, Y
    add $t0, $t0, $t1
    srl $t1, $t1, 2
    lw  $t1, 0($t0)
    la $t0, t1
    sw $t1, 0($t0)

    # IR:    mult, t0, t1, t2
    la $t4, t0
    lw $t5, 0($t4)
    la $t4, t1
    lw $t4, 0($t4)
    mult $t5, $t4
    mflo $t5
    la $t4, t2
    sw $t5, 0($t4)

    # IR:    add, sum, t2, t3
    la $t4, sum
    lw $t5, 0($t4)
    la $t4, t2
    lw $t4, 0($t4)
    add $t5, $t5, $t4
    la $t4, t3
    sw $t5, 0($t4)

    # IR:    assign, sum, t3,
    la $t4, t3
    lw $t5, 0($t4)
    la $t4, sum
    sw $t5, 0($t4)

    # IR:    add, i, 1, i
    la $t4, i
    lw $t5, 0($t4)
    la $t4, num_1
    lw $t4, 0($t4)
    add $t5, $t5, $t4
    la $t4, i
    sw $t5, 0($t4)
    j loop_label0
loop_label1:

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
    la $t4, sum
    lw $a0, 0($t4)
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

