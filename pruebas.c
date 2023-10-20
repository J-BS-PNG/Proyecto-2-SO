// Preprocessor Directives
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <regex.h>
#include <string.h>
#include <unistd.h> 
#include "matriz.h"
#include "listA.h"
#include "util.h"
#include "sram.h"
#include "svirtual.h"

#define TAMANNORAM 4000 


// //Ejemplo de file
// //      FILE *archivo;
// //     archivo = fopen("p.txt", "r");

// //     if(archivo == NULL){
// //         printf("Error al abrir el archivo");
// //         return 1;
// //     }
    
// //     char linea[100];
// //     while(fgets(linea, sizeof(linea),archivo) != NULL){
// //         printf("%s", linea);
// //     }
// //     fclose(archivo);

// // Main Function

struct RAM RamOPT; // Algoritmo opt
struct RAM RamAlg; // algoritmo FIFO, Second chance, MRU y RDN

struct Virtual HDD1;//memoria virtual
struct Virtual HDD2; //memoria virtual

struct Lista listaProcesos; // lista de procesos

struct Matrix tablaPunteros; // tabla de punteros, procesos, tamaño

struct Matrix tablaPaginasOPT; // tabla de pagina, puntero, esta en memeoria, Dir. memoria, timestamp, bit
struct Matrix tablaPaginasAlg; // tabla de pagina, puntero, esta en memeoria, Dir. memoria, timestamp, bit

struct Lista futuroOPT; // lista de procesos

int tiempoOPT = 0;
int tiempoAlg = 0;

int trashingOpt = 0;
int trashingAlg = 0;

void prueba1(){
    struct Matrix mat = createMatrix(5, 2); 
    printf("se agregan dos filas en la matriz\n");
    addRows(&mat, 2);
    appendElementTwo(&mat, 1, 1);
    appendElementTwo(&mat, 2, 2);
    appendElementTwo(&mat, 3, 1);
    appendElementTwo(&mat, 4, 2);
    printf("Primera matriz\n");
    printMatrix(&mat);
    mat = deleteElementProcess(&mat, 1);
    printf("Primera matriz con elementos borrados\n");
    freeMatrix(&mat);
}

void prueba2(){
    struct Matrix mat = createMatrix(5, 2);
    createSimulationTxt(5, 100, &mat);
    // printMatrix(&mat);
    freeMatrix(&mat);
}

void prueba3(){
    struct Lista lista;
    inicializarLista(&lista, 5);
    agregarElemento(&lista, 1);
    agregarElemento(&lista, 10);
    imprimirLista(&lista);
    liberarLista(&lista);
}

int prueba4(){
    regex_t regex;
    regmatch_t matches[1]; // para almacenar los resultados de coincidencia
    int status;
    status = regcomp(&regex, "(new\\([[:digit:]]+, [[:digit:]]+\\)|use\\([[:digit:]]+\\))", REG_EXTENDED);
    if (status != 0) {
        // Manejar errores de compilación de la expresión regular
        return 1;
    }

    const char *cadena = "new(1, 20)\n use(1) ";
    int offset = 0;
    while (1) {
        status = regexec(&regex, cadena + offset, 1, matches, 0);

        if (status == 0) {
            printf("Cadena válida: %.*s\n", (int)(matches[0].rm_eo - matches[0].rm_so), &cadena[offset + matches[0].rm_so]);
            offset += matches[0].rm_eo;
        } else if (status == REG_NOMATCH) {
            break; // No se encontraron más coincidencias
        } else {
            printf("Error al buscar coincidencia.\n");
            break;
        }
    }
    regfree(&regex);
    return 0;
}

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
    
    // // printf("ptr limite: %d\n", ptrLimite);
    // imprimirLista(&auxPaginas);
    // printf("Tabla de paginas\n");
    for(int i = 0; i < futuroOPT.longitud; i++){
        if(obtenerIndice(&seUso, futuroOPT.datos[i]) == -1 && futuroOPT.datos[i] < ptrLimite){
            // printf("Se uso la puntero: %d\n", futuroOPT.datos[i]);
            getPageElementOPT(&tablaPaginasOPT, &auxPaginas, &opcionCambiar, futuroOPT.datos[i]);
            agregarElemento(&seUso, futuroOPT.datos[i]);
        }
        if(auxPaginas.longitud < 0){
            break;
        }
    }
    int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
    // printf("Longitud de aux: %d\n", auxPaginas.longitud);
    if(auxPaginas.longitud > 0){
        replacePageOPT(&tablaPaginasOPT, &RamOPT, &HDD1, auxPaginas.datos[auxPaginas.longitud-1], pagina, numeroProceso);
    }else{
        replacePageOPT(&tablaPaginasOPT, &RamOPT, &HDD1, opcionCambiar.datos[opcionCambiar.longitud-1], pagina, numeroProceso);
    }

    
    //Ya con el procesos listo si auxPagina esta Vacia se usa la opcionCambiar
    // printf("Paginas se cambiar primero si hay\n");
    // imprimirLista(&auxPaginas);
    // printf("Paginas a cambiar segundo si primero esta vacio\n");
    // imprimirLista(&opcionCambiar);
    // printf("Paginas en RAM\n");
    // imprimirLista(&auxPaginas);
    // imprimirVirtual(&HDD1);
    liberarLista(&auxPaginas);
    liberarLista(&opcionCambiar);
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
    if(hayEspacioRAM(&RamOPT, ocupa) == 1){
        for(int i = 0; i < ocupa; i++){
            int dirMemoria = agregarElementoRAM(&RamOPT, proceso, tablaPaginasOPT.amount);
            appendElementPagina(&tablaPaginasOPT, ptr, 1, dirMemoria, 0, 0);
        }
        tiempoOPT++;
    }else{
        printf("No hay espacio en RAM usar paginacion\n");
        // se hace algoritmo de reemplazo
        struct Lista paginasUsadas;
        inicializarLista(&paginasUsadas, 10);
        for(int i = 0; i < ocupa; i++){
            int dirVirtual = agregarElementoVirtual(&HDD1, tablaPaginasOPT.amount);
            int numPagina = tablaPaginasOPT.amount;
            printf("Numero a reemplazar: %d\n", numPagina);
            appendElementPagina(&tablaPaginasOPT, ptr, 0, dirVirtual, 0, 0);
            AlgoritmoOPT(numPagina, 0, ptr, &paginasUsadas);
            tiempoOPT+=5;
            trashingOpt+=5;
        }
        liberarLista(&paginasUsadas);
    }

    if(hayEspacioRAM(&RamAlg, ocupa) == 1){
        for(int i = 0; i < ocupa; i++){
            int dirMemoria = agregarElementoRAM(&RamAlg, proceso, tablaPaginasAlg.amount);
            appendElementPagina(&tablaPaginasAlg, ptr, 1, dirMemoria, 0, 0);
        }
        tiempoAlg++;
    }else{
        printf("No hay espacio en RAM usar paginacion\n");
        // se hace algoritmo de reemplazo
        // AlgoritmoOtros();
        for(int i = 0; i < ocupa; i++){
            int dirVirtual = agregarElementoVirtual(&HDD2, tablaPaginasAlg.amount);
            // int numPagina = tablaPaginasOPT.amount;
            //AlgoritmoOPT(numPagina);
            appendElementPagina(&tablaPaginasAlg, ptr, 0, dirVirtual, 0, 0);
            tiempoAlg+=5;
            trashingAlg+=5;
        }
    }
    
    // imprimirRAM(&RamOPT);
    // imprimirLista(&listaProcesos);
    // printMatrix(&tablaPunteros);
    // printf("Tabla de paginas\n");
    // printMatrix(&tablaPaginasOPT);
}

int estaEnRAM(struct Matrix *auxPaginas, int ptr){
    int principio = 0;
    for(int i = 0; i < auxPaginas->size; i++){
        if(principio == 0){
            if(auxPaginas->data[i][1] == ptr){
                principio++;
            }
        }else{
            if(auxPaginas->data[i][1] == ptr){
                if(auxPaginas->data[i][2] == 0){
                    return 0;
                }
            } else {
                return 1;
            }
        }
    }
    return 1;
}

void operacionUse(const char *cadena){
    int ptr = 0;

    //Algortimo opt
    obtenerNumeroOper(&ptr, cadena);
    // se busca en la tabla de paginas
    if(estaEnRAM(&tablaPaginasOPT, ptr)){
        printf("hit\n");
        tiempoOPT++;
    }else{
        //se hace algoritmo de reemplazo
        printf("miss\n");
        tiempoOPT+=5;
    }

    // Algortimo FIFO, Second chance, MRU y RDN
    if(estaEnRAM(&tablaPaginasAlg, ptr)){
        printf("hit\n");
        tiempoAlg++;
    }else{
        //se hace algoritmo de reemplazo
        printf("miss\n");
        tiempoAlg+=5;
    }
    eliminarElemento(&futuroOPT, 0);
}

void borrarPagina(int ptr){
    tablaPaginasOPT = deleteElementPage(&tablaPaginasOPT, &RamOPT, ptr);
    tablaPaginasAlg = deleteElementPage(&tablaPaginasAlg, &RamAlg, ptr);
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

int prueba5(){
    FILE *archivo;
    regex_t regexNew, regexUse, regexDelete, regexKill;
    char *nombreArchivo = "simulation.txt";
    archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL){
        printf("Error al abrir el archivo");
        return 1;
    }

    int status;
    status = regcomp(&regexNew, "new\\([[:digit:]]+,[[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        // Manejar errores de compilación de la expresión regular
        return 1;
    }

    status = regcomp(&regexUse, "use\\([[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        // Manejar errores de compilación de la expresión regular
        return 1;
    }

    status = regcomp(&regexDelete, "delete\\([[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        // Manejar errores de compilación de la expresión regular
        return 1;
    }

    status = regcomp(&regexKill, "kill\\([[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        // Manejar errores de compilación de la expresión regular
        return 1;
    }

    char linea[100];
    printf("Se encontraron las siguientes operaciones:\n");
    inicializarRAM(&RamOPT, 50);
    inicializarRAM(&RamAlg, 50);
    int numeroProcesos = 10;
    inicializarLista(&listaProcesos, numeroProcesos); // Inicializar lista de procesos
    tablaPunteros = createMatrix(numeroProcesos, 3); // crear tabla de punteros
    tablaPaginasOPT = createMatrix(5, 6); //crear tabla de paginas OPT
    tablaPaginasAlg = createMatrix(5, 6); //crear tabla de paginas demas algoritmos
    inicializarVirtual(&HDD1, 4); // Memoria virtual para el OPT
    inicializarVirtual(&HDD2, 4); // Memoria virtual para los demas algoritmos
    inicializarLista(&futuroOPT, 10); // lista a futuro de procesos
    
    while(fgets(linea, sizeof(linea),archivo) != NULL){
        status = regexec(&regexUse, linea, 0, NULL, 0);
        if (status == 0) {
            int ptr = 0;
            obtenerNumeroOper(&ptr, linea);
            agregarElemento(&futuroOPT, ptr);
        }

    }
    // imprimirLista(&futuroOPT);
    fclose(archivo);

    archivo = fopen(nombreArchivo, "r");
    while(fgets(linea, sizeof(linea),archivo) != NULL){
        status = regexec(&regexNew, linea, 0, NULL, 0);
        if (status == 0) {
            printf("%s", linea); // se hace operacion
            operacionNew(linea);
        }

        status = regexec(&regexUse, linea, 0, NULL, 0);
        if (status == 0) {
            printf("%s", linea); // se hace operacion
            operacionUse(linea); 
        }

        status = regexec(&regexDelete, linea, 0, NULL, 0);
        if (status == 0) {
            printf("%s", linea); // se hace operacion
            operacionDelete(linea); 
        }

        status = regexec(&regexKill, linea, 0, NULL, 0);
        if (status == 0) {
            printf("%s", linea); // se hace operacion
            operacionKill(linea); 
        }
        // printf("Lista de procesosos a Futuro :");
        // imprimirLista(&futuroOPT);
        imprimirRAMPaginas(&RamOPT);
        // sleep(3);
        // printMatrix(&tablaPaginasOPT);
        // imprimirVirtual(&HDD1);
        // printf("Tiempo opt: %d\n", tiempoOPT);
        // printf("Tiempo alg: %d\n", tiempoAlg);
        // printf("Trashing opt: %d\n", trashingOpt);
        // printf("Trashing alg: %d\n", trashingAlg);
    }
    //liberar memoria
    regfree(&regexDelete);
    regfree(&regexKill);
    regfree(&regexUse);
    regfree(&regexNew);
    liberarLista(&listaProcesos);
    liberarRAM(&RamOPT);
    liberarRAM(&RamAlg);
    freeMatrix(&tablaPunteros);
    freeMatrix(&tablaPaginasOPT);
    freeMatrix(&tablaPaginasAlg);
    liberarVirtual(&HDD1);
    liberarVirtual(&HDD2);
    liberarLista(&futuroOPT);
    fclose(archivo);
    return 0;
}

int main() {

    srand(1200);
    // for(int i = 0; i < 10; i++){
    //     printf("%d\n", rand()%10000);
    // }
    // bool listProcess[10];
    // initBoolList(listProcess, 10);
    // for(int i = 0; i < 10; i++){
    //     printf("%d\n", listProcess[i]);
    // }
    // createSimulationTxt(10, 10);
    int err = prueba5();
    printf("\nDevuelve: %d\n", err);
    // int numero = 0;
    // int numero2 = 0;
    // obtenerNumerosNew(&numero, &numero2, "new(1, 20)");
    // printf("Número 1: %d\n", numero);
    // printf("Número 2: %d\n", numero2);

    // int numero3 = 0;
    // obtenerNumeroOper(&numero3, "use(1)");
    // printf("Número 3: %d\n", numero3);
    return 0;
    }
