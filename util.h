#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "matriz.h"
#include "listA.h"

enum algoritm{
    FIFO = 1, // First In First Out 
    SC = 2,  // Second Chance
    MRU = 3, // Most Recently Used
    RDN = 4 // Random
    };

struct start
{
    int seed; // Seed for random number generator
    enum algoritm option; // Algorithm to use
    char *file; // File to read
    int NumberProcess; // Number of processes
    int amoutOperations; // Number of operations
};

struct start crearPreparación(int seed, enum algoritm option, FILE *file, int NumberProcess, int amoutOperations){
    struct start start;
    start.seed = seed;
    start.option = option;
    start.file = file;
    start.NumberProcess = NumberProcess;
    start.amoutOperations = amoutOperations;
    return start;
}

//Ejemplo de file
//      FILE *archivo;
//     archivo = fopen("p.txt", "r");

//     if(archivo == NULL){
//         printf("Error al abrir el archivo");
//         return 1;
//     }
    
//     char linea[100];
//     while(fgets(linea, sizeof(linea),archivo) != NULL){
//         printf("%s", linea);
//     }
//     fclose(archivo);

// Main Function

void initBoolList(bool list[], int size){
    for(int i = 0; i < size; i++){
        list[i] = false;
    }
}

void initIntList(int list[], int size){
    for(int i = 0; i < size; i++){
        list[i] = 0;
    }
}

int generarRadom(int sizeProcess, int numberProcess, int sizePTRMatrix, int sizeProcessKill){
    if(numberProcess <= sizeProcess/2){
        return rand()%2;
    }else if(sizePTRMatrix <= sizeProcess){
        return rand()%2;
    }else{
        if(sizeProcess/2 <= sizeProcessKill){
            return rand()%3;
        }else{
            return rand()%4;
        }
    }
}

int generarRadomV2(int sizeProcess, int sizePTRMatrix, int sizeProcessKill){
    // Genera un número aleatorio entre 0 y 1
    double random_number = (double)rand() / RAND_MAX;

    // Define las probabilidades para cada tipo de proceso
    double probabilidad_proceso_1 = 0.40;
    double probabilidad_proceso_2 = 0.50;
    double probabilidad_proceso_3 = 0.05;
    double probabilidad_proceso_4 = 0.05;

    if (random_number < probabilidad_proceso_1) {
        return 0;
    } else if (random_number < probabilidad_proceso_1 + probabilidad_proceso_2) {
        return 1;
    } else if (random_number < probabilidad_proceso_1 + probabilidad_proceso_2 + probabilidad_proceso_3) {
        if(sizePTRMatrix <= 1){
            return rand()%2;
        }else{
            return 2;
        }
    } else {
        if(sizePTRMatrix <= 1) return 2;

        if(sizeProcess/2 <= sizeProcessKill){
            return 2;
        }else{
            return 3;
        }
    }

}
 


void createSimulationTxt(int sizeProcess, int sizeOperations, struct Matrix *mat){
    FILE *archivo;
    archivo = fopen("simulation.txt", "w");

    if(archivo == NULL){
        printf("Error al abrir el archivo");
        return;
    }

    struct Lista listProcess;
    inicializarLista(&listProcess, sizeProcess);

    int sizeActualProcess = rand()%sizeProcess; // cantidad de procesos que se van a hacer al inicio
    int sizePTR = 1; // cantidad de punteros
    if(sizeActualProcess == 0){
        sizeActualProcess = sizeProcess/2;
    }

    for(int i = 0; i < sizeActualProcess; i++){
        int sizeBytes = rand()%10000;
        fprintf(archivo, "new(%d,%d)\n", i+1, sizeBytes);
        appendElementTwo(mat, sizePTR, i+1);
        agregarElemento(&listProcess, i+1);
        sizePTR++;
    }

    int ptrRandom = 0; // puntero random
    int saveProcessKill = 0; // cantidad de procesos que se van a eliminar al final
    int randProcess = 0; // proceso random
    
    for(int i = sizePTR; i < sizeOperations -  (sizeProcess/2); i++){
        int randOption = generarRadomV2(sizeProcess, mat->size, saveProcessKill);
        switch (randOption){

        case 0:
            int newOption = rand()%2;
            int sizeBytes = rand()%10000;
            if(newOption == 0 && listProcess.longitud != sizeProcess && sizeActualProcess != sizeProcess){
                fprintf(archivo, "new(%d,%d)\n", sizeActualProcess+1, sizeBytes);
                appendElementTwo(mat, sizePTR, sizeActualProcess+1);
                agregarElemento(&listProcess, sizeActualProcess+1);
                sizePTR++;
                sizeActualProcess++;
            } else {
                randProcess = obtenerElementoRandom(&listProcess);
                fprintf(archivo, "new(%d,%d)\n", randProcess, sizeBytes);
                appendElementTwo(mat, sizePTR, randProcess);
                sizePTR++;
            }
            break;
        case 1:
            ptrRandom = getPTRRamdon(mat);
            fprintf(archivo, "use(%d)\n", ptrRandom);
            break;
        case 2:
            ptrRandom = getPTRRamdon(mat);
            fprintf(archivo, "delete(%d)\n", ptrRandom);
            deleteElement(mat, ptrRandom);
            break;
        case 3:
            randProcess = obtenerElementoRandom(&listProcess);
            fprintf(archivo, "kill(%d)\n", randProcess);
            eliminarElemento(&listProcess, obtenerIndice(&listProcess, randProcess));
            struct Matrix aux = deleteElementProcess(mat, randProcess);
            mat = &aux;
            saveProcessKill++;
            // printf("Nueva tabla con proceso eliminado: %d\n", randProcess);
            // printMatrix(mat);
            break;
        
        default:
            break;
        }
    }
    // printf("Tabla final\n");
    // printMatrix(mat);
    for(int i = 0; i < listProcess.longitud; i++){
        fprintf(archivo, "kill(%d)\n", obtenerElemento(&listProcess, i));
    }
    liberarLista(&listProcess);
    fclose(archivo);
}
#endif