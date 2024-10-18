#include <stdio.h>
#include <stdlib.h>

void multiply(int **result, int **A, int **B, int k) {
    int **temp = (int **)malloc(k * sizeof(int *));
    for(int i = 0; i < k; i++) {
        temp[i] = (int *)malloc(k * sizeof(int));
        if(!temp[i]) {
            for(int x = 0; x < i; x++) {
                free(temp[x]);
            }
            free(temp);
        }
        for(int j = 0; j < k; j++) {
            temp[i][j] = 0;
            for(int l = 0; l < k; l++) {
                temp[i][j] += A[i][l] * B[l][j]; 
                //multiply matrices of size k x k
            }
        }
    }
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++) {
            result[i][j] = temp[i][j]; //copy temp into result
        }
        free(temp[i]);
    }
    free(temp);
}

void printMatrix(int **matrix, int k) {
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++) {
            printf("%d", matrix[i][j]);
            if(j < k - 1) {
                printf(" ");
            }
        }
        if(i < k - 1) {
            printf("\n");
        }

    }
    printf("\n");
}

void printIdentityMatrix(int k) {
    for(int i = 0; i < k; i++) {
        for(int j = 0; j < k; j++) {
            if(i == j) {
                printf("1");
            }
            else{
                printf("0");
            }
            if(j < k - 1) {
                printf(" ");
            }
        }
        if(i < k - 1) {
            printf("\n");
        }
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_SUCCESS; //program receives one argument (filename)
    } 

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        return EXIT_SUCCESS; //if fopen returns NULL, file cannot be opened 
    } 
    

    int k;
    if(fscanf(file, "%d", &k) != 1 || k <= 0 || k > 1000) {
        fclose(file);
        return EXIT_SUCCESS; //k is read and size is within range 
    }


    int **matrix = (int **)malloc(k * sizeof(int *));
    int **result = (int **)malloc(k * sizeof(int *));
    if (matrix == NULL || result == NULL) {
        fclose(file);
        free(matrix);
        free(result);
    }

    

    for (int i = 0; i < k; i++) {
        matrix[i] = (int *)malloc(k * sizeof(int));
        result[i] = (int *)malloc(k * sizeof(int));
        if (matrix[i] == NULL || result[i] == NULL) {
            for(int j = 0; j <= i; j++) {
                free(matrix[i]);
                free(result[j]);
            }
            free(matrix);
            free(result);
            fclose(file);
        }
        for(int j = 0; j < k; j++) {
            if(fscanf(file, "%d", &matrix[i][j]) != 1) {
                for(int x = 0; x <= i; x++) {
                    free(matrix[x]);
                    free(result[x]);
                }
                free(matrix);
                free(result);
                fclose(file);
            }
            result[i][j] = matrix[i][j];
        }
    }

    int n;
    if (fscanf(file, "%d", &n) != 1 || n < 0 || n > 1000) {
        for(int i = 0; i < k; i++) {
            free(matrix[i]);
            free(result[i]);
        }
        free(matrix);
        free(result);
        fclose(file);
    }
    fclose(file);

    if(n == 0) {
        printIdentityMatrix(k);
        for(int i = 0; i < k; i++) {
            free(matrix[i]);
            free(result[i]);
        }
        free(matrix);
        free(result);
        return 0;
    }

    for (int exp = 1; exp < n; exp++) {
        multiply(result, result, matrix, k);
    }

    printMatrix(result, k);


    for (int i = 0; i < k; i++) {
        free(matrix[i]);
        free(result[i]);
    }
    free(matrix);
    free(result);
    return 0;
}