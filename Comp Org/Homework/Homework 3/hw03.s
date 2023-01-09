################################################################################
# Homework 3: Matrix-matrix multiplication
# CSCI-2500 Fall 2021
################################################################################
## Data segment
################################################################################
.data
  newline:  .asciiz "\n"
  tab:      .asciiz "\t"

################################################################################
## Text segment
################################################################################
.text
  #.align 2          # instructions must be on word boundaries
  .globl main       # main is a global label
  .globl mm_read    # read in NxM matrix
  .globl mm_alloc   # allocate space for NxM matrix
  .globl mm_print   # print out formatted NxM matrix
  .globl mm_mult    # multiple two non-square matrices

################################################################################
# Main segment
################################################################################
main:
  # save return address on the stack
  sub $sp, $sp, 4   
  sw  $ra, 0($sp)

  # Read in a matrix 1 of some NxM size
  # Return in $v0 - N
  # Return in $v1 - M
  # Return in 0($sp) - base address of matrix
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 1 values
  move  $s0, $v0
  move  $s1, $v1
  lw    $s2, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 1
  move  $a0, $s0
  move  $a1, $s1
  move  $a2, $s2
  jal mm_print

  # Read in matrix 2 of some NxM size
  sub $sp, $sp, 4   # make space on stack for return value
  jal mm_read

  # save matrix 2 values
  move  $s3, $v0
  move  $s4, $v1
  lw    $s5, 0($sp)
  add   $sp, $sp, 4   # restore stack

  # Setup arguments and print out matrix 2
  move  $a0, $s3
  move  $a1, $s4
  move  $a2, $s5
  jal mm_print

  # Setup arguments and call mm_mult
  # Note: 5 arguments, so we need to use stack
  # Note: num cols matrix 1 == num rows matrix 2
  move  $a0, $s0    # num rows matrix1
  move  $a1, $s1    # num cols matrix1
  move  $a2, $s2    # address of matrix 1
  move  $a3, $s4    # num cols matrix2
  sub   $sp, $sp, 4   
  sw    $s5, 0($sp) # address of matrix 2
  jal mm_mult
  add   $sp, $sp, 4

  # print the result
  move $a0, $s0 
  move $a1, $s4
  move $a2, $v0
  jal mm_print

  # restore $ra, free stack space, and return
  lw  $ra, 0($sp)
  add $sp, $sp, 4
  jr  $ra

################################################################################
# mm_read: Read in a NxM matrix from standard input
# Note: You will be calling mm_alloc from this function, so you'll need to save 
#       $ra on the stack. Also, you'll be returning 3 values back to main, one
#       value (the matrix base address), will need to be saved on the stack.
#       Matrix format is:
#         First line:  N = num rows
#         Second line: M = num cols
#         Subsequent lines contain one value each, filling each row in turn
#         (same format as hw01, except N,M on separate lines)  
# Input:
#   None
# Return:
#   v0 = N
#   v1 = M
#   0($sp) = base address of matrix
################################################################################
mm_read:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 16
  sw $s0, 0($sp)
  sw $s1, 4($sp)
  sw $s2, 8($sp)
  sw $ra, 12($sp)
  
  # get N
  li $v0, 5
  syscall 
  move $s0, $v0

  # get M
  li $v0, 5
  syscall 
  move $s1, $v0

  # Setup up arguments and call mm_alloc - v0 is returned as base address
  move $a0, $s0
  move $a1, $s1
  jal mm_alloc
  
  move $s3, $v0
 

  # do nested loops to read in values
  mul $t1, $s0, $s1 #loop counter
  move $t3, $s3 #array 
  loop:
  	li $v0, 5 #reads
  	syscall 
  	sw $v0, 0($t3) #adds to array
  	addi $t3, $t3, 4 #moves to next index
  	
  	sub $t1, $t1, 1 #decrements
  	bne $t1, $zero, loop
  	
  # setup up return values
  # Note: third return value goes on the stack *after* restoration below
  move $v0, $s0
  move $v1, $s1
	
  # restore stack, ra, and any saved registers, if necessary
  lw $s0, 0($sp)
  lw $s1, 4($sp)
  lw $s2, 8($sp)
  lw $ra, 12($sp)
  addi $sp, $sp, 16
  
  sw $s3, 0($sp)
  
  # return to main
  jr  $ra

################################################################################
# mm_alloc: Allocate an NxM matrix
# Note: You can just allocate a single array of length N*M.
# Input:
#   a0 = N
#   a1 = M
# Return:
#   v0 = base address of allocated space
################################################################################
mm_alloc:
  # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 12
  sw $ra, 0($sp)
  sw $s0, 4($sp)
  sw $s1, 8($sp)

  # Allocate sufficient space
  move $s0, $a0 #move rows
  move $s1, $a1 #move cols
  li $t3, 4 
  mul $t4, $a0, $a1 #num total spots
  mul $t4, $t4, $t3 #total bytes
  
  li $v0, 9 #heap call
  move $a0, $t4 #total size
  syscall
  
  # set return value
  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  lw $s0, 4($sp)
  lw $s1, 8($sp)
  addi $sp, $sp, 12
  # return to main
  jr  $ra

################################################################################
# mm_print: Output the formatted contents of NxM matrix
# Note: Within a row, separate each value by tab
#       End each row with a newline
# Input:
#   a0 = N
#   a1 = M
#   a2 = base pointer
# Return:
#   None
################################################################################
mm_print:
   # save return address and any saved registers on the stack, if necessary
  sub $sp, $sp, 28
  sw $ra, 0($sp)
  sw $s5, 4($sp)
  sw $s4, 8($sp)
  sw $s3, 12($sp)
  sw $s2, 16($sp)
  sw $s1, 20($sp) 
  sw $s0, 24($sp)

  # do nested loops to print out values
  move $s0, $a0
  move $s1, $a1
  move $s2, $a2
  
  move $t1, $s0 #loop counter
  move $t3, $s2 #array 
  printloop:
  	add $t0, $zero, $s1 #num cols
  	printloop2:
  		li $v0, 1 #print call
  		lw $a0, 0($t3)
  		syscall 
  		addi $t3, $t3, 4 #moves to next index
  		
  		li $v0, 4 #print call
  		la $a0, tab
  		syscall
  		
  		sub $t0, $t0, 1 #decrements
  		bne $t0, $zero, printloop2
  		
  	li $v0, 4
  	la $a0, newline
  	syscall 
  	
  	sub $t1, $t1, 1 #decrements
  	bne $t1, $zero, printloop
  	
  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  lw $s5, 4($sp)
  lw $s4, 8($sp)
  lw $s3, 12($sp)
  lw $s2, 16($sp)
  lw $s1, 20($sp)
  lw $s0, 24($sp)
  addi $sp, $sp, 28
  # return to main
  jr  $ra

################################################################################
# mm_mult: Perform the actual NxM matrix multiplication
# Input:
#   a0 = N for matrix 1
#   a1 = M for matrix 1 <-> N for matrix 2
#   a2 = base address for matrix 1
#   a3 = M for matrix 2
#   0($sp) = base address for matrix 2
# Return:
#   v0 = base address for result matrix
################################################################################
mm_mult:
  # save return address and any saved registers on the stack, if necessary
  lw $t0, 0($sp) # base address
  sub $sp, $sp, 28
  sw $ra, 0($sp)
  sw $s5, 4($sp)
  sw $s4, 8($sp)
  sw $s3, 12($sp)
  sw $s2, 16($sp)
  sw $s1, 20($sp)
  sw $s0, 24($sp)  
  # set up and call mm_alloc for result matrix
  move $s0, $a0 #rows matrix 1
  move $s1, $a1 #cols matrix 1 and rows matrix 2
  move $s3, $a2 #base address matrix 1
  move $s2, $a3 #cols for matrix 2
  move $s4, $t0 #base address matrix 2

  move $a0, $s0
  move $a1, $s2
  jal mm_alloc
  move $s5, $v0 #base address of final matrix
 
  # three nested loops for matrix-matrix multiplication
  move $s6, $s5 #holds next index 
  move $t3, $s0 #rows in matrix 1

  mloop1: #matrix 1 rows
    move $t0, $s2
    mloop2: #matrix 2 cols
      move $t1, $s1
      li $t2, 0
      mloop3: #matrix 1 cols and matrix 2 rows
        sub $t4, $t3, $s0 #get row
        sub $t5, $s1, $t1 #get col
        li $t6, 4
        
        mul $t4, $t4, $s1
        add $t4, $t4, $t5 #matrix 1 index
        mul $t4, $t4, $t6 #the offset to get the new index
        add $t4, $t4, $s3 #location of index item
        lw $t6, 0($t4) #holds value

        sub $t4, $s2, $t0 #gets col
  			mul $t5, $t5, $s2 
  			add $t5, $t5, $t4 #matrix 2 index
  		  li $t4, 4 #Size of int
  			mul $t5, $t5, $t4 #offset to get new index
  			add $t5, $t5, $s4 #location of index item
  			lw $t7, 0($t5) #holds value

  			mul $t6, $t6, $t7 #multiplies the values
  			add $t2, $t2, $t6 #adds them into the single value that will be added to the final matrix
  			
  			sub $t1, $t1, 1
  			bne $t1, $zero, mloop3
  			
  		sw $t2, 0($s6) #adds the values into the matrix
  		addi $s6, $s6, 4 #next index
  		
  		sub $t0, $t0, 1
  		bne $t0, $zero, mloop2
  	
  	sub $s0, $s0, 1
  	bne $s0, $zero, mloop1
  		



  # set return value
  move $v0, $s5
  # restore stack, ra, and any saved registers, if necessary
  lw $ra, 0($sp)
  lw $s5, 4($sp)
  lw $s4, 8($sp)
  lw $s3, 12($sp)
  lw $s2, 16($sp)
  lw $s1, 20($sp)
  lw $s0, 24($sp)  
  addi $sp, $sp, 28
  # return to main
  jr  $ra