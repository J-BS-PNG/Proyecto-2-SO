#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>

struct Queue {
    int *items;
    int front;
    int rear;
    int capacity;
};

// Función para crear una cola vacía con una capacidad inicial
struct Queue* createQueue(int capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = -1;
    queue->rear = -1;
    queue->capacity = capacity;
    queue->items = (int*)malloc(sizeof(int) * capacity);
    return queue;
}

// Función para verificar si la cola está vacía
int isEmpty(struct Queue* queue) {
    return (queue->front == -1);
}

// Función para verificar si la cola está llena
int isFull(struct Queue* queue) {
    return (queue->rear == queue->capacity - 1);
}

// Función para redimensionar la cola duplicando su capacidad
void resizeQueue(struct Queue* queue) {
    int newCapacity = queue->capacity * 2;
    queue->items = (int*)realloc(queue->items, sizeof(int) * newCapacity);
    queue->capacity = newCapacity;
}

// Función para agregar un elemento a la cola (encolar)
void enqueue(struct Queue* queue, int value) {
    if (isFull(queue)) {
        // Si la cola está llena, redimensionar
        resizeQueue(queue);
    }
    
    if (isEmpty(queue)) {
        queue->front = 0;
    }
    
    queue->rear++;
    queue->items[queue->rear] = value;
}

// Función para eliminar un elemento de la cola (desencolar)
int dequeue(struct Queue* queue) {
    int item;
    if (isEmpty(queue)) {
        printf("La cola está vacía. No se puede desencolar.\n");
        return -1;
    } else {
        item = queue->items[queue->front];
        if (queue->front >= queue->rear) {
            queue->front = queue->rear = -1;
        } else {
            queue->front++;
        }
        return item;
    }
}

// Función para obtener el elemento en el frente de la cola sin eliminarlo
int peek(struct Queue* queue) {
    if (isEmpty(queue)) {
        printf("La cola está vacía. No hay elementos para ver.\n");
        return -1;
    } else {
        return queue->items[queue->front];
    }
}

// Función para eliminar la primera ocurrencia de un número en la cola
void deleteFirstOccurrence(struct Queue* queue, int elementToDelete) {
    if (isEmpty(queue)) {
        printf("La cola está vacía. No se puede eliminar elementos.\n");
        return;
    }

    int originalSize = queue->rear - queue->front + 1;
    int found = 0;

    for (int i = 0; i < originalSize; i++) {
        int item = dequeue(queue);
        if (item == elementToDelete && !found) {
            found = 1;
            continue; // Saltar la primera ocurrencia
        }
        enqueue(queue, item);
    }

    if (!found) {
        printf("El elemento %d no se encontró en la cola.\n", elementToDelete);
    }
}

void printQueue(struct Queue* queue) {
    if (isEmpty(queue)) {
        printf("La cola está vacía.\n");
        return;
    }

    printf("Elementos en la cola: ");
    for (int i = queue->front; i <= queue->rear; i++) {
        printf("%d ", queue->items[i]);
    }
    printf("\n");
}
#endif