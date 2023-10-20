#ifndef SVIRTUAL_H
#define SVIRTUAL_H
#include <stdio.h>
#include <stdlib.h>

// Definición de la estructura de la lista
struct Virtual {
    int *datos;     // Puntero a los datos (arreglo dinámico)
    int capacidad;  // Capacidad actual del arreglo
    int cantidadDatos;   // Número de elementos en la lista
};

// Función para inicializar la lista
void inicializarVirtual(struct Virtual *mVirtual, int capacidadInicial) {
    mVirtual->datos = (int *)malloc(capacidadInicial * sizeof(int));
    mVirtual->capacidad = capacidadInicial;
    mVirtual->cantidadDatos = capacidadInicial;
    for(int i = 0; i < capacidadInicial; i++) {
        mVirtual->datos[i] = -1;
    }
}

// Función para agregar un elemento al final de la lista
int agregarElementoVirtual(struct Virtual *mVirtual, int valor) {
    int auxCapacidad = mVirtual->capacidad;
    if (mVirtual->cantidadDatos == 0) {
        // Si la lista está llena, aumenta su capacidad
        mVirtual->capacidad += 100;
        mVirtual->cantidadDatos = 100;
        mVirtual->datos = (int *)realloc(mVirtual->datos, mVirtual->capacidad * sizeof(int));

        for(int i = auxCapacidad; i < mVirtual->capacidad; i++) {
            mVirtual->datos[i] = -1;
        }
    }

    for(int i = 0; i < mVirtual->capacidad; i++) {
        if(mVirtual->datos[i] == -1) {
            mVirtual->datos[i] = valor;
            mVirtual->cantidadDatos--;
            return i;
        }
    }
    return -1;
}

// Funcion para cambiar proceso y pagina
void cambiarElementoVirtual(struct Virtual *mVirtual, int pos, int pagina) {
    mVirtual->datos[pos] = pagina;
}

// Función para eliminar un elemento por índice
void eliminarElementoVirtual(struct Virtual *mVirtual, int indice) {
    mVirtual->datos[indice] = -1;
    mVirtual->cantidadDatos++;
}

//Función para obtener un elemento por índice
int obtenerIndiceVirtual(struct Virtual *mVirtual, int pagina) {
    // validar que el indice sea válido
    for(int i = 0; i < mVirtual->capacidad; i++) {
        if(mVirtual->datos[i] == pagina) {
            return i;
        }
    }
    return -1;
}

// Función para imprimir la lista
void imprimirVirtual(struct Virtual *mVirtual) {
    printf("Memoria virtual: ");
    for (int i = 0; i < mVirtual->capacidad; i++) {
        printf("%d, ", mVirtual->datos[i]);
    }
    printf("\n");
}

// Función para liberar la memoria de la lista
void liberarVirtual(struct Virtual *mVirtual) {
    free(mVirtual->datos);
    mVirtual->datos = NULL;
    mVirtual->capacidad = 0;
    mVirtual->cantidadDatos = 0;
}

#endif