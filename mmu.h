#ifndef MMU_H
#define MMU_H

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <unistd.h> 
#include "matriz.h"
#include "listA.h"
#include "util.h"
#include "sram.h"
#include "svirtual.h"
#include "queue.h"

#define TAMANNORAM 4000 

// // Main Function

struct RAM RamOPT; // Algoritmo opt
struct RAM RamAlg; // algoritmo FIFO, Second chance, MRU y RDN
//

struct Virtual HDD1;//memoria virtual
struct Virtual HDD2; //memoria virtual

struct Lista listaProcesos; // lista de procesos


struct Matrix tablaPunteros; // tabla de punteros, procesos, tamaño

struct Matrix tablaPaginasOPT; // tabla de pagina, puntero, esta en memeoria, Dir. memoria, timestamp, bit
struct Matrix tablaPaginasAlg; // tabla de pagina, puntero, esta en memeoria, Dir. memoria, timestamp, bit

struct Lista futuroOPT; // lista de procesos

int algoritmoSeleccionado = 4;

char *nombreArchivo = NULL; // Nombre de txt

// Estadissticas  
int tiempoOPT = 0;
int tiempoAlg = 0;

int trashingOpt = 0;
int porcentajeTrashingOpt = 0;
int trashingAlg = 0;
int porcentajeTrashingAlg = 0;

int procesos = 0;

int ramKBOPT = 0;
int porcentajeOPT = 0;
int ramKBAlg = 0;
int porcentajeAlg = 0;

int virtualKBOPT = 0;
int porcentajeVirtualOPT = 0;
int virtualKBAlg = 0;
int porcentajeVirtualAlg = 0;

int paginasCargadaOPT = 0;
int paginasFueraOPT = 0;

int paginasCargadaAlg = 0;
int paginasFueraAlg = 0;

int fragmentacionInternaOPT = 0;
int fragmentacionInternaAlg = 0;

// Funcion para obtener los numeros de la operacion new
void obtenerNumerosNew(int *numero1, int *numero2, const char *cadena){
    char numero1_str[10];
    char numero2_str[10];

    // Buscar el primer número
    const char *inicio_numero1 = strchr(cadena, '(') + 1;
    const char *fin_numero1 = strchr(inicio_numero1, ',');
    strncpy(numero1_str, inicio_numero1, fin_numero1 - inicio_numero1);
    numero1_str[fin_numero1 - inicio_numero1] = '\0';

    // Buscar el segundo número
    const char *inicio_numero2 = fin_numero1 + 1;
    const char *fin_numero2 = strchr(inicio_numero2, ')');
    strncpy(numero2_str, inicio_numero2, fin_numero2 - inicio_numero2);
    numero2_str[fin_numero2 - inicio_numero2] = '\0';

    // Convertir las cadenas a enteros
    *numero1 = atoi(numero1_str);
    *numero2 = atoi(numero2_str);
}

// Funcion para obtener el numero de la operacion como use, delete y kill
void obtenerNumeroOper(int *numero1, const char *cadena){
    char numero1_str[10];

    // Buscar el primer número
    const char *inicio_numero1 = strchr(cadena, '(') + 1;
    const char *fin_numero1 = strchr(inicio_numero1, ')');
    strncpy(numero1_str, inicio_numero1, fin_numero1 - inicio_numero1);
    numero1_str[fin_numero1 - inicio_numero1] = '\0';

    // Convertir las cadenas a enteros
    *numero1 = atoi(numero1_str);
}

// Se obtiene cuanto espacio se ocupa en RAM
int ocupaEspacio(int tamanno){  
    double i = tamanno / (double) TAMANNORAM;
    int resultado = 0;
    while(i > 0){
        resultado++;
        i--;
    }
    return resultado;
}

//AlgoritmoOPT(numPagina, 0, ptr, &paginasUsadas);
//entradas: numero de pagina, opcion, ptr, lista de paginas usadas
void AlgoritmoOPT(int pagina, int operacion, int ptr, struct Lista *paginasUsadas){
    struct Lista auxPaginas;
    struct Lista opcionCambiar;
    struct Lista seUso;
    //struct Lista ordePAginas;
    inicializarLista(&auxPaginas, 10);
    inicializarLista(&opcionCambiar, 10);
    inicializarLista(&seUso, futuroOPT.longitud);
    for(int i = 0; i < RamOPT.capacidad; i++){
        if(RamOPT.datos2[i] != -1 && obtenerIndice(paginasUsadas, RamOPT.datos2[i]) == -1){
            agregarElemento(&auxPaginas, RamOPT.datos2[i]);
        }
    }

    agregarElemento(paginasUsadas, pagina);
    int ptrLimite = tablaPunteros.amount-1;
    if(operacion == 1) ptrLimite = tablaPunteros.amount;
    
    
    for(int i = 0; i < futuroOPT.longitud; i++){
        if(obtenerIndice(&seUso, futuroOPT.datos[i]) == -1 && futuroOPT.datos[i] < ptrLimite){
            
            getPageElementOPT(&tablaPaginasOPT, &auxPaginas, &opcionCambiar, futuroOPT.datos[i]);
            agregarElemento(&seUso, futuroOPT.datos[i]);
        }
        if(auxPaginas.longitud < 0){
            break;
        }
    }
    //obtiene el proceso 
    int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
    
    if(auxPaginas.longitud > 0){
        replacePageOPT(&tablaPaginasOPT, &RamOPT, &HDD1, auxPaginas.datos[auxPaginas.longitud-1], pagina, numeroProceso);
    }else{
        replacePageOPT(&tablaPaginasOPT, &RamOPT, &HDD1, opcionCambiar.datos[opcionCambiar.longitud-1], pagina, numeroProceso);
    }

    liberarLista(&auxPaginas);
    liberarLista(&opcionCambiar);
}

struct Queue *colaPaginas;
void algoritmoFIFO2(int pagina, int ptr){
    int paginaCambio = dequeue(colaPaginas);
    int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
    replacePageOPT(&tablaPaginasAlg, &RamAlg, &HDD2, paginaCambio, pagina, numeroProceso);
    enqueue(colaPaginas, pagina);
} 

// Primera version del FIFO que busca el primero que entra por el tiempo 
void algoritmoFIFO(int pagina, int ptr){
    //obtiene el proceso que se va a escoger de la RAM
    int tiempo = tablaPaginasAlg.data[tablaPaginasAlg.size-1][4];
    int paginaCambio = tablaPaginasAlg.data[tablaPaginasAlg.size-1][0];
    // printf("Pagina: %d \t Tiempo: \n", paginaCambio);
    // printf("tamanno: %d\n", tablaPaginasAlg.size);
    for(int i = 0; i < tablaPaginasAlg.size; i++){
        if(tablaPaginasAlg.data[i][2] == 1  && tablaPaginasAlg.data[i][4] < tiempo){
            // printf("Paginas que estan en ram: %d\n", tablaPaginasAlg.data[i][0]);
            tiempo = tablaPaginasAlg.data[i][4];
            paginaCambio = tablaPaginasAlg.data[i][0];
        }
    }
    // printf("Pagina a reemplazar: %d\n", paginaCambio);
    // printf("Pagina a reemplazar de oarametro: %d\n", pagina);
    //obtiene el proceso de la pagina que se va a cambiar
    int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
    replacePageOPT(&tablaPaginasAlg, &RamAlg, &HDD2, paginaCambio, pagina, numeroProceso);
} 

int verificarCambioSC(int paginaSC){
    if(tablaPaginasAlg.size == 0) return -1;
    for(int i = 0; i < tablaPaginasAlg.size; i++){
        if(tablaPaginasAlg.data[i][2] == 1  && tablaPaginasAlg.data[i][5] == 1 && tablaPaginasAlg.data[i][0] == paginaSC){
            tablaPaginasAlg.data[i][5] = 0;
            return 1;
        }
        if(tablaPaginasAlg.data[i][2] == 1  && tablaPaginasAlg.data[i][5] == 0 && tablaPaginasAlg.data[i][0] == paginaSC){
            return 0;
        }
    }
    return -1;
}

void algoritmoSC2(int pagina, int ptr){
    int paginaCambio = 0;
    int bucleSC = 1;
    while(bucleSC){
        paginaCambio = dequeue(colaPaginas);
        if(verificarCambioSC(paginaCambio) == 1){
            enqueue(colaPaginas, paginaCambio);
        }else{
            bucleSC = 0;
        }
    }
    int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
    replacePageOPT(&tablaPaginasAlg, &RamAlg, &HDD2, paginaCambio, pagina, numeroProceso);
    enqueue(colaPaginas, pagina);
} 

int poscionSC = 0;
void algoritmoSC(int pagina, int ptr){
    //cambiar lo que se va a hacer
    if(poscionSC > tablaPaginasAlg.size || poscionSC < 0) poscionSC = 0;
    int paginaCambio = 0;
    while(poscionSC <= tablaPaginasAlg.size){
        if(poscionSC == tablaPaginasAlg.size) poscionSC = 0;
        if(tablaPaginasAlg.data[poscionSC][2] == 1  && tablaPaginasAlg.data[poscionSC][5] == 1){
            tablaPaginasAlg.data[poscionSC][5] = 0;
        }
        if(tablaPaginasAlg.data[poscionSC][2] == 1  && tablaPaginasAlg.data[poscionSC][5] == 0){
            paginaCambio = tablaPaginasAlg.data[poscionSC][0];
            break;
        }
        poscionSC++;
    }

    if (paginaCambio == 0){
        printf("No hay paginas con bit 0\n");
        printMatrix(&tablaPaginasAlg);
        printf("pos: %d\n", poscionSC);
        sleep(10);
    }
    int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
    replacePageOPT(&tablaPaginasAlg, &RamAlg, &HDD2, paginaCambio, pagina, numeroProceso);
}

// int actualMRU = 0;

void algoritmoMRU(int pagina, int ptr){
    int paginaCambio = 0;
    for(int i = 0; i < tablaPaginasAlg.size; i++){
        // printf("Pagina a reemplazar: %d \t bit :%d \n", tablaPaginasAlg.data[i][0], tablaPaginasAlg.data[i][5]);
        if(tablaPaginasAlg.data[i][2] == 1 && tablaPaginasAlg.data[i][5] == 1){
            paginaCambio = tablaPaginasAlg.data[i][0];
            tablaPaginasAlg.data[i][5] = 0;
            break;
        }
    }
    if(paginaCambio == 0){
        printf("No hay paginas con bit 1\n");
        
    }
    int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
    replacePageOPT(&tablaPaginasAlg, &RamAlg, &HDD2, paginaCambio, pagina, numeroProceso);
}

void AlgoritmoRamdon(int pagina, int ptr, struct Lista *paginasUsadas){
    struct Lista auxPaginas;
    //struct Lista ordePAginas;
    inicializarLista(&auxPaginas, 10);
    for(int i = 0; i < RamAlg.capacidad; i++){
        if(RamAlg.datos2[i] != -1 && obtenerIndice(paginasUsadas, RamAlg.datos2[i]) == -1){
            agregarElemento(&auxPaginas, RamAlg.datos2[i]);
        }
    }
    // imprimirLista(&auxPaginas);
    agregarElemento(paginasUsadas, pagina);
    // se obtiene una pagina random
    int paginaRadom = obtenerElemento(&auxPaginas, rand()%auxPaginas.longitud);

    // printf("Pagina a reemplazar: %d\t pagina entrada: %d\n", paginaRadom, pagina);
    //obtiene el proceso 
    int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
    replacePageOPT(&tablaPaginasAlg, &RamAlg, &HDD2, paginaRadom, pagina, numeroProceso);
    
    liberarLista(&auxPaginas);
}

//entradas: numero de pagina, opcion, ptr, lista de paginas usadas
void algoritmoRemplazoUsar(int opcion, int pagina, int ptr, struct Lista *paginasUsadas){
    switch(opcion){
        case 1:
            //FIFO
            algoritmoFIFO2(pagina, ptr);
            break;
        case 2:
            algoritmoSC2(pagina, ptr);
            //Second chance
            break;
        case 3:
            algoritmoMRU(pagina, ptr);
            //MRU
            break;
        case 4:
            AlgoritmoRamdon(pagina, ptr, paginasUsadas);
            //RDN
            break;
    }
}

void borrarBitMRU(){
    if(tablaPaginasAlg.size == 0) return;
    for(int i = 0; i < tablaPaginasAlg.size; i++){
        if(tablaPaginasAlg.data[i][5] == 1){
            tablaPaginasAlg.data[i][5] = 0;
            break;
        }
    }
}

void operacionNew(const char *cadena){

    int proceso = 0;
    int tamanno = 0;
    obtenerNumerosNew(&proceso, &tamanno, cadena);
    // agregar a la lista de procesos
    if(listaProcesos.longitud < proceso){
        agregarElemento(&listaProcesos, proceso);
    }
    // agregar a la tabla de punteros
    appendElementPuntero(&tablaPunteros, proceso, tamanno);
    // // agregar a la tabla de paginas
    int ptr = tablaPunteros.amount-1;
    int ocupa = ocupaEspacio(tamanno);
    int  ocupa2 = ocupaEspacio(tamanno);

    // LLenar RAM del algo OPT
    while(ocupa > 0){
        if(RamOPT.cantidadDatos == 0) break; 
        // Se llena la ram que usa el algoritmo OPT
        int dirMemoria = agregarElementoRAM(&RamOPT, proceso, tablaPaginasOPT.amount);
        appendElementPagina(&tablaPaginasOPT, ptr, 1, dirMemoria, tiempoOPT, 0);
        tiempoOPT++;
        ocupa--;
    }
    // se hace el algoritmo de reemplazo si el ocupa es mayor a 0
    if(ocupa > 0){
        // printf("No hay espacio en RAM usar paginacion se uso remplazo por OPT\n");
        // se hace algoritmo de reemplazo
        struct Lista paginasUsadas;
        inicializarLista(&paginasUsadas, 10);
        for(int i = 0; i < ocupa; i++){
            int dirVirtual = agregarElementoVirtual(&HDD1, tablaPaginasOPT.amount);
            int numPagina = tablaPaginasOPT.amount;
            printf("Numero a reemplazar: %d\n", numPagina);
            appendElementPagina(&tablaPaginasOPT, ptr, 0, dirVirtual, tiempoOPT, 0);
            AlgoritmoOPT(numPagina, 0, ptr, &paginasUsadas);
            tiempoOPT+=5;
            trashingOpt+=5;
        }
        liberarLista(&paginasUsadas);
    }
    int bit = 0;
    if(algoritmoSeleccionado == 2 || algoritmoSeleccionado == 3) bit = 1; 
    
    // LLenar RAM por parte dee los demas algoritmos
    while(ocupa2 > 0){
        if(RamAlg.cantidadDatos == 0) break; 
        // Se llena la ram que usa n los demas algoritmos
        int dirMemoria = agregarElementoRAM(&RamAlg, proceso, tablaPaginasAlg.amount);
        
        // Esta linea pasa si se hace el algoritmo de MRU, para borrar en el que tenia el bit marcado
        if(algoritmoSeleccionado == 3) borrarBitMRU();
        
        //Se empieza a llenar la lista con las paginas como van entrando
        if(algoritmoSeleccionado == 1 || algoritmoSeleccionado == 2) enqueue(colaPaginas, tablaPaginasAlg.amount);
        appendElementPagina(&tablaPaginasAlg, ptr, 1, dirMemoria, tiempoAlg, bit);
        tiempoAlg++;
        ocupa2--;
    }

    // se hace el algoritmo de reemplazo si el ocupa es mayor a 0
    if(ocupa2 > 0){
        // printf("No hay espacio en RAM usar paginacion\n");
        // se hace algoritmo de reemplazo
        // AlgoritmoOtros();
        struct Lista paginasUsadasAlg;
        inicializarLista(&paginasUsadasAlg, 10);
        for(int i = 0; i < ocupa2; i++){
            int dirVirtual = agregarElementoVirtual(&HDD2, tablaPaginasAlg.amount);
            int numPagina = tablaPaginasAlg.amount;
            printf("Numero a reemplazar: %d\n", numPagina);
            appendElementPagina(&tablaPaginasAlg, ptr, 0, dirVirtual, tiempoAlg, bit);
            algoritmoRemplazoUsar(algoritmoSeleccionado, numPagina, ptr, &paginasUsadasAlg);
            tiempoAlg+=5;
            trashingAlg+=5;
        }
        liberarLista(&paginasUsadasAlg);
    }
    
    // imprimirRAM(&RamOPT);
    // imprimirLista(&listaProcesos);
    // printMatrix(&tablaPunteros);
    // printf("Tabla de paginas\n");
    // printMatrix(&tablaPaginasOPT);
}

// 
void verificacionRam(struct Matrix *auxPaginas, int ptr, int opcion){
    int principio = 0;
    struct Lista paginasUsadas;
    inicializarLista(&paginasUsadas, 10);
    struct Lista paginasUsadasAlg;
    inicializarLista(&paginasUsadasAlg, 10);
    // if(opcion == 1){
    //     printf("Tabla de paginas Fuera for\n");
    //     printMatrix(&tablaPaginasAlg);
    // }
    for(int i = 0; i < auxPaginas->size; i++){
        if(auxPaginas->data[i][1] == ptr ){
            if(principio != 1) principio = 1;
            if(auxPaginas->data[i][2] == 1){
                // printf("hit\n");
                if (algoritmoSeleccionado == 2 && auxPaginas->data[i][5] == 0 && opcion != 0){
                    auxPaginas->data[i][5] = 1;
                }else if(algoritmoSeleccionado == 3 && opcion != 0){
                    // Se setea el bit de MRU
                    borrarBitMRU();
                    auxPaginas->data[i][5] = 1;
                }

                
                if(opcion == 0){
                    // entra si es el algoritmo opt
                    tiempoOPT++;
                    agregarElemento(&paginasUsadas, auxPaginas->data[i][0]);
                } else tiempoAlg++;
                // tiempo++;
            }else{
                // printf("miss\n");
                // opcion 0 es para el algoritmo opt
                if(opcion == 0){

                    if(RamOPT.cantidadDatos > 0) {
                        int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
                        int dirMemoria = agregarElementoRAM(&RamOPT, numeroProceso, auxPaginas->data[i][0]);
                        // se cambia direccion de memoria en la tabla de paginas
                        eliminarElementoVirtual(&HDD1, auxPaginas->data[i][3]);
                        auxPaginas->data[i][2] = 1;
                        auxPaginas->data[i][3] = dirMemoria;
                        auxPaginas->data[i][4] = tiempoOPT;
                    }else{
                        printf("Numero a reemplazar: %d\n", auxPaginas->data[i][0]);
                        auxPaginas->data[i][4] = tiempoOPT;
                        AlgoritmoOPT(auxPaginas->data[i][0], 1, auxPaginas->data[i][1], &paginasUsadas);
                    }
                    tiempoOPT+=5;
                    trashingOpt+=5;
                }else{
                    // Demas algoritmos
                    if(RamAlg.cantidadDatos > 0) {
                        int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
                        int dirMemoria = agregarElementoRAM(&RamAlg, numeroProceso, auxPaginas->data[i][0]);
                        // se cambia direccion de memoria en la tabla de paginas
                        eliminarElementoVirtual(&HDD2, auxPaginas->data[i][3]);
                        auxPaginas->data[i][2] = 1;
                        auxPaginas->data[i][3] = dirMemoria;
                        auxPaginas->data[i][4] = tiempoAlg;

                        if(algoritmoSeleccionado == 1 || algoritmoSeleccionado == 2){
                            //Se empieza a llenar la lista con las paginas como van entrando
                            enqueue(colaPaginas, auxPaginas->data[i][0]);
                        } 
                    }else{
                        printf("Numero a reemplazar: %d\n", auxPaginas->data[i][0]);
                        auxPaginas->data[i][4] = tiempoAlg;
                        algoritmoRemplazoUsar(algoritmoSeleccionado, auxPaginas->data[i][0], auxPaginas->data[i][1], &paginasUsadasAlg);
                        // AlgoritmoOPT(auxPaginas->data[i][0], 1, auxPaginas->data[i][1], &paginasUsadas);
                    }

                    if (algoritmoSeleccionado == 2 && opcion != 0){
                        auxPaginas->data[i][5] = 1;
                    }else if (algoritmoSeleccionado == 3 && opcion != 0){
                        borrarBitMRU();
                        auxPaginas->data[i][5] = 1;
                    }
                    tiempoAlg+=5;
                    trashingAlg+=5;
                }
                
            }
        }else{
            if(principio == 1) break;
        }
        liberarLista(&paginasUsadas);
    }
}

void operacionUse(const char *cadena){
    int ptr = 0;

    //Algortimo opt
    obtenerNumeroOper(&ptr, cadena);
    // se busca en la tabla de paginas
    verificacionRam(&tablaPaginasOPT, ptr, 0);

    verificacionRam(&tablaPaginasAlg, ptr, 1);
    eliminarElemento(&futuroOPT, 0);
}

void borrarPagina(int ptr){
    tablaPaginasOPT = deleteElementPage(&tablaPaginasOPT, &RamOPT, &HDD1, NULL, ptr, 0);
    int cantidadPagBorradas = 0;
    
    if(algoritmoSeleccionado == 2) cantidadPagBorradas = tablaPaginasAlg.size;
    
    tablaPaginasAlg = deleteElementPage(&tablaPaginasAlg, &RamAlg, &HDD2, colaPaginas, ptr, algoritmoSeleccionado);
    
    if(algoritmoSeleccionado == 2) {
        cantidadPagBorradas -= tablaPaginasAlg.size;
        poscionSC -= cantidadPagBorradas;
    }
    // printf("puntero de second chance: %d\n", poscionSC);
}

void operacionDelete(const char *cadena){
    int ptr = 0;
    obtenerNumeroOper(&ptr, cadena);
    // Borrar de la tabla de punteros
    deleteElementPtr(&tablaPunteros, ptr);
    // Borrar de la tabla de paginas
    borrarPagina(ptr);
    // imprimirRAM(&RamOPT);
    // imprimirLista(&listaProcesos);
    // printMatrix(&tablaPunteros);
    // printf("Tabla de paginas\n");
    // printMatrix(&tablaPaginasOPT);
}

void operacionKill(const char *cadena){
    int ptr = 0;
    struct Lista borrarPtr;
    inicializarLista(&borrarPtr, 5);
    obtenerNumeroOper(&ptr, cadena);
    //Borrar proceso de tabla de procesos
    eliminarElemento(&listaProcesos, obtenerIndice(&listaProcesos, ptr));
    //Borra todo los punteros asociados con el proceso
    tablaPunteros = deleteElementProcessPtr(&tablaPunteros, &borrarPtr, ptr);

    //Borrar resto de tablas
    for(int i = 0; i < borrarPtr.longitud; i++){
        borrarPagina(borrarPtr.datos[i]);
    }
    // Borrar de la tabla de paginas
    // borrarPagina(ptr);
    // printf("Procesos\n");
    // imprimirLista(&listaProcesos);
    // imprimirLista(&borrarPtr);
    // printMatrix(&tablaPunteros);
    // imprimirRAM(&RamOPT);
    // printf("Tabla de paginas\n");
    // printMatrix(&tablaPaginasOPT);
}

void contarPaginas(){
    paginasCargadaOPT = 0;
    paginasFueraOPT = 0;
    paginasCargadaAlg = 0;
    paginasFueraAlg = 0;
    for(int i = 0; i < tablaPaginasOPT.size; i++){
        if(tablaPaginasOPT.data[i][2] == 1) paginasCargadaOPT++;
        else paginasFueraOPT++;

        if(tablaPaginasAlg.data[i][2] == 1) paginasCargadaAlg++;
        else paginasFueraAlg++;
    }
}

void contarFragmentacion(){
    fragmentacionInternaAlg = 0;
    fragmentacionInternaOPT = 0;
    fragmentacionInternaOPT = (tablaPaginasOPT.size*4)*1000;
    fragmentacionInternaAlg = (tablaPaginasAlg.size*4)*1000;
    for(int i = 0; i < tablaPunteros.size; i++){
        fragmentacionInternaAlg -= tablaPunteros.data[i][2];
        fragmentacionInternaOPT -= tablaPunteros.data[i][2];
    }
    fragmentacionInternaOPT = ((float)fragmentacionInternaOPT/1000);
    fragmentacionInternaAlg = ((float)fragmentacionInternaAlg/1000);
}

// void contarFragmentacion(){
//     fragmentacionInternaOPT = 0;
//     fragmentacionInternaAlg = 0;
//     for(int i = 0; i < tablaPaginasOPT.size; i++){
//         if(tablaPaginasOPT.data[i][2] == 1) fragmentacionInternaOPT += 4000 - tablaPunteros.data[tablaPaginasOPT.data[i][1]][2];
//         if(tablaPaginasAlg.data[i][2] == 1) fragmentacionInternaAlg += 4000 - tablaPunteros.data[tablaPaginasAlg.data[i][1]][2];
//     }
// }

// int prueba5(){
//     FILE *archivo;
//     regex_t regexNew, regexUse, regexDelete, regexKill;
//     char *nombreArchivo = "simulation.txt";
//     archivo = fopen(nombreArchivo, "r");

//     if(archivo == NULL){
//         printf("Error al abrir el archivo");
//         return 1;
//     }

//     int status;
//     status = regcomp(&regexNew, "new\\([[:digit:]]+,[[:digit:]]+\\)", REG_EXTENDED);
//     if (status != 0) {
//         // Manejar errores de compilación de la expresión regular
//         return 1;
//     }

//     status = regcomp(&regexUse, "use\\([[:digit:]]+\\)", REG_EXTENDED);
//     if (status != 0) {
//         // Manejar errores de compilación de la expresión regular
//         return 1;
//     }

//     status = regcomp(&regexDelete, "delete\\([[:digit:]]+\\)", REG_EXTENDED);
//     if (status != 0) {
//         // Manejar errores de compilación de la expresión regular
//         return 1;
//     }

//     status = regcomp(&regexKill, "kill\\([[:digit:]]+\\)", REG_EXTENDED);
//     if (status != 0) {
//         // Manejar errores de compilación de la expresión regular
//         return 1;
//     }

//     char linea[100];
//     printf("Se encontraron las siguientes operaciones:\n");
//     inicializarRAM(&RamOPT, 100);
//     inicializarRAM(&RamAlg, 100);
//     int numeroProcesos = 10;
//     inicializarLista(&listaProcesos, numeroProcesos); // Inicializar lista de procesos
//     tablaPunteros = createMatrix(numeroProcesos, 3); // crear tabla de punteros
//     tablaPaginasOPT = createMatrix(5, 6); //crear tabla de paginas OPT
//     tablaPaginasAlg = createMatrix(5, 6); //crear tabla de paginas demas algoritmos
//     inicializarVirtual(&HDD1, 4); // Memoria virtual para el OPT
//     inicializarVirtual(&HDD2, 4); // Memoria virtual para los demas algoritmos
//     inicializarLista(&futuroOPT, 10); // lista a futuro de procesos
    
//     while(fgets(linea, sizeof(linea),archivo) != NULL){
//         status = regexec(&regexUse, linea, 0, NULL, 0);
//         if (status == 0) {
//             int ptr = 0;
//             obtenerNumeroOper(&ptr, linea);
//             agregarElemento(&futuroOPT, ptr);
//         }

//     }
//     // imprimirLista(&futuroOPT);
//     fclose(archivo);

//     archivo = fopen(nombreArchivo, "r");
//     while(fgets(linea, sizeof(linea),archivo) != NULL){
//         status = regexec(&regexNew, linea, 0, NULL, 0);
//         if (status == 0) {
//             printf("%s", linea); // se hace operacion
//             operacionNew(linea);
//         }

//         status = regexec(&regexUse, linea, 0, NULL, 0);
//         if (status == 0) {
//             printf("%s", linea); // se hace operacion
//             operacionUse(linea); 
//         }

//         status = regexec(&regexDelete, linea, 0, NULL, 0);
//         if (status == 0) {
//             printf("%s", linea); // se hace operacion
//             operacionDelete(linea); 
//         }

//         status = regexec(&regexKill, linea, 0, NULL, 0);
//         if (status == 0) {
//             printf("%s", linea); // se hace operacion
//             operacionKill(linea); 
//         }
//         // printf("Lista de procesosos a Futuro :");
//         // imprimirLista(&futuroOPT);
//         // imprimirRAMPaginas(&RamOPT);
//         // imprimirVirtual(&HDD1);
//         // printf("Ram de los algoritmos\n");
//         // imprimirRAMPaginas(&RamAlg);
//         // imprimirVirtual(&HDD2);
//         // sleep(2);
//         // imprimirLista(&listaProcesos);
//         // printMatrix(&tablaPaginasOPT);
//     }
//     printf("Tiempo opt: %d\n", tiempoOPT);
//     printf("Tiempo alg: %d\n", tiempoAlg);
//     printf("Trashing opt: %d\n", trashingOpt);
//     printf("Trashing alg: %d\n", trashingAlg);
//     //liberar memoria
//     regfree(&regexDelete);
//     regfree(&regexKill);
//     regfree(&regexUse);
//     regfree(&regexNew);
//     fclose(archivo);
//     return 0;
// }

void liberarMemoriaPrograma(){
    //liberar memoria
    liberarLista(&listaProcesos);
    liberarRAM(&RamOPT);
    liberarRAM(&RamAlg);
    freeMatrix(&tablaPunteros);
    freeMatrix(&tablaPaginasOPT);
    freeMatrix(&tablaPaginasAlg);
    liberarVirtual(&HDD1);
    liberarVirtual(&HDD2);
    liberarLista(&futuroOPT);
}
#endif