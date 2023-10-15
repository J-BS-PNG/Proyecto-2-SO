#ifndef SRAM_H
#define SRAM_H
#include <stdio.h>
#include <stdlib.h>

// Definición de la estructura de la lista
struct RAM {
    int *datos;     // Puntero a los datos (arreglo dinámico)
    int capacidad;  // Capacidad actual del arreglo
    int cantidadDatos;   // Número de elementos en la lista
};

// Función para inicializar la lista
void inicializarRAM(struct RAM *ram, int capacidadInicial) {
    ram->datos = (int *)malloc(capacidadInicial * sizeof(int));
    ram->capacidad = capacidadInicial;
    ram->cantidadDatos = 400;
    for(int i = 0; i < capacidadInicial; i++) {
        ram->datos[i] = -1;
    }
}

// Función para agregar un elemento al final de la lista
int agregarElementoRAM(struct RAM *ram, int valor) {
    for(int i = 0; i < ram->capacidad; i++) {
        if(ram->datos[i] == -1) {
            ram->datos[i] = valor;
            ram->cantidadDatos--;
            return i;
        }
    }
    return -1;
}

// Función para ver si hay espacio en la RAM
int hayEspacioRAM(struct RAM *ram, int tamanno) {
    if(ram->cantidadDatos > tamanno) {
        return 1;
    }
    return 0;
}

// Función para eliminar un elemento por índice
void eliminarElementoRAM(struct RAM *ram, int indice) {
    printf("Eliminando elemento en índice %d\n", indice);
}


// Función para obtener un elemento por índice
// int obtenerElementoRAM(struct RAM *ram, int indice) {
//     // validar que el indice sea válido
// }


// Función para imprimir la lista
void imprimirRAM(struct RAM *ram) {
    printf("RAM: ");
    for (int i = 0; i < ram->capacidad; i++) {
        printf("%d, ", ram->datos[i]);
    }
    printf("\n");
}

// Función para liberar la memoria de la lista
void liberarRAM(struct RAM *ram) {
    free(ram->datos);
    ram->datos = NULL;
    ram->capacidad = 0;
    ram->cantidadDatos = 0;
}

#endif