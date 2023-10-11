#ifndef LISTA_H
#define LISTA_H
#include <stdio.h>
#include <stdlib.h>

// Definición de la estructura de la lista
struct Lista {
    int *datos;     // Puntero a los datos (arreglo dinámico)
    int capacidad;  // Capacidad actual del arreglo
    int longitud;   // Número de elementos en la lista
};

// Función para inicializar la lista
void inicializarLista(struct Lista *lista, int capacidadInicial) {
    lista->datos = (int *)malloc(capacidadInicial * sizeof(int));
    lista->capacidad = capacidadInicial;
    lista->longitud = 0;
}

// Función para agregar un elemento al final de la lista
void agregarElemento(struct Lista *lista, int valor) {
    if (lista->longitud == lista->capacidad) {
        // Si la lista está llena, aumenta su capacidad
        lista->capacidad *= 2;
        lista->datos = (int *)realloc(lista->datos, lista->capacidad * sizeof(int));
    }
    
    lista->datos[lista->longitud++] = valor;
}

// Función para eliminar un elemento por índice
void eliminarElemento(struct Lista *lista, int indice) {
    if (indice < 0 || indice >= lista->longitud) {
        printf("Índice fuera de rango.\n");
        return;
    }

    for (int i = indice; i < lista->longitud - 1; i++) {
        lista->datos[i] = lista->datos[i + 1];
    }

    lista->longitud--;

    // Si la lista está suficientemente vacía, reduce su capacidad
    if (lista->longitud < lista->capacidad / 2) {
        lista->capacidad /= 2;
        lista->datos = (int *)realloc(lista->datos, lista->capacidad * sizeof(int));
    }
}


// Función para obtener un elemento por índice
int obtenerElemento(struct Lista *lista, int indice) {
    if (indice < 0 || indice >= lista->longitud) {
        printf("Índice fuera de rango.\n");
        return -1; // Otra opción podría ser devolver un valor especial para indicar error
    }

    return lista->datos[indice];
}

// funcion para obtner el indice de un elemento
int obtenerIndice(struct Lista *lista, int valor) {
    for (int i = 0; i < lista->longitud; i++) {
        if (lista->datos[i] == valor) {
            return i;
        }
    }
    return -1;
}

// Funcion para obtener un elemento random
int obtenerElementoRandom(struct Lista *lista) {
    int indice = rand() % lista->longitud;
    return lista->datos[indice];
}

// Función para imprimir la lista
void imprimirLista(struct Lista *lista) {
    printf("Lista: ");
    for (int i = 0; i < lista->longitud; i++) {
        printf("%d, ", lista->datos[i]);
    }
    printf("\n");
}

// Función para liberar la memoria de la lista
void liberarLista(struct Lista *lista) {
    free(lista->datos);
    lista->datos = NULL;
    lista->capacidad = 0;
    lista->longitud = 0;
}

#endif