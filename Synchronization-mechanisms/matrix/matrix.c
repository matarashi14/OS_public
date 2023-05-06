#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <pthread.h>

// create, fill and destroy matrices
int **allocateMatrix(int size);
void fillMatrix(int **matrix, int size);
void destroyMatrix(int **matrix, int size);
void printMatrix(int **matrix, int size);

// thread function
void *multiplyMatrix(void *arg);

// 2D arrays for each matrix
int **matrixA;
int **matrixB;
int **matrixC;

int size;

void *multiplyMatrix(void *arg)
{
    // get thread ID from arg
    int tid = (intptr_t)arg;

    // calculate row and col of thread
    int row = tid / size;
    int col = tid % size;

    // do dot product calculation
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += matrixA[row][i] * matrixB[i][col];
    }

    // save value in matrix C
    matrixC[row][col] = sum;

    // exit thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Seed the random number generator
    srand(time(NULL));

    // check that arguments make sense, print error message if they do not.
    if (argc != 2)
    {
        printf("Usage: ./matrix [size of matrix]");
        exit(1);
    }

    size = atoi(argv[1]);

    if (size == 0)
    {
        printf("Error: Argument is not an integer or invalid number\n");
        exit(1);
    }

    // allocate memory for the matrices
    matrixA = allocateMatrix(size);
    matrixB = allocateMatrix(size);
    matrixC = allocateMatrix(size);

    // fill A and B with random values

    fillMatrix(matrixA, size);
    fillMatrix(matrixB, size);

    // create enough threads to run multiplyMatrix
    int thread_num = size * size;
    long count = 0;
    pthread_t threads[thread_num];

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            pthread_create(&threads[count], NULL, multiplyMatrix, (void *)(count));
            count++;
        }
    }

    // join all threads

    for (int i = 0; i < thread_num; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // print each matrix
    printf("Matrix A:\n");
    printMatrix(matrixA, size);
    printf("Matrix B:\n");
    printMatrix(matrixB, size);
    printf("Matrix C:\n");
    printMatrix(matrixC, size);

    // destroy each matrix
    destroyMatrix(matrixA, size);
    destroyMatrix(matrixB, size);
    destroyMatrix(matrixC, size);

    return 0;
}

// allocate memory for the matrix
int **allocateMatrix(int size)
{
    int **matrix = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
    {
        matrix[i] = (int *)malloc(size * sizeof(int));
    }
    return matrix;
}

// Fill the matrix with random values
void fillMatrix(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

// Free the memory used by the matrix
void destroyMatrix(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

// Print the matrix
void printMatrix(int **matrix, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
