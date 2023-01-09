#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "matrix.h"
#include "mult.h"
#include "print.h"
#include "read.h"

int mm_free(matrix* mat)
{
/*
Notes:
  Reverse the allocation process, but you'll be calling free() instead
*/
    for (int i = 0; i < mat->rows; ++i) {
        free(mat->data[i]);
    }
    free(mat->data);
    /* note: free does not return a value */

    return 0;
}

int main()
{
    /*
     * you must keep this function AS IS.
     * you cannot modify it!
     */
    char filename_A[256];
    char filename_B[256];
    matrix* A = (matrix*)malloc(sizeof(matrix));
    matrix* B = (matrix*)malloc(sizeof(matrix));
    matrix* C = NULL;

    printf("Please enter the matrix A data file name: ");
    if (1 != scanf("%s", filename_A)) {
        printf("main: scanf failed to read filename\n");
        exit(-1);
    }
    mm_read(filename_A, A);
    mm_print(A);

    printf("Please enter the matrix B data file name: ");
    if (1 != scanf("%s", filename_B)) {
        printf("main: scanf failed to read filename\n");
        exit(-1);
    }
    mm_read(filename_B, B);
    mm_print(B);

    C = mm_matrix_mult(A, B);
    mm_print(C);

    mm_free(A);
    mm_free(B);
    mm_free(C);
    free(A);
    free(B);
    free(C);

    return 0;
}