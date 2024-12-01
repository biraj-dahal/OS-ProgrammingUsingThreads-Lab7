// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <time.h>

// int matA[MAX][MAX]; 
// int matB[MAX][MAX]; 

// int matSumResult[MAX][MAX];
// int matDiffResult[MAX][MAX]; 
// int matProductResult[MAX][MAX]; 

// int MAX;

// void fillMatrix(int matrix[MAX][MAX]) {
//     for(int i = 0; i<MAX; i++) {
//         for(int j = 0; j<MAX; j++) {
//             matrix[i][j] = rand()%10+1;
//         }
//     }
// }

// void printMatrix(int matrix[MAX][MAX]) {
//     for(int i = 0; i<MAX; i++) {
//         for(int j = 0; j<MAX; j++) {
//             printf("%5d", matrix[i][j]);
//         }
//         printf("\n");
//     }
//     printf("\n");
// }

// // Fetches the appropriate coordinates from the argument, and sets
// // the cell of matSumResult at the coordinates to the sum of the
// // values at the coordinates of matA and matB.
// void* computeSum(void* args) { // pass in the number of the ith thread
//     return NULL;
// }

// // Fetches the appropriate coordinates from the argument, and sets
// // the cell of matSumResult at the coordinates to the difference of the
// // values at the coordinates of matA and matB.
// void* computeDiff(void* args) { // pass in the number of the ith thread
//     return NULL;
// }

// // Fetches the appropriate coordinates from the argument, and sets
// // the cell of matSumResult at the coordinates to the inner product
// // of matA and matB.
// void* computeProduct(void* args) { // pass in the number of the ith thread
//     return NULL;
// }

// // Spawn a thread to fill each cell in each result matrix.
// // How many threads will you spawn?
// int main() {
//     srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
//     // 0. Get the matrix size from the command line and assign it to MAX
    
//     // 1. Fill the matrices (matA and matB) with random values.
    
//     // 2. Print the initial matrices.
//     printf("Matrix A:\n");
//     printMatrix(matA);
//     printf("Matrix B:\n");
//     printMatrix(matB);
    
//     // 3. Create pthread_t objects for our threads.
    
//     // 4. Create a thread for each cell of each matrix operation.
//     // 
//     // You'll need to pass in the coordinates of the cell you want the thread
//     // to compute.
//     // 
//     // One way to do this is to malloc memory for the thread number i, populate the coordinates
//     // into that space, and pass that address to the thread. The thread will use that number to calcuate 
//     // its portion of the matrix. The thread will then have to free that space when it's done with what's in that memory.
    
//     // 5. Wait for all threads to finish.
    
//     // 6. Print the results.
//     printf("Results:\n");
//     printf("Sum:\n");
//     printMatrix(matSumResult);
//     printf("Difference:\n");
//     printMatrix(matDiffResult);
//     printf("Product:\n");
//     printMatrix(matProductResult);
//     return 0;
  
// }






#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20
#define NUM_THREADS 10

int matA[MAX][MAX];
int matB[MAX][MAX];
int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

typedef struct {
    int startRow;
    int endRow;
} ThreadData;

void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        for(int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* computeSum(void* args) {
    ThreadData* data = (ThreadData*) args;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    pthread_exit(0);
}

void* computeDiff(void* args) {
    ThreadData* data = (ThreadData*) args;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    pthread_exit(0);
}

void* computeProduct(void* args) {
    ThreadData* data = (ThreadData*) args;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matProductResult[i][j] = 0;
            for (int k = 0; k < MAX; k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    pthread_exit(0);
}

int main() {
    srand(time(0));

    fillMatrix(matA);
    fillMatrix(matB);

    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    int rowsPerThread = MAX / NUM_THREADS;

    // Spawn threads for addition
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].startRow = i * rowsPerThread;
        threadData[i].endRow = (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, computeSum, (void*)&threadData[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Spawn threads for subtraction
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, computeDiff, (void*)&threadData[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Spawn threads for product
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, computeProduct, (void*)&threadData[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print results
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}
