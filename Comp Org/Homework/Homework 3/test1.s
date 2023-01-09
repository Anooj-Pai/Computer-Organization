$LC0:
        .ascii  "mm_alloc: allocation failed.\000"
mm_alloc:
        addiu   $sp,$sp,-48
        sw      $31,44($sp)
        sw      $fp,40($sp)
        sw      $16,36($sp)
        move    $fp,$sp
        sw      $4,48($fp)
        lw      $2,48($fp)
        nop
        lw      $2,4($2)
        li      $5,4                        # 0x4
        move    $4,$2
        jal     calloc
        nop

        move    $3,$2
        lw      $2,48($fp)
        nop
        sw      $3,0($2)
        lw      $2,48($fp)
        nop
        lw      $2,0($2)
        nop
        bne     $2,$0,$L2
        nop

        lui     $2,mfhi($LC0)
        addiu   $4,$2,mflo($LC0)
        jal     puts
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L2:
        sw      $0,24($fp)
        b       $L3
        nop

$L5:
        lw      $2,48($fp)
        nop
        lw      $3,0($2)
        lw      $2,24($fp)
        nop
        sll     $2,$2,2
        addu    $16,$3,$2
        lw      $2,48($fp)
        nop
        lw      $2,8($2)
        li      $5,8                        # 0x8
        move    $4,$2
        jal     calloc
        nop

        sw      $2,0($16)
        lw      $2,48($fp)
        nop
        lw      $3,0($2)
        lw      $2,24($fp)
        nop
        sll     $2,$2,2
        addu    $2,$3,$2
        lw      $2,0($2)
        nop
        bne     $2,$0,$L4
        nop

        lui     $2,%hi($LC0)
        addiu   $4,$2,%lo($LC0)
        jal     puts
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L4:
        lw      $2,24($fp)
        nop
        addiu   $2,$2,1
        sw      $2,24($fp)
$L3:
        lw      $2,48($fp)
        nop
        lw      $3,4($2)
        lw      $2,24($fp)
        nop
        slt     $2,$2,$3
        bne     $2,$0,$L5
        nop

        move    $2,$0
        move    $sp,$fp
        lw      $31,44($sp)
        lw      $fp,40($sp)
        lw      $16,36($sp)
        addiu   $sp,$sp,48
        j       $31
        nop

mm_free:
        addiu   $sp,$sp,-40
        sw      $31,36($sp)
        sw      $fp,32($sp)
        move    $fp,$sp
        sw      $4,40($fp)
        sw      $0,24($fp)
        b       $L8
        nop

$L9:
        lw      $2,40($fp)
        nop
        lw      $3,0($2)
        lw      $2,24($fp)
        nop
        sll     $2,$2,2
        addu    $2,$3,$2
        lw      $2,0($2)
        nop
        move    $4,$2
        jal     free
        nop

        lw      $2,24($fp)
        nop
        addiu   $2,$2,1
        sw      $2,24($fp)
$L8:
        lw      $2,40($fp)
        nop
        lw      $3,4($2)
        lw      $2,24($fp)
        nop
        slt     $2,$2,$3
        bne     $2,$0,$L9
        nop

        lw      $2,40($fp)
        nop
        lw      $2,0($2)
        nop
        move    $4,$2
        jal     free
        nop

        move    $2,$0
        move    $sp,$fp
        lw      $31,36($sp)
        lw      $fp,32($sp)
        addiu   $sp,$sp,40
        j       $31
        nop

$LC1:
        .ascii  "\012/******** START of 2-D Matrix **********************"
        .ascii  "************/\000"
$LC2:
        .ascii  "%10.2lf\011\000"
$LC3:
        .ascii  "/******** END of 2-D Matrix ****************************"
        .ascii  "********/\000"
mm_print:
        addiu   $sp,$sp,-40
        sw      $31,36($sp)
        sw      $fp,32($sp)
        move    $fp,$sp
        sw      $4,40($fp)
        lui     $2,%hi($LC1)
        addiu   $4,$2,%lo($LC1)
        jal     puts
        nop

        sw      $0,24($fp)
        b       $L12
        nop

$L15:
        sw      $0,28($fp)
        b       $L13
        nop

$L14:
        lw      $2,40($fp)
        nop
        lw      $3,0($2)
        lw      $2,24($fp)
        nop
        sll     $2,$2,2
        addu    $2,$3,$2
        lw      $3,0($2)
        lw      $2,28($fp)
        nop
        sll     $2,$2,3
        addu    $2,$3,$2
        lwc1    $f0,4($2)
        nop
        lwc1    $f1,0($2)
        nop
        mfc1    $7,$f0
        mfc1    $6,$f1
        lui     $2,%hi($LC2)
        addiu   $4,$2,%lo($LC2)
        jal     printf
        nop

        lw      $2,28($fp)
        nop
        addiu   $2,$2,1
        sw      $2,28($fp)
$L13:
        lw      $2,40($fp)
        nop
        lw      $3,8($2)
        lw      $2,28($fp)
        nop
        slt     $2,$2,$3
        bne     $2,$0,$L14
        nop

        li      $4,10                 # 0xa
        jal     putchar
        nop

        lw      $2,24($fp)
        nop
        addiu   $2,$2,1
        sw      $2,24($fp)
$L12:
        lw      $2,40($fp)
        nop
        lw      $3,4($2)
        lw      $2,24($fp)
        nop
        slt     $2,$2,$3
        bne     $2,$0,$L15
        nop

        lui     $2,%hi($LC3)
        addiu   $4,$2,%lo($LC3)
        jal     puts
        nop

        move    $2,$0
        move    $sp,$fp
        lw      $31,36($sp)
        lw      $fp,32($sp)
        addiu   $sp,$sp,40
        j       $31
        nop

$LC4:
        .ascii  "r\000"
$LC5:
        .ascii  "mm_read: failed to open file.\000"
$LC6:
        .ascii  "mm_read: failed to read from file.\000"
$LC7:
        .ascii  "%d %d\000"
$LC8:
        .ascii  "mm_read: failed to read matrix dimensions.\000"
$LC9:
        .ascii  "mm_read: failed to read matrix values.\000"
$LC10:
        .ascii  "%lf\000"
$LC11:
        .ascii  "mm_read: fclose failed: %s\012\000"
mm_read:
        addiu   $sp,$sp,-304
        sw      $31,300($sp)
        sw      $fp,296($sp)
        move    $fp,$sp
        sw      $4,304($fp)
        sw      $5,308($fp)
        sw      $0,32($fp)
        lw      $2,308($fp)
        nop
        sw      $0,0($2)
        lui     $2,%hi($LC4)
        addiu   $5,$2,%lo($LC4)
        lw      $4,304($fp)
        jal     fopen
        nop

        sw      $2,32($fp)
        lw      $2,32($fp)
        nop
        bne     $2,$0,$L18
        nop

        lui     $2,%hi($LC5)
        addiu   $4,$2,%lo($LC5)
        jal     printf
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L18:
        addiu   $2,$fp,36
        lw      $6,32($fp)
        li      $5,256                  # 0x100
        move    $4,$2
        jal     fgets
        nop

        bne     $2,$0,$L19
        nop

        lui     $2,%hi($LC6)
        addiu   $4,$2,%lo($LC6)
        jal     printf
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L19:
        lw      $2,308($fp)
        nop
        addiu   $4,$2,4
        lw      $2,308($fp)
        nop
        addiu   $2,$2,8
        addiu   $3,$fp,36
        move    $7,$2
        move    $6,$4
        lui     $2,%hi($LC7)
        addiu   $5,$2,%lo($LC7)
        move    $4,$3
        jal     __isoc99_sscanf
        nop

        move    $3,$2
        li      $2,2                        # 0x2
        beq     $3,$2,$L20
        nop

        lui     $2,%hi($LC8)
        addiu   $4,$2,%lo($LC8)
        jal     puts
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L20:
        lw      $4,308($fp)
        jal     mm_alloc
        nop

        sw      $0,24($fp)
        b       $L21
        nop

$L26:
        sw      $0,28($fp)
        b       $L22
        nop

$L25:
        addiu   $2,$fp,36
        lw      $6,32($fp)
        li      $5,256                  # 0x100
        move    $4,$2
        jal     fgets
        nop

        bne     $2,$0,$L23
        nop

        lui     $2,%hi($LC9)
        addiu   $4,$2,%lo($LC9)
        jal     puts
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L23:
        lw      $2,308($fp)
        nop
        lw      $3,0($2)
        lw      $2,24($fp)
        nop
        sll     $2,$2,2
        addu    $2,$3,$2
        lw      $3,0($2)
        lw      $2,28($fp)
        nop
        sll     $2,$2,3
        addu    $2,$3,$2
        addiu   $3,$fp,36
        move    $6,$2
        lui     $2,%hi($LC10)
        addiu   $5,$2,%lo($LC10)
        move    $4,$3
        jal     __isoc99_sscanf
        nop

        move    $3,$2
        li      $2,1                        # 0x1
        beq     $3,$2,$L24
        nop

        lui     $2,%hi($LC9)
        addiu   $4,$2,%lo($LC9)
        jal     puts
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L24:
        lw      $2,28($fp)
        nop
        addiu   $2,$2,1
        sw      $2,28($fp)
$L22:
        lw      $2,308($fp)
        nop
        lw      $3,8($2)
        lw      $2,28($fp)
        nop
        slt     $2,$2,$3
        bne     $2,$0,$L25
        nop

        lw      $2,24($fp)
        nop
        addiu   $2,$2,1
        sw      $2,24($fp)
$L21:
        lw      $2,308($fp)
        nop
        lw      $3,4($2)
        lw      $2,24($fp)
        nop
        slt     $2,$2,$3
        bne     $2,$0,$L26
        nop

        lw      $4,32($fp)
        jal     fclose
        nop

        beq     $2,$0,$L27
        nop

        jal     __errno_location
        nop

        lw      $2,0($2)
        nop
        move    $4,$2
        jal     strerror
        nop

        move    $5,$2
        lui     $2,%hi($LC11)
        addiu   $4,$2,%lo($LC11)
        jal     printf
        nop

$L27:
        move    $2,$0
        move    $sp,$fp
        lw      $31,300($sp)
        lw      $fp,296($sp)
        addiu   $sp,$sp,304
        j       $31
        nop

$LC12:
        .ascii  "mm_matrix_mult: dimension mismatch between matrices.\000"
mm_matrix_mult:
        addiu   $sp,$sp,-48
        sw      $31,44($sp)
        sw      $fp,40($sp)
        move    $fp,$sp
        sw      $4,48($fp)
        sw      $5,52($fp)
        lw      $2,48($fp)
        nop
        lw      $3,8($2)
        lw      $2,52($fp)
        nop
        lw      $2,4($2)
        nop
        beq     $3,$2,$L30
        nop

        lui     $2,%hi($LC12)
        addiu   $4,$2,%lo($LC12)
        jal     puts
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L30:
        li      $4,12                 # 0xc
        jal     malloc
        nop

        sw      $2,36($fp)
        lw      $2,48($fp)
        nop
        lw      $3,4($2)
        lw      $2,36($fp)
        nop
        sw      $3,4($2)
        lw      $2,52($fp)
        nop
        lw      $3,8($2)
        lw      $2,36($fp)
        nop
        sw      $3,8($2)
        lw      $4,36($fp)
        jal     mm_alloc
        nop

        sw      $0,24($fp)
        b       $L31
        nop

$L36:
        sw      $0,28($fp)
        b       $L32
        nop

$L35:
        sw      $0,32($fp)
        b       $L33
        nop

$L34:
        lw      $2,36($fp)
        nop
        lw      $3,0($2)
        lw      $2,24($fp)
        nop
        sll     $2,$2,2
        addu    $2,$3,$2
        lw      $3,0($2)
        lw      $2,28($fp)
        nop
        sll     $2,$2,3
        addu    $2,$3,$2
        lw      $3,36($fp)
        nop
        lw      $4,0($3)
        lw      $3,24($fp)
        nop
        sll     $3,$3,2
        addu    $3,$4,$3
        lw      $4,0($3)
        lw      $3,28($fp)
        nop
        sll     $3,$3,3
        addu    $3,$4,$3
        lwc1    $f2,4($3)
        nop
        lwc1    $f3,0($3)
        lw      $3,48($fp)
        nop
        lw      $4,0($3)
        lw      $3,24($fp)
        nop
        sll     $3,$3,2
        addu    $3,$4,$3
        lw      $4,0($3)
        lw      $3,32($fp)
        nop
        sll     $3,$3,3
        addu    $3,$4,$3
        lwc1    $f4,4($3)
        nop
        lwc1    $f5,0($3)
        lw      $3,52($fp)
        nop
        lw      $4,0($3)
        lw      $3,32($fp)
        nop
        sll     $3,$3,2
        addu    $3,$4,$3
        lw      $4,0($3)
        lw      $3,28($fp)
        nop
        sll     $3,$3,3
        addu    $3,$4,$3
        lwc1    $f0,4($3)
        nop
        lwc1    $f1,0($3)
        nop
        mul.d   $f0,$f4,$f0
        add.d   $f0,$f2,$f0
        swc1    $f0,4($2)
        swc1    $f1,0($2)
        lw      $2,32($fp)
        nop
        addiu   $2,$2,1
        sw      $2,32($fp)
$L33:
        lw      $2,48($fp)
        nop
        lw      $3,8($2)
        lw      $2,32($fp)
        nop
        slt     $2,$2,$3
        bne     $2,$0,$L34
        nop

        lw      $2,28($fp)
        nop
        addiu   $2,$2,1
        sw      $2,28($fp)
$L32:
        lw      $2,52($fp)
        nop
        lw      $3,8($2)
        lw      $2,28($fp)
        nop
        slt     $2,$2,$3
        bne     $2,$0,$L35
        nop

        lw      $2,24($fp)
        nop
        addiu   $2,$2,1
        sw      $2,24($fp)
$L31:
        lw      $2,48($fp)
        nop
        lw      $3,4($2)
        lw      $2,24($fp)
        nop
        slt     $2,$2,$3
        bne     $2,$0,$L36
        nop

        lw      $2,36($fp)
        move    $sp,$fp
        lw      $31,44($sp)
        lw      $fp,40($sp)
        addiu   $sp,$sp,48
        j       $31
        nop

$LC13:
        .ascii  "Please enter the matrix A data file name: \000"
$LC14:
        .ascii  "%s\000"
$LC15:
        .ascii  "main: scanf failed to read filename\000"
$LC16:
        .ascii  "Please enter the matrix B data file name: \000"
main:
        addiu   $sp,$sp,-560
        sw      $31,556($sp)
        sw      $fp,552($sp)
        move    $fp,$sp
        li      $4,12                 # 0xc
        jal     malloc
        nop

        sw      $2,24($fp)
        li      $4,12                 # 0xc
        jal     malloc
        nop

        sw      $2,28($fp)
        sw      $0,32($fp)
        lui     $2,%hi($LC13)
        addiu   $4,$2,%lo($LC13)
        jal     printf
        nop

        addiu   $2,$fp,36
        move    $5,$2
        lui     $2,%hi($LC14)
        addiu   $4,$2,%lo($LC14)
        jal     __isoc99_scanf
        nop

        move    $3,$2
        li      $2,1                        # 0x1
        beq     $3,$2,$L39
        nop

        lui     $2,%hi($LC15)
        addiu   $4,$2,%lo($LC15)
        jal     puts
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L39:
        addiu   $2,$fp,36
        lw      $5,24($fp)
        move    $4,$2
        jal     mm_read
        nop

        lw      $4,24($fp)
        jal     mm_print
        nop

        lui     $2,%hi($LC16)
        addiu   $4,$2,%lo($LC16)
        jal     printf
        nop

        addiu   $2,$fp,292
        move    $5,$2
        lui     $2,%hi($LC14)
        addiu   $4,$2,%lo($LC14)
        jal     __isoc99_scanf
        nop

        move    $3,$2
        li      $2,1                        # 0x1
        beq     $3,$2,$L40
        nop

        lui     $2,%hi($LC15)
        addiu   $4,$2,%lo($LC15)
        jal     puts
        nop

        li      $4,-1                 # 0xffffffffffffffff
        jal     exit
        nop

$L40:
        addiu   $2,$fp,292
        lw      $5,28($fp)
        move    $4,$2
        jal     mm_read
        nop

        lw      $4,28($fp)
        jal     mm_print
        nop

        lw      $5,28($fp)
        lw      $4,24($fp)
        jal     mm_matrix_mult
        nop

        sw      $2,32($fp)
        lw      $4,32($fp)
        jal     mm_print
        nop

        lw      $4,24($fp)
        jal     mm_free
        nop

        lw      $4,28($fp)
        jal     mm_free
        nop

        lw      $4,32($fp)
        jal     mm_free
        nop

        lw      $4,24($fp)
        jal     free
        nop

        lw      $4,28($fp)
        jal     free
        nop

        lw      $4,32($fp)
        jal     free
        nop

        move    $2,$0
        move    $sp,$fp
        lw      $31,556($sp)
        lw      $fp,552($sp)
        addiu   $sp,$sp,560
        j       $31
        nop