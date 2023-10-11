#ifndef MATRIZ_H
#define MATRIZ_H
#include <stdio.h>
#include <stdlib.h>


struct Matrix {
    int **data;     // Puntero a los datos de la matriz
    int rows;       // Número de filas
    int columns;    // Número de columnas
    int size;       // Número total de elementos
};

void initMatrix(struct Matrix *matrix);

struct Matrix createMatrix(int rows, int columns) {
    struct Matrix matrix;
    matrix.rows = rows;
    matrix.columns = columns;
    matrix.size = 0;

    // Asignar memoria para los datos de la matriz
    matrix.data = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix.data[i] = (int *)malloc(columns * sizeof(int));
    }
    initMatrix(&matrix);
    return matrix;
}

// Initialize matrix with random values

void initMatrix(struct Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->columns; j++) {
            matrix->data[i][j] = 0;
        }
    }
}

// Funcion para imprimir una matriz

void printMatrix(struct Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->columns; j++) {
            printf("%d ", matrix->data[i][j]);
        }
        printf("\n");
    }
}

// funcion para que la matriz tenga mas filas
void addRows(struct Matrix *matrix, int rows) {
    matrix->data = (int **)realloc(matrix->data, (matrix->rows + rows) * sizeof(int *));
    for (int i = matrix->rows; i < matrix->rows + rows; i++) {
        matrix->data[i] = (int *)malloc(matrix->columns * sizeof(int));
    }
    matrix->rows += rows;
} 

// Funcion agregar elementos a la matriz
void addElement(struct Matrix *matrix, int row, int column, int value) {
    matrix->data[row][column] = value;
    matrix->size++;
}

void appendElementTwo(struct Matrix *matrix, int value, int value2) {
    if(matrix->size == matrix->rows) {
        addRows(matrix, matrix->rows*2);
    }
    matrix->data[matrix->size][0] = value;
    matrix->data[matrix->size][1] = value2;
    matrix->size++;
}

//Funccion que devuelve un ptr de la matriz de forma random

int getPTRRamdon(struct Matrix *matrix){
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        return -1; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    int posRandom = rand()%matrix->size;
    return matrix->data[posRandom][0];
}

//Funccion que devuelve un proceso aletorio

int getprocessRamdon(struct Matrix *matrix){
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        return 0; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    int posRandom = rand()%matrix->size;
    return matrix->data[posRandom][1];
}

// Funcion para eliminar ptr de la matriz

void deleteElement(struct Matrix *matrix,int ptr) {
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        return; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    for(int i = 0; i < matrix->size; i++){
        if(i == matrix->size-1){
            matrix->data[i][0] = 0;
            matrix->data[i][1] = 0;
            break;
        }
        if(matrix->data[i][0] >= ptr){
            matrix->data[i][0] = matrix->data[i+1][0];
            matrix->data[i][1] = matrix->data[i+1][1];
        }
        
    }
    matrix->size--;
}

// Funcion para elimina procesos de la matriz

struct Matrix deleteElementProcess(struct Matrix *matrix, int process){
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        struct Matrix matrizVacia = {NULL, 0, 0, 0};
        return matrizVacia; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    
    struct Matrix matrixAux = createMatrix(matrix->size, 2);
    for(int i = 0; i < matrix->size; i++){
        if(matrix->data[i][1] != process){
            appendElementTwo(&matrixAux, matrix->data[i][0], matrix->data[i][1]);
        }
    }
    return matrixAux;
}

// Función para liberar la memoria de una matriz
void freeMatrix(struct Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
}
#endif