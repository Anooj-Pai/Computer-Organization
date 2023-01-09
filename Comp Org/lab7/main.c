/* lab07.c */

#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

/* define BIT type as a char (i.e., one byte) */
typedef char BIT;

/* function prototypes (basic gates) */
BIT not_gate(BIT);
BIT or_gate(BIT, BIT);
BIT and_gate(BIT, BIT);
BIT xor_gate(BIT, BIT);

/* function prototypes (combinational circuits) */
BIT multiplexer(BIT, BIT, BIT, BIT, BIT, BIT);
void decoder(BIT, BIT, BIT *, BIT *, BIT *, BIT *);

/* main() provides some unit testing */
int main()
{
    BIT A, B;
    BIT I0, I1, I2, I3;
    BIT *O0, *O1, *O2, *O3;
    BIT S0, S1;
    BIT OP0, OP1, CIN, COUT;
    BIT A0, A1, A2, A3, B0, B1, B2, B3, C0, C1, C2, C3, E0, E1, E2, E3;

    /* Unit test for NOT gate:
     *      _
     *  A | A
     * ---|---
     *  0 | 1
     *  1 | 0
     */
    printf("===== Unit test for not_gate =====\n");
    printf("not_gate( A ) | RESULT\n");
    printf("--------------|--------\n");
    printf("not_gate( 0 ) | %d\n", not_gate(0));
    printf("not_gate( 1 ) | %d\n", not_gate(1));

    /* Unit test for OR gate:
     *
     *  A  B | A OR B
     * ------|--------
     *  0  0 |   0
     *  0  1 |   1
     *  1  0 |   1
     *  1  1 |   1
     */
    printf("\n===== Unit test for or_gate =====\n");
    printf("or_gate( A, B ) | RESULT\n");
    printf("----------------|--------\n");

    for (A = 0; A < 2; A++)
        for (B = 0; B < 2; B++)
            printf("or_gate( %d, %d ) | %d\n", A, B, or_gate(A, B));

    /* Unit test for AND gate:
     *
     *  A  B | A AND B
     * ------|--------
     *  0  0 |    0
     *  0  1 |    0
     *  1  0 |    0
     *  1  1 |    1
     */
    printf("\n===== Unit test for and_gate =====\n");
    printf("and_gate( A, B ) | RESULT\n");
    printf("-----------------|--------\n");

    for (A = 0; A < 2; A++)
        for (B = 0; B < 2; B++)
            printf("and_gate( %d, %d ) | %d\n", A, B, and_gate(A, B));

    /* Unit test for XOR gate:
     *
     *  A  B | A XOR B
     * ------|--------
     *  0  0 |    0
     *  0  1 |    1
     *  1  0 |    1
     *  1  1 |    0
     */
    printf("\n===== Unit test for xor_gate =====\n");
    printf("xor_gate( A, B ) | RESULT\n");
    printf("-----------------|--------\n");

    for (A = 0; A < 2; A++)
        for (B = 0; B < 2; B++)
            printf("xor_gate( %d, %d ) | %d\n", A, B, xor_gate(A, B));

    /* Unit test for 4-input multiplexer */
    printf("\n===== Unit test for 4-input multiplexer =====\n");
    printf("multiplexer( S0, S1 ) | ( I0, I1, I2, I3 ) | RESULT ( I0 | I1 | I2 | I3 )\n");
    printf("----------------------|--------------------|------------------------------\n");
    for (S0 = 0; S0 < 2; S0++)
        for (S1 = 0; S1 < 2; S1++)
            for (I0 = 0; I0 < 2; I0++)
                for (I1 = 0; I1 < 2; I1++)
                    for (I2 = 0; I2 < 2; I2++)
                        for (I3 = 0; I3 < 2; I3++)
                            printf("multiplexer( %2d, %2d ) | ( %2d, %2d, %2d, %2d ) | %2d\n",
                                   S0, S1, I0, I1, I2, I3, multiplexer(I0, I1, I2, I3, S0, S1));

    /* Unit test for 2-input decoder:
     *
     *  I0 I1 | O0 O1 O2 O3
     * -------|-------------
     *   0  0 |  1  0  0  0
     *   0  1 |  0  0  1  0
     *   1  0 |  0  1  0  0
     *   1  1 |  0  0  0  1
     */
    printf("\n===== Unit test for 2-input decoder =====\n");
    printf("decoder( I0, I1 ) | ( O0, O1, O2, O3 )\n");
    printf("------------------|--------------------\n");
    I0 = 0;
    I1 = 0;
    decoder(I0, I1, &O0, &O1, &O2, &O3);
    printf("Decoder( %2d, %2d ) | ( %2d, %2d, %2d, %2d )\n", I0, I1, O0, O1, O2, O3);
    O0 = 0;
    O1 = 0;
    O2 = 0;
    O3 = 0;
    I0 = 0;
    I1 = 1;
    decoder(I0, I1, &O0, &O1, &O2, &O3);
    printf("Decoder( %2d, %2d ) | ( %2d, %2d, %2d, %2d )\n", I0, I1, O0, O1, O2, O3);
    O0 = 0;
    O1 = 0;
    O2 = 0;
    O3 = 0;
    I0 = 1;
    I1 = 0;
    decoder(I0, I1, &O0, &O1, &O2, &O3);
    printf("Decoder( %2d, %2d ) | ( %2d, %2d, %2d, %2d )\n", I0, I1, O0, O1, O2, O3);
    O0 = 0;
    O1 = 0;
    O2 = 0;
    O3 = 0;
    I0 = 1;
    I1 = 1;
    decoder(I0, I1, &O0, &O1, &O2, &O3);
    printf("Decoder( %2d, %2d ) | ( %2d, %2d, %2d, %2d )\n", I0, I1, O0, O1, O2, O3);
    /*
     * TO DO: Implement test cases
     */

    //printf("%d test cases out of %d failed.\n", failed, total);

    return EXIT_SUCCESS;
}

BIT not_gate(BIT A)
{
    /* TO DO: implement logical NOT */
    if (A == TRUE)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

BIT or_gate(BIT A, BIT B)
{
    /* TO DO: implement logical OR */
    if (A == FALSE && B == FALSE){
        return FALSE;
    }
    
    return TRUE;
}

BIT and_gate(BIT A, BIT B)
{
    /* TO DO: implement logical AND */
    if(A == TRUE && B == TRUE){
        return TRUE;
    }

    return FALSE;
}

BIT xor_gate(BIT A, BIT B)
{
    /* TO DO: implement logical XOR */
    if(A == B){
        return FALSE;
    }

    return TRUE;
}

BIT multiplexer(BIT I0, BIT I1, BIT I2, BIT I3, BIT S0, BIT S1)
{
    /* TO DO: implement a 4-input multiplexer */
    if(S0 == 0 && and_gate(S0, S1) == TRUE){
        return I0;
    }else if(S0 == 0 && or_gate(S0, S1) == TRUE){
        return I1;
    }else if(S0 == 1 && or_gate(S0, S1) == TRUE){
        return I2;
    }else{
        return I3;
    }
}

void decoder(BIT I0, BIT I1, BIT *O0, BIT *O1, BIT *O2, BIT *O3)
{
    /* TO DO: implement a 2-input decoder */
    if(I0 == 0 && and_gate(I0, I1) == TRUE){
        *O0 = 1;
    }else if(I0 == 0 && or_gate(I0, I1) == TRUE){
        *O1 = 1;
    }else if(I0 == 1 && or_gate(I0, I1) == TRUE){
        *O2 = 1;
    }else{
        *O3 = 1;
    }
    return;
}
