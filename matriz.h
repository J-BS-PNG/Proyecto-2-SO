#ifndef MATRIZ_H
#define MATRIZ_H
#include <stdio.h>
#include <stdlib.h>
#include "sram.h"
#include "svirtual.h"
#include "listA.h"

struct Matrix {
    int **data;     // Puntero a los datos de la matriz
    int rows;       // Número de filas
    int columns;    // Número de columnas
    int size;       // Número total de elementos
    int amount;   // Contador de page o ptr
};

void initMatrix(struct Matrix *matrix);

struct Matrix createMatrix(int rows, int columns) {
    struct Matrix matrix;
    matrix.rows = rows;
    matrix.columns = columns;
    matrix.size = 0;
    matrix.amount = 1;

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

// Funcion agregar elementos a la matriz que sea de estructura de punteros, procesos, tamaño
void appendElementPuntero(struct Matrix *matrix, int proceso, int tamanno) {
    if(matrix->size == matrix->rows) {
        addRows(matrix, matrix->rows*2);
    }
    matrix->data[matrix->size][0] = matrix->amount;
    matrix->data[matrix->size][1] = proceso;
    matrix->data[matrix->size][2] = tamanno;
    matrix->size++;
    matrix->amount++;
}

void appendElementPunteroM(struct Matrix *matrix,int pos ,int ptr, int proceso, int tamanno) {
    matrix->data[pos][0] = ptr;
    matrix->data[pos][1] = proceso;
    matrix->data[pos][2] = tamanno;
}


void appendElementPagina(struct Matrix *matrix, int ptr, int ram,int dicRam , int timeStamp, int bit) {
    if(matrix->size == matrix->rows) {
        addRows(matrix, matrix->rows*2);
    }
    matrix->data[matrix->size][0] = matrix->amount;
    matrix->data[matrix->size][1] = ptr;
    matrix->data[matrix->size][2] = ram;
    matrix->data[matrix->size][3] = dicRam;
    matrix->data[matrix->size][4] = timeStamp;
    matrix->data[matrix->size][5] = bit;
    matrix->size++;
    matrix->amount++;
}

void appendElementPaginaM(struct Matrix *matrix,int pos, int idPage, int ptr, int ram,int dicRam , int timeStamp, int bit) {
    matrix->data[pos][0] = idPage;
    matrix->data[pos][1] = ptr;
    matrix->data[pos][2] = ram;
    matrix->data[pos][3] = dicRam;
    matrix->data[pos][4] = timeStamp;
    matrix->data[pos][5] = bit;
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

// obtener proceso de la matriz tablapunteros
int getProcessElementPtr(struct Matrix *matrix, int ptr){
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        return 0; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    for(int i = 0; i < matrix->size; i++){
        if(matrix->data[i][0] == ptr){
            return matrix->data[i][1];
        }
    }
    return -1;
}

//obtine el ptr de la matriz
int getPtrElement(struct Matrix *matrix, int pos){
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        return -1; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    return matrix->data[pos][0];
}

// obtiene las paginas que se pueden usar para cambiar
void getPageElementOPT(struct Matrix *matrix, struct Lista *listaPaginas, struct Lista *opcCambiar, int ptr){
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        return; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    int empieza = 0;
    for(int i = 0; i < matrix->size; i++){
        if(matrix->data[i][2] == 1 && matrix->data[i][1] == ptr ){
            if(empieza != 1) empieza = 1;
            int pos = obtenerIndice(listaPaginas, matrix->data[i][0]);
            if(pos != -1){
                eliminarElemento(listaPaginas, pos);
                agregarElemento(opcCambiar, matrix->data[i][0]);
            }
        }else{
            if(empieza == 1) break;
        }
    }
}

//
void replacePageOPT(struct Matrix *matrix, struct RAM *ram, struct Virtual *virtual, int paginaAVirtual, int paginaARam, int procesoRAM){
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        return; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    int terminar = 0;
    int posRAM = 0;
    int posVirtual = 0;
    for(int i = 0; i < matrix->size; i++){
        if(matrix->data[i][0] == paginaAVirtual){
            posVirtual = obtenerIndiceVirtual(virtual, paginaARam);
            matrix->data[i][2] = 0;
            matrix->data[i][3] = posVirtual;
            matrix->data[i][4] = 0;
            terminar++;
        }
        if(matrix->data[i][0] == paginaARam){
            posRAM = obtenerIndiceRAM(ram, paginaAVirtual);
            matrix->data[i][2] = 1;
            matrix->data[i][3] = posRAM;
            terminar++;
        }
        if(terminar == 2) break;
    }
    cambiarElementoRAM(ram, posRAM, procesoRAM, paginaARam);\
    cambiarElementoVirtual(virtual, posVirtual, paginaAVirtual);
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

// Funcion para eliminar ptr de la matriz de punteros
void deleteElementPtr(struct Matrix *matrix,int ptr) {
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        return; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    for(int i = 0; i < matrix->size; i++){
        if(i == matrix->size-1){
            matrix->data[i][0] = 0;
            matrix->data[i][1] = 0;
            matrix->data[i][2] = 0;
            break;
        }
        if(matrix->data[i][0] >= ptr){
            matrix->data[i][0] = matrix->data[i+1][0];
            matrix->data[i][1] = matrix->data[i+1][1];
            matrix->data[i][2] = matrix->data[i+1][2];
        }
        
    }
    matrix->size--;
}

// Funcion para eliminar proceso y devuelve puntero
struct Matrix deleteElementProcessPtr(struct Matrix *matrix, struct Lista *listaPtr, int process){
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        struct Matrix matrizVacia = {NULL, 0, 0, 0};
        return matrizVacia; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    
    struct Matrix matrixAux = createMatrix(matrix->rows, matrix->columns);
    matrixAux.amount = matrix->amount;
    matrixAux.size = matrix->size;
    int position = 0;
    for(int i = 0; i < matrix->size; i++){
        if(matrix->data[i][1] != process){
            appendElementPunteroM(&matrixAux,position , matrix->data[i][0], matrix->data[i][1], matrix->data[i][2]);
            position++;
        }else{
            agregarElemento(listaPtr, matrix->data[i][0]);
        }
    }
    matrixAux.size = position;
    // freeMatrix(matrix);
    return matrixAux;
}

// Funcion para eliminar page con el punetro que coincida
struct Matrix deleteElementPage(struct Matrix *matrix, struct RAM *ram, struct Virtual *virtual, int ptr, int MRU){
    if (matrix == NULL || matrix->size == 0) {
        // Manejo de error: matriz nula o tamaño igual a cero
        printf("Error: valor de size:%d\n", matrix->size);
        struct Matrix matrizVacia = {NULL, 0, 0, 0};
        return matrizVacia; // O cualquier otro valor que tenga sentido en tu aplicación
    }
    
    struct Matrix matrixAux = createMatrix(matrix->rows, matrix->columns);
    matrixAux.amount = matrix->amount;
    matrixAux.size = matrix->size;
    int position = 0;
    int timeMRU = 0;
    int posMRU = 0;
    for(int i = 0; i < matrix->size; i++){
        if(matrix->data[i][1] != ptr){
            if(MRU == 3){
                if(matrix->data[i][2] == 1 && matrix->data[i][4] > timeMRU){
                    timeMRU = matrix->data[i][4];
                    posMRU = i;
                }
            }
            appendElementPaginaM(&matrixAux, position, matrix->data[i][0], matrix->data[i][1], matrix->data[i][2], matrix->data[i][3], matrix->data[i][4], matrix->data[i][5]);
            position++;
        }else{
            if(matrix->data[i][2] == 1){
                eliminarElementoRAM(ram, matrix->data[i][3]);
            }else{
                eliminarElementoVirtual(virtual, matrix->data[i][3]);
            }
        }
    }
    if(MRU == 3) matrix->data[posMRU][5] = 1;
    matrixAux.size = position;
    return matrixAux;
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