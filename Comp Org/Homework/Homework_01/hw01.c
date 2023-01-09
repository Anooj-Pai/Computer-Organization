/*
Assignment 1: NxM Matrix Multiplication
CSCI-2500 Fall 2021
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/******************************************************************************/
/* Function Calls */
/******************************************************************************/

typedef struct
{
  double **data;
  int rows;
  int cols;
} matrix;

int mm_alloc(matrix *mat);
int mm_free(matrix *mat);
int mm_print(matrix *mat);
int mm_read(char *filename, matrix *mat);
matrix *mm_matrix_mult(matrix *mat1, matrix *mat2);

int mm_alloc(matrix *mat)
{
  /*
  Notes:
    Allocate an array of pointers to a double
    Then, allocate an row/array of doubles and assign each double pointer that row
    Make sure you're allocating using mat->data in the matrix struct
    You can use calloc for allocation and zeroing out all the bytes
    - otherwise, you'd want to manually set all arrays to 0 (good practice)
    If your malloc/calloc returns null, the allocation failed
  */

  //initializes the first layer of the array
  mat->data = (double**)calloc(mat->rows, sizeof(double *));

  //checks for allocation error on first layer
  if(mat->data == NULL){
    printf("mm_alloc: allocation failed.");
    exit(0);
  }

  //checks to see if the rows and cols were read in properly
  //if no then it gives an allocation error
  if(mat->rows > 0 || mat->cols > 0){
    for (int i = 0; i < mat->rows; i++) {
      mat->data[i] = (double*)malloc(mat->cols * sizeof(double));
      if(mat->data[i] == NULL){ //checks to see if any spots didnt allocate
        printf("mm_alloc: allocation failed.");
        exit(0);
      }
    }
  }else{
    printf("mm_alloc: allocation failed.");
    exit(0);
  }

  return 0;
}

int mm_free(matrix *mat)
{
  /*
  Notes:
    Reverse the allocation process, but you'll be calling free() instead
  */
  //runs through the array and frees each index
  for (int i = 0; i < mat->rows; i++){
    free(mat->data[i]);
  }

  return 0;
}

int mm_print(matrix *mat)
{
  /*
  Notes:
    Print all the data values in your matrix
    Go row by row
    At the end of each row, print a newline
    Between each data value, insert a tab
    Use 2 digits of precision after the decimal (use "%10.2lf\t" as format string)
    See output.txt for expected output formatting
  */


  //runs a double for loop to go through the array and print the values with the formatting given
  printf("\n/******** START of 2-D Matrix **********************************/\n");
  for (int i = 0; i < mat->rows; i++)
  {
    for (int j = 0; j < mat->cols; j++)
    {
      printf("%10.2lf\t", mat->data[i][j]);
    }
    printf("\n");
  }
  printf("/******** END of 2-D Matrix ************************************/\n");
  return 0;
}

int mm_read(char *filename, matrix *mat)
{
  /*
  Notes:
    Use fopen to open the matrix file, then use fgets or getline to read lines
    The first line of the matrix contains integers for "#rows #columns"
    - Use these to allocate your arrays for matrix data
    Each subsequent line contains a single data value
    - You can use sscanf to parse each matrix entry
    - First value is data[0][0], next is data[0][1] ... data[#rows-1][#columns-1]
  */

  //opens the file
  FILE* fp = fopen(filename, "r");
  //creates a holding var
  char test[256];

  //checks if the file was opened
  if(fp == NULL){
    printf("mm_read: failed to open file.\n");
    exit(0);
  }

  //makes sure the file isnt empty
  if(fgets(test, 265, fp) == NULL) {
    printf("mm read: failed to read from file.\n");
    exit(0);
  }

  //checks that the rows and cols are there and read in properly
  if(sscanf(test, "%d %d", &mat->rows, &mat->cols) != 2){
    printf("mm_read: failed to read matrix dimensions.\n");
    exit(0);
  }

  //allocates the matrix with the rows and cols just read in
  mm_alloc(mat);
  int count = 0; // used to make sure the amount of vals in the doc is the correct number need for the matrix

  //gets the first line so the rows and cols dont mess up the vals
  fgets(test, 256, fp);

  //double for loop to scan in the values from the file to the array
  for (int i = 0; i < mat->rows; ++i) {
    for (int j = 0; j < mat->cols; ++j) {
      //scans the line into the matrix spot
      sscanf(test, "%lf", &mat->data[i][j]);
      if(mat->data[i][j] == 0){
        printf("mm read: failed to read matrix values.\n");
        exit(0);
      }
      fgets(test, 256, fp);// next line
      count++;
      }
    }

    //checks that the number of values that were read is the number needed
    if (count != mat->rows*mat->cols){
        printf("mm read: failed to read matrix values.\n");
        exit(0);
    }

  fclose(fp);

  return 0;
}

matrix *mm_matrix_mult(matrix *mat1, matrix *mat2)
{
  /*
  Notes:
    First check that it is actually possible to multiply the two inputs
    If so, construct a result matrix
    Then perform the multiplication
    Then return a pointer to your result matrix
  */

  //makes sure the values of the rows and cols are right to allow the multiplication
  if (mat1->cols != mat2->rows)
  {
    printf("mm_matrix_mult: dimension mismatch between matrices.");
    exit(0);
  }
  else
  {
    matrix* result_matrix = (matrix *)malloc(sizeof(matrix)); // creates the new matrix
    result_matrix->rows = mat1->rows; // assigns the new row dimention
    result_matrix->cols = mat2->cols; // assigns the new col dimention

    mm_alloc(result_matrix); //allocates the array based on the rows and cols

    //double for loop runs through the array for each spot
    for (int i = 0; i < mat1->rows; i++)
    {
      for (int j = 0; j < mat2->cols; j++)
      {
        //this for loop does the math for the multipliction
        for (int k = 0; k < mat2->rows; k++)
        {
          result_matrix->data[i][j] += mat1->data[i][k] * mat2->data[k][j];
        }
      }
    }
    return result_matrix;
  }
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