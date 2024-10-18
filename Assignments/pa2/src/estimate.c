#include <stdio.h>
#include <stdlib.h>

//need triple pointer for X matrix because we allocating a 2D array (pointer to a pointer to  double) and we're passing to function, we need extra third pinter inside function which points to matrix pointer
//Y is prices of houses but has double pointer because it is 1D array and inside function
//if you want to allocate memory inside a function, you need to add extra pointer because the function needs to modify original pointer outside the function in order to allocate space
void read(char filename, double ***X, double **Y, int *n, int *k, int training) {
    //training is 1 if it's training data and 0 if it's test data
    FILE *file = fopen(filename, "r");
    if(file == NULL) exit(EXIT_FAILURE);

    char type[50];
    fscanf(file, "%s", type); //reads train or data
    fscanf(file, "%d %d", k, n); //number of attributes and number of houses

    *X = (double **)malloc((*n) * sizeof(double *));
    for(int i = 0; i < *n; i++) {
        (*X)[i] = (double *)malloc((*k + 1) * sizeof(double)); //extra 1 in x due to bias term, first column with 1 is for bias term
        (*X)[i][0] = 1;
    } 

    if(training) {
        *Y = (double *)malloc((*n) * sizeof(double)); //Y for training data (house prices)
    }
    for(int i = 0; i < *n; i++) {
        for(int j = 1; j <= *k; j++) {
            fscanf(file, "%lf", &(*X)[i][j]); //X matrix attributes
        }
        if(training) fscanf(file, "%lf", &(*Y)[i]); //Y (house prices)
            
        
    }
    fclose(file);
}

//transpose matrix
void transpose(double **matrix, double **result, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
}

//multiply matrix
void multiply(double **A, double **B, double **result, int aRows, int bCols, int aCols) {
    for(int i = 0; i < aRows; i++) {
        for(int j = 0; j < bCols; j++) {
            result[i][j] = 0;
            for(int k = 0; k < aCols; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
            //result stores matrix of aRows x bCols and iterates throw rows in matrix A and columns of matrix B
        }
    }
}

//multiply matrix A and vector B to get result vector 
void multiplyVector(double **A, double *B, double **result, int aRows, int aCols) {
    for(int i = 0; i < aRows; i++) {
        result[i][0] = 0;
        for(int j = 0; j < aCols; j++) {
            result[i][0] += A[i][j] * B[j];
        }
    }
}

//invert matrix (Gauss-Jordan Elimination)
void invert(double **matrix, int n) {
    double **I = (double **)malloc(n * sizeof(double *)); //Identity matrix which will be turned into inverse matrix after row operations
    for(int i = 0; i < n; i++) {
        I[i] = (double *)malloc(n * sizeof(double));
        for(int j = 0; j < n; j++) {
            if(i == j) {
                I[i][j] = 1; //diagonal elements are 1 for identity matrix
            }
            else {
                I[i][j] = 0; //non-diagonal elements are 0 for identity matrix
            }
        }
    }
    //row operations to turn matrix into identity matrix and I into inverse matrix
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

//calculates weights W using formula W = (X^T X)^-1 X^T Y
void calculateWeights(double **X, double *Y, int n, int k, double **W) {
    double **xTranspose = (double **)malloc((k + 1) * sizeof(double *));
    for(int i = 0; i <= k; i++) {
        xTranspose[i] = (double *)malloc(n * sizeof(double));
    }
    transpose(X, xTranspose, n, k + 1);

    double **xTransposeX = (double **)malloc((k + 1) * sizeof(double *));
    for(int i = 0; i <= k; i++) {
        xTransposeX[i] = (double *)malloc((k + 1) * sizeof(double));
    }
    multiply(xTranspose, X, xTransposeX, k + 1, k + 1, n);

    invert(xTransposeX, k + 1);

    double **xTransposeY = (double **)malloc((k + 1) * sizeof(double *));
    for(int i = 0; i <= k; i++) {
        xTransposeY[i] = (double *)malloc(1 *sizeof(double));
    }
    multiplyVector(xTranspose, Y, xTransposeY, k + 1, n);

    multiply(xTransposeX, xTransposeY, W, k + 1, 1, k + 1);

    for(int i = 0; i <= k; i++) {
        free(xTranspose[i]);
        free(xTransposeX[i]);
        free(xTransposeY[i]);
    }
    free(xTranspose);
    free(xTransposeX);
    free(xTransposeY);
}

//predict house prices for test data using weights
void predict(double **X, double **W, int n, int k) {
    double **predictions = (double **)malloc(n * sizeof(double *));
    for(int i = 0; i < n; i++) {
        predictions[i] = (double *)malloc(1 * sizeof(double));
    }
    multiplyVector(X, *W, predictions, n, k + 1); //multiply matrix X with W weights to get predicted prices of houses
    for(int i = 0; i < n; i++) {
        printf("%.0f\n", predictions[i][0]);
        free(predictions[i]);
    }
    free(predictions);
}