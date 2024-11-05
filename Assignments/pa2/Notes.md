#include <stdio.h>
#include <stdlib.h>

//reads data from file and distinguishes between training and testing data
void read(char *filename, double ***X, double **Y, int *n, int *k, int training) {
    //training is 1 if it's training data and 0 if it's test data
    FILE *file = fopen(filename, "r");
    if(file == NULL) exit(EXIT_FAILURE);

    char type[50];
    fscanf(file, "%s", type); //reads train or data
    fscanf(file, "%d %d", k, n); //read number of attributes (k) and number of houses (n)

    *X = (double **)malloc((*n) * sizeof(double *)); //allocate memory for matrix X
    for(int i = 0; i < *n; i++) {
        (*X)[i] = (double *)malloc((*k + 1) * sizeof(double)); //extra 1 in x due to bias term, first column with 1 is for bias term
        //loop allocates memory for each house's features in matrix X
        (*X)[i][0] = 1; //first column of matrix X is set to 1 for bias term in linear regression 
    } 

    if(training) { //allocate memory for vector Y if training data
        *Y = (double *)malloc((*n) * sizeof(double)); //Y for training data (house prices)
    }
    //read matrix X and vector Y data from file 
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
    for(int i = 0; i < n; i++) {
        double predicted = 0;
        for(int j = 0; j <= k; j++) {
            predicted += X[i][j] * W[j][0];
        }
        printf("%.0f\n", predicted);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) return EXIT_FAILURE;

    double **xTrain;
    double *yTrain;
    int nTrain, kTrain;

    read(argv[1], &xTrain, &yTrain, &nTrain, &kTrain, 1);

    double **W = (double **)malloc((kTrain + 1) * sizeof(double *));
    for(int i = 0; i <= kTrain; i++) {
        W[i] = (double *)malloc(1 * sizeof(double));
    }

    calculateWeights(xTrain, yTrain, nTrain, kTrain, W);
    
    double **xTest;
    int nTest, kTest;

    read(argv[2], &xTest, NULL, &nTest, &kTest, 0);

    if(kTrain != kTest) return EXIT_FAILURE;

    predict(xTest, W, nTest, kTest);
    
    for(int i = 0; i <= kTrain; i++) {
        free(W[i]);
    }
    free(W);

    for(int i = 0; i < nTrain; i++) {
        free(xTrain[i]);
    }
    free(xTrain);
    free(yTrain);
    for(int i = 0; i < nTest; i++) {
        free(xTest[i]);
    }
    free(xTest);

    return EXIT_SUCCESS;
}