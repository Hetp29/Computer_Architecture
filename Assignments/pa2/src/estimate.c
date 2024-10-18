#include <stdio.h>
#include <stdlib.h>

//first we need to read the matrix from the data text files (training data)
//need triple pointer for X matrix because we allocating a 2D array (pointer to a pointer to  double) and we're passing to function, we need extra third pinter inside function which points to matrix pointer
//Y is prices of houses but has double pointer because it is 1D array and inside function
//if you want to allocate memory inside a function, you need to add extra pointer because the function needs to modify original pointer outside the function in order to allocate space
void read_matrix(char *filename, double ***X, double **Y, int *n, int *k) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d %d", n, k); //number or rows and number of features
    //memory allocation for rows (array of pointers) and then each row
    *X = (double**)malloc((*n) * sizeof(double *));
    for(int i = 0; i < *n; i++) {
        (*X)[i] = (double *)malloc((*k + 1) * sizeof(double)); //X matrix memory allocation
        (*X)[i][0] = 1;
    }

    *Y = (double *)malloc((*n) * sizeof(double));

    for(int i = 0; i < *n; i++) {
        for(int j = 1; j <= *k; j++) {
            fscanf(file, "%lf", &(*X)[i][j]);
        }
        fscanf(file, "%lf", &(*Y)[i]);
    }
    fclose(file);

}

//we need function to transpose the matrix 
void transpose(double **matrix, double **result, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j]; 
        }
    }
}

//matrix multiplication 
void multiply(double **A, double **B, double **result, int aRows, int bCols, int aCols) {
    for(int i = 0; i < aRows; i++) {
        for(int j = 0; j < bCols; j++) {
            result[i][j] = 0;
            for(int k = 0; k < aCols; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

//gauss-jordan elimination to invert n x n matrix
void invert(double **matrix, int n) {
    double **I = (double **)malloc(n * sizeof(double *));
    for(int i = 0; i < n; i++) {
        I[i] = (double *)malloc(n * sizeof(double ));
        for(int j = 0; j < n; j++) {
            if(i == j) {
                I[i][j] = 1;
            }
            else {
                I[i][j] = 0;
            }
        }
    }

    for(int p = 0; p < n; p++) {
        double f = matrix[p][p];
        for(int j = 0; j < n; j++) {
            matrix[p][j] /= f;
            I[p][j] /= f;
        }

        for(int i = 0; i < n; i++) {
            if (i != p) {
                f = matrix[i][p];
                for(int j = 0; j < n; j++) {
                    matrix[i][j] -= matrix[p][j] * f;
                    I[i][j] -= I[p][j] * f;
                }
            }
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            matrix[i][j] = I[i][j];
        }
    }
    for(int i = 0; i < n; i++) {
        free(I[i]);
    }
    free(I);
}