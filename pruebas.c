// Preprocessor Directives
#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
// #include <stdbool.h>
// #include <regex.h>
// #include <string.h>
// #include <unistd.h> 
// #include "matriz.h"
// #include "listA.h"
// #include "util.h"
// #include "sram.h"
// #include "svirtual.h"

// #define TAMANNORAM 4000 


// // //Ejemplo de file
// // //      FILE *archivo;
// // //     archivo = fopen("p.txt", "r");

// // //     if(archivo == NULL){
// // //         printf("Error al abrir el archivo");
// // //         return 1;
// // //     }
    
// // //     char linea[100];
// // //     while(fgets(linea, sizeof(linea),archivo) != NULL){
// // //         printf("%s", linea);
// // //     }
// // //     fclose(archivo);

// // // Main Function

// struct RAM RamOPT; // Algoritmo opt
// struct RAM RamAlg; // algoritmo FIFO, Second chance, MRU y RDN

// struct Virtual HDD1;//memoria virtual
// struct Virtual HDD2; //memoria virtual

// struct Lista listaProcesos; // lista de procesos

// struct Matrix tablaPunteros; // tabla de punteros, procesos, tamaño

// struct Matrix tablaPaginasOPT; // tabla de pagina, puntero, esta en memeoria, Dir. memoria, timestamp, bit
// struct Matrix tablaPaginasAlg; // tabla de pagina, puntero, esta en memeoria, Dir. memoria, timestamp, bit

// struct Lista futuroOPT; // lista de procesos

// int algoritmoSeleccionado = 3;
// int tiempoOPT = 0;
// int tiempoAlg = 0;

// int trashingOpt = 0;
// int trashingAlg = 0;

// void prueba1(){
//     struct Matrix mat = createMatrix(5, 2); 
//     printf("se agregan dos filas en la matriz\n");
//     addRows(&mat, 2);
//     appendElementTwo(&mat, 1, 1);
//     appendElementTwo(&mat, 2, 2);
//     appendElementTwo(&mat, 3, 1);
//     appendElementTwo(&mat, 4, 2);
//     printf("Primera matriz\n");
//     printMatrix(&mat);
//     mat = deleteElementProcess(&mat, 1);
//     printf("Primera matriz con elementos borrados\n");
//     freeMatrix(&mat);
// }

// void prueba2(){
//     struct Matrix mat = createMatrix(5, 2);
//     createSimulationTxt(5, 100, &mat);
//     // printMatrix(&mat);
//     freeMatrix(&mat);
// }

// void prueba3(){
//     struct Lista lista;
//     inicializarLista(&lista, 5);
//     agregarElemento(&lista, 1);
//     agregarElemento(&lista, 10);
//     imprimirLista(&lista);
//     liberarLista(&lista);
// }

// int prueba4(){
//     regex_t regex;
//     regmatch_t matches[1]; // para almacenar los resultados de coincidencia
//     int status;
//     status = regcomp(&regex, "(new\\([[:digit:]]+, [[:digit:]]+\\)|use\\([[:digit:]]+\\))", REG_EXTENDED);
//     if (status != 0) {
//         // Manejar errores de compilación de la expresión regular
//         return 1;
//     }

//     const char *cadena = "new(1, 20)\n use(1) ";
//     int offset = 0;
//     while (1) {
//         status = regexec(&regex, cadena + offset, 1, matches, 0);

//         if (status == 0) {
//             printf("Cadena válida: %.*s\n", (int)(matches[0].rm_eo - matches[0].rm_so), &cadena[offset + matches[0].rm_so]);
//             offset += matches[0].rm_eo;
//         } else if (status == REG_NOMATCH) {
//             break; // No se encontraron más coincidencias
//         } else {
//             printf("Error al buscar coincidencia.\n");
//             break;
//         }
//     }
//     regfree(&regex);
//     return 0;
// }

// // Funcion para obtener los numeros de la operacion new
// void obtenerNumerosNew(int *numero1, int *numero2, const char *cadena){
//     char numero1_str[10];
//     char numero2_str[10];

//     // Buscar el primer número
//     const char *inicio_numero1 = strchr(cadena, '(') + 1;
//     const char *fin_numero1 = strchr(inicio_numero1, ',');
//     strncpy(numero1_str, inicio_numero1, fin_numero1 - inicio_numero1);
//     numero1_str[fin_numero1 - inicio_numero1] = '\0';

//     // Buscar el segundo número
//     const char *inicio_numero2 = fin_numero1 + 1;
//     const char *fin_numero2 = strchr(inicio_numero2, ')');
//     strncpy(numero2_str, inicio_numero2, fin_numero2 - inicio_numero2);
//     numero2_str[fin_numero2 - inicio_numero2] = '\0';

//     // Convertir las cadenas a enteros
//     *numero1 = atoi(numero1_str);
//     *numero2 = atoi(numero2_str);
// }

// // Funcion para obtener el numero de la operacion como use, delete y kill
// void obtenerNumeroOper(int *numero1, const char *cadena){
//     char numero1_str[10];

//     // Buscar el primer número
//     const char *inicio_numero1 = strchr(cadena, '(') + 1;
//     const char *fin_numero1 = strchr(inicio_numero1, ')');
//     strncpy(numero1_str, inicio_numero1, fin_numero1 - inicio_numero1);
//     numero1_str[fin_numero1 - inicio_numero1] = '\0';

//     // Convertir las cadenas a enteros
//     *numero1 = atoi(numero1_str);
// }

// // Se obtiene cuanto espacio se ocupa en RAM
// int ocupaEspacio(int tamanno){  
//     double i = tamanno / (double) TAMANNORAM;
//     int resultado = 0;
//     while(i > 0){
//         resultado++;
//         i--;
//     }
//     return resultado;
// }

// //AlgoritmoOPT(numPagina, 0, ptr, &paginasUsadas);
// //entradas: numero de pagina, opcion, ptr, lista de paginas usadas
// void AlgoritmoOPT(int pagina, int operacion, int ptr, struct Lista *paginasUsadas){


//     struct Lista auxPaginas;
//     struct Lista opcionCambiar;
//     struct Lista seUso;
//     //struct Lista ordePAginas;
//     inicializarLista(&auxPaginas, 10);
//     inicializarLista(&opcionCambiar, 10);
//     inicializarLista(&seUso, futuroOPT.longitud);
//     for(int i = 0; i < RamOPT.capacidad; i++){
//         if(RamOPT.datos2[i] != -1 && obtenerIndice(paginasUsadas, RamOPT.datos2[i]) == -1){
//             agregarElemento(&auxPaginas, RamOPT.datos2[i]);
//         }
//     }
//     agregarElemento(paginasUsadas, pagina);
//     int ptrLimite = tablaPunteros.amount-1;
//     if(operacion == 1) ptrLimite = tablaPunteros.amount;
    
//     // // printf("ptr limite: %d\n", ptrLimite);
//     // imprimirLista(&auxPaginas);
//     // printf("Tabla de paginas\n");
//     for(int i = 0; i < futuroOPT.longitud; i++){
//         if(obtenerIndice(&seUso, futuroOPT.datos[i]) == -1 && futuroOPT.datos[i] < ptrLimite){
//             // printf("Se uso la puntero: %d\n", futuroOPT.datos[i]);
//             getPageElementOPT(&tablaPaginasOPT, &auxPaginas, &opcionCambiar, futuroOPT.datos[i]);
//             agregarElemento(&seUso, futuroOPT.datos[i]);
//         }
//         if(auxPaginas.longitud < 0){
//             break;
//         }
//     }
//     //obtiene el proceso 
//     int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
//     // printf("Longitud de aux: %d\n", auxPaginas.longitud);
//     if(auxPaginas.longitud > 0){
//         replacePageOPT(&tablaPaginasOPT, &RamOPT, &HDD1, auxPaginas.datos[auxPaginas.longitud-1], pagina, numeroProceso);
//     }else{
//         replacePageOPT(&tablaPaginasOPT, &RamOPT, &HDD1, opcionCambiar.datos[opcionCambiar.longitud-1], pagina, numeroProceso);
//     }

    
//     //Ya con el procesos listo si auxPagina esta Vacia se usa la opcionCambiar
//     // printf("Paginas se cambiar primero si hay\n");
//     // imprimirLista(&auxPaginas);
//     // printf("Paginas a cambiar segundo si primero esta vacio\n");
//     // imprimirLista(&opcionCambiar);
//     // printf("Paginas en RAM\n");
//     // imprimirLista(&auxPaginas);
//     // imprimirVirtual(&HDD1);
//     liberarLista(&auxPaginas);
//     liberarLista(&opcionCambiar);
// }

// void algoritmoFIFO(int pagina, int ptr){
//     //obtiene el proceso que se va a escoger de la RAM
//     int tiempo = tablaPaginasAlg.data[tablaPaginasAlg.size-1][4];
//     int paginaCambio = tablaPaginasAlg.data[tablaPaginasAlg.size-1][0];
//     // printf("Pagina: %d \t Tiempo: \n", paginaCambio);
//     // printf("tamanno: %d\n", tablaPaginasAlg.size);
//     for(int i = 0; i < tablaPaginasAlg.size; i++){
//         if(tablaPaginasAlg.data[i][2] == 1  && tablaPaginasAlg.data[i][4] < tiempo){
//             // printf("Paginas que estan en ram: %d\n", tablaPaginasAlg.data[i][0]);
//             tiempo = tablaPaginasAlg.data[i][4];
//             paginaCambio = tablaPaginasAlg.data[i][0];
//         }
//     }
//     // printf("Pagina a reemplazar: %d\n", paginaCambio);
//     // printf("Pagina a reemplazar de oarametro: %d\n", pagina);
//     //obtiene el proceso de la pagina que se va a cambiar
//     int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
//     replacePageOPT(&tablaPaginasAlg, &RamAlg, &HDD2, paginaCambio, pagina, numeroProceso);
// } 

// int poscionSC = 0;
// void algoritmoSC(int pagina, int ptr){
//     int paginaCambio = 0;
//     while(poscionSC <= tablaPaginasAlg.size){
//         if(poscionSC == tablaPaginasAlg.size) poscionSC = 0;
//         if(tablaPaginasAlg.data[poscionSC][2] == 1  && tablaPaginasAlg.data[poscionSC][5] == 1){
//             tablaPaginasAlg.data[poscionSC][5] = 0;
//         }
//         if(tablaPaginasAlg.data[poscionSC][2] == 1  && tablaPaginasAlg.data[poscionSC][5] == 0){
//             paginaCambio = tablaPaginasAlg.data[poscionSC][0];
//             break;
//         }
//         poscionSC++;
//     }
//     int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
//     replacePageOPT(&tablaPaginasAlg, &RamAlg, &HDD2, paginaCambio, pagina, numeroProceso);
// }

// // int actualMRU = 0;

// void algoritmoMRU(int pagina, int ptr){
//     int paginaCambio = 0;
//     for(int i = 0; i < tablaPaginasAlg.size; i++){
//         // printf("Pagina a reemplazar: %d \t bit :%d \n", tablaPaginasAlg.data[i][0], tablaPaginasAlg.data[i][5]);
//         if(tablaPaginasAlg.data[i][2] == 1 && tablaPaginasAlg.data[i][5] == 1){
//             paginaCambio = tablaPaginasAlg.data[i][0];
//             tablaPaginasAlg.data[i][5] = 0;
//             break;
//         }
//     }
//     if(paginaCambio == 0){
//         printf("No hay paginas con bit 1\n");
        
//     }
//     int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
//     replacePageOPT(&tablaPaginasAlg, &RamAlg, &HDD2, paginaCambio, pagina, numeroProceso);
// }

// //entradas: numero de pagina, opcion, ptr, lista de paginas usadas
// void algoritmoRemplazoUsar(int opcion, int pagina, int ptr, struct Lista *paginasUsadas){
//     switch(opcion){
//         case 1:
//             //FIFO
//             algoritmoFIFO(pagina, ptr);
//             break;
//         case 2:
//             algoritmoSC(pagina, ptr);
//             //Second chance
//             break;
//         case 3:
//             algoritmoMRU(pagina, ptr);
//             //MRU
//             break;
//         case 4:
//             //RDN
//             break;
//     }
// }

// void borrarBitMRU(){
//     if(tablaPaginasAlg.size == 0) return;
//     for(int i = 0; i < tablaPaginasAlg.size; i++){
//         if(tablaPaginasAlg.data[i][5] == 1){
//             tablaPaginasAlg.data[i][5] = 0;
//             break;
//         }
//     }
// }

// void operacionNew(const char *cadena){

//     int proceso = 0;
//     int tamanno = 0;
//     obtenerNumerosNew(&proceso, &tamanno, cadena);
//     // agregar a la lista de procesos
//     if(listaProcesos.longitud < proceso){
//         agregarElemento(&listaProcesos, proceso);
//     }
//     // agregar a la tabla de punteros
//     appendElementPuntero(&tablaPunteros, proceso, tamanno);
//     // // agregar a la tabla de paginas
//     int ptr = tablaPunteros.amount-1;
//     int ocupa = ocupaEspacio(tamanno);
//     int  ocupa2 = ocupaEspacio(tamanno);
//     // LLenar RAM 
//     while(ocupa > 0){
//         if(RamOPT.cantidadDatos == 0) break; 
//         // Se llena la ram que usa el algoritmo OPT
//         int dirMemoria = agregarElementoRAM(&RamOPT, proceso, tablaPaginasOPT.amount);
//         appendElementPagina(&tablaPaginasOPT, ptr, 1, dirMemoria, tiempoOPT, 0);
//         tiempoOPT++;
//         ocupa--;
//     }
//     // se hace el algoritmo de reemplazo si el ocupa es mayor a 0
//     if(ocupa > 0){
//         // printf("No hay espacio en RAM usar paginacion se uso remplazo por OPT\n");
//         // se hace algoritmo de reemplazo
//         struct Lista paginasUsadas;
//         inicializarLista(&paginasUsadas, 10);
//         for(int i = 0; i < ocupa; i++){
//             int dirVirtual = agregarElementoVirtual(&HDD1, tablaPaginasOPT.amount);
//             int numPagina = tablaPaginasOPT.amount;
//             printf("Numero a reemplazar: %d\n", numPagina);
//             appendElementPagina(&tablaPaginasOPT, ptr, 0, dirVirtual, tiempoOPT, 0);
//             AlgoritmoOPT(numPagina, 0, ptr, &paginasUsadas);
//             tiempoOPT+=5;
//             trashingOpt+=5;
//         }
//         liberarLista(&paginasUsadas);
//     }
//     int bit = 0;
//     if(algoritmoSeleccionado == 2 || algoritmoSeleccionado == 3) bit = 1; 
//     // LLenar RAM
//     while(ocupa2 > 0){
//         if(RamAlg.cantidadDatos == 0) break; 
//         // Se llena la ram que usa n los demas algoritmos
//         int dirMemoria = agregarElementoRAM(&RamAlg, proceso, tablaPaginasAlg.amount);
//         // Esta linea pasa si se hace el algoritmo de MRU
//         if(algoritmoSeleccionado == 3) borrarBitMRU();
//         appendElementPagina(&tablaPaginasAlg, ptr, 1, dirMemoria, tiempoAlg, bit);
//         tiempoAlg++;
//         ocupa2--;
//     }

//     // se hace el algoritmo de reemplazo si el ocupa es mayor a 0
//     if(ocupa2 > 0){
//         // printf("No hay espacio en RAM usar paginacion\n");
//         // se hace algoritmo de reemplazo
//         // AlgoritmoOtros();
//         for(int i = 0; i < ocupa2; i++){
//             int dirVirtual = agregarElementoVirtual(&HDD2, tablaPaginasAlg.amount);
//             int numPagina = tablaPaginasAlg.amount;
//             printf("Numero a reemplazar: %d\n", numPagina);
//             appendElementPagina(&tablaPaginasAlg, ptr, 0, dirVirtual, tiempoAlg, bit);
//             algoritmoRemplazoUsar(algoritmoSeleccionado, numPagina, ptr, NULL);
//             tiempoAlg+=5;
//             trashingAlg+=5;
//         }
//     }
    
//     // imprimirRAM(&RamOPT);
//     // imprimirLista(&listaProcesos);
//     // printMatrix(&tablaPunteros);
//     // printf("Tabla de paginas\n");
//     // printMatrix(&tablaPaginasOPT);
// }

// // 
// void verificacionRam(struct Matrix *auxPaginas, int ptr, int opcion){
//     int principio = 0;
//     struct Lista paginasUsadas;
//     inicializarLista(&paginasUsadas, 10);
//     // if(opcion == 1){
//     //     printf("Tabla de paginas Fuera for\n");
//     //     printMatrix(&tablaPaginasAlg);
//     // }
//     for(int i = 0; i < auxPaginas->size; i++){
//         if(auxPaginas->data[i][1] == ptr ){
//             if(principio != 1) principio = 1;
//             if(auxPaginas->data[i][2] == 1){
//                 // printf("hit\n");
//                 if (algoritmoSeleccionado == 2 && auxPaginas->data[i][5] == 0){
//                     auxPaginas->data[i][5] = 1;
//                 }else if(algoritmoSeleccionado == 3 && opcion != 0){
//                     borrarBitMRU();
//                     auxPaginas->data[i][5] = 1;
//                 }
//                 if(opcion == 0) tiempoOPT++;
//                 else tiempoAlg++;
//                 // tiempo++;
//             }else{
//                 // printf("miss\n");
//                 // opcion 0 es para el algoritmo opt
//                 if(opcion == 0){

//                     if(RamOPT.cantidadDatos > 0) {
//                         int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
//                         int dirMemoria = agregarElementoRAM(&RamOPT, numeroProceso, auxPaginas->data[i][0]);
//                         // se cambia direccion de memoria en la tabla de paginas
//                         eliminarElementoVirtual(&HDD1, auxPaginas->data[i][3]);
//                         auxPaginas->data[i][2] = 1;
//                         auxPaginas->data[i][3] = dirMemoria;
//                         auxPaginas->data[i][4] = tiempoOPT;
//                     }else{
//                         printf("Numero a reemplazar: %d\n", auxPaginas->data[i][0]);
//                         auxPaginas->data[i][4] = tiempoOPT;
//                         AlgoritmoOPT(auxPaginas->data[i][0], 1, auxPaginas->data[i][1], &paginasUsadas);
//                     }
//                     tiempoOPT+=5;
//                     trashingOpt+=5;
//                 }else{
//                     // printMatrix(&tablaPaginasAlg);
//                     // Demas algoritmos
//                     if(RamAlg.cantidadDatos > 0) {
//                         int numeroProceso = getProcessElementPtr(&tablaPunteros, ptr);
//                         int dirMemoria = agregarElementoRAM(&RamAlg, numeroProceso, auxPaginas->data[i][0]);
//                         // se cambia direccion de memoria en la tabla de paginas
//                         eliminarElementoVirtual(&HDD2, auxPaginas->data[i][3]);
//                         auxPaginas->data[i][2] = 1;
//                         auxPaginas->data[i][3] = dirMemoria;
//                         auxPaginas->data[i][4] = tiempoAlg;
//                     }else{
//                         printf("Numero a reemplazar: %d\n", auxPaginas->data[i][0]);
//                         auxPaginas->data[i][4] = tiempoAlg;
//                         algoritmoRemplazoUsar(algoritmoSeleccionado, auxPaginas->data[i][0], auxPaginas->data[i][1], NULL);
//                         // AlgoritmoOPT(auxPaginas->data[i][0], 1, auxPaginas->data[i][1], &paginasUsadas);
//                     }

//                     if (algoritmoSeleccionado == 2){
//                         auxPaginas->data[i][5] = 1;
//                     }else if (algoritmoSeleccionado == 3 && opcion != 0){
//                         borrarBitMRU();
//                         auxPaginas->data[i][5] = 1;
//                     }
//                     tiempoAlg+=5;
//                     trashingAlg+=5;
//                 }
                
//             }
//         }else{
//             if(principio == 1) break;
//         }
//         liberarLista(&paginasUsadas);
//     }
// }

// void operacionUse(const char *cadena){
//     int ptr = 0;

//     //Algortimo opt
//     obtenerNumeroOper(&ptr, cadena);
//     // se busca en la tabla de paginas
//     verificacionRam(&tablaPaginasOPT, ptr, 0);

//     verificacionRam(&tablaPaginasAlg, ptr, 1);
//     eliminarElemento(&futuroOPT, 0);
// }

// void borrarPagina(int ptr){
//     tablaPaginasOPT = deleteElementPage(&tablaPaginasOPT, &RamOPT, &HDD1, ptr, 0);
//     tablaPaginasAlg = deleteElementPage(&tablaPaginasAlg, &RamAlg, &HDD2, ptr, algoritmoSeleccionado);
// }

// void operacionDelete(const char *cadena){
//     int ptr = 0;
//     obtenerNumeroOper(&ptr, cadena);
//     // Borrar de la tabla de punteros
//     deleteElementPtr(&tablaPunteros, ptr);
//     // Borrar de la tabla de paginas
//     borrarPagina(ptr);
//     // imprimirRAM(&RamOPT);
//     // imprimirLista(&listaProcesos);
//     // printMatrix(&tablaPunteros);
//     // printf("Tabla de paginas\n");
//     // printMatrix(&tablaPaginasOPT);
// }

// void operacionKill(const char *cadena){
//     int ptr = 0;
//     struct Lista borrarPtr;
//     inicializarLista(&borrarPtr, 5);
//     obtenerNumeroOper(&ptr, cadena);
//     //Borrar proceso de tabla de procesos
//     eliminarElemento(&listaProcesos, obtenerIndice(&listaProcesos, ptr));
//     //Borra todo los punteros asociados con el proceso
//     tablaPunteros = deleteElementProcessPtr(&tablaPunteros, &borrarPtr, ptr);

//     //Borrar resto de tablas
//     for(int i = 0; i < borrarPtr.longitud; i++){
//         borrarPagina(borrarPtr.datos[i]);
//     }
//     // Borrar de la tabla de paginas
//     // borrarPagina(ptr);
//     // printf("Procesos\n");
//     // imprimirLista(&listaProcesos);
//     // imprimirLista(&borrarPtr);
//     // printMatrix(&tablaPunteros);
//     // imprimirRAM(&RamOPT);
//     // printf("Tabla de paginas\n");
//     // printMatrix(&tablaPaginasOPT);
// }

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
    inicializarRAM(&RamOPT, 20);
    inicializarRAM(&RamAlg, 20);
    int numeroProcesos = 10;
    inicializarLista(&listaProcesos, numeroProcesos); // Inicializar lista de procesos
    tablaPunteros = createMatrix(numeroProcesos, 3); // crear tabla de punteros
    tablaPaginasOPT = createMatrix(5, 6); //crear tabla de paginas OPT
    tablaPaginasAlg = createMatrix(5, 6); //crear tabla de paginas demas algoritmos
    inicializarVirtual(&HDD1, 4); // Memoria virtual para el OPT
    inicializarVirtual(&HDD2, 4); // Memoria virtual para los demas algoritmos
    inicializarLista(&futuroOPT, 10); // lista a futuro de procesos

    if(algoritmoSeleccionado == 1 || algoritmoSeleccionado == 2){
        colaPaginas = createQueue(10);
    }
    
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
        procesos = listaProcesos.longitud; // cantidad de procesos
        ramKBOPT = (RamAlg.capacidad - RamOPT.cantidadDatos)*4; // cantidad de KB en la RAM OPT
        ramKBAlg = (RamAlg.capacidad - RamAlg.cantidadDatos)*4; // cantidad de KB en la RAM de los demas algoritmos
        porcentajeOPT = ((double)ramKBOPT/(RamOPT.capacidad*4))*100; // porcentaje de la RAM OPT
        porcentajeAlg = ((double)ramKBAlg/(RamAlg.capacidad*4))*100; // porcentaje de la RAM de los demas algoritmos
        virtualKBOPT = HDD1.capacidad - HDD1.cantidadDatos; // cantidad de KB en la memoria virtual OPT
        virtualKBAlg =  HDD2.capacidad - HDD2.cantidadDatos; // cantidad de KB en la memoria virtual de los demas algoritmos
        porcentajeVirtualOPT = ((double)virtualKBOPT/RamOPT.capacidad)*100; // porcentaje de la memoria virtual OPT
        porcentajeVirtualAlg = ((double)virtualKBAlg/RamAlg.capacidad)*100; // porcentaje de la memoria virtual de los demas algoritmos
        if(tiempoAlg != 0 ){
            porcentajeTrashingAlg = ((double)trashingAlg/tiempoAlg)*100; // porcentaje de trashing de los demas algoritmos
            porcentajeTrashingOpt = ((double)trashingOpt/tiempoOPT)*100; // porcentaje de trashing de OPT
        }
        contarPaginas();
        contarFragmentacion();
        printf("Procesos: %d\n", procesos);
        printf("Tiempo opt: %d\n", tiempoOPT);
        printf("Ram KB opt: %d\n", ramKBOPT);
        printf("RAM %% %d\n", porcentajeOPT);
        printf("V-RAM KB: %d\n", virtualKBOPT);
        printf("V-RAM %%: %d\n", porcentajeVirtualOPT);
        printf("Paginas Cargadas: %d\n", paginasCargadaOPT);
        printf("Paginas fuera: %d\n", paginasFueraOPT);
        printf("Trashing opt: %d\n", trashingOpt);
        printf("Porcentaje trashing opt: %d\n", porcentajeTrashingOpt);
        printf("Fragmentacion opt: %d\n", fragmentacionInternaOPT);
        printf("----------------------------------------\n");
        printf("Procesos: %d\n", procesos);
        printf("Tiempo alg: %d\n", tiempoAlg);
        printf("Ram KB alg: %d\n", ramKBAlg);
        printf("RAM %% %d\n", porcentajeAlg);
        printf("V-RAM KB: %d\n", virtualKBAlg);
        printf("V-RAM %%: %d\n", porcentajeVirtualAlg);
        printf("Paginas Cargadas: %d\n", paginasCargadaAlg);
        printf("Paginas fuera: %d\n", paginasFueraAlg);
        printf("Trashing alg: %d\n", trashingAlg);
        printf("Porcentaje trashing alg: %d\n", porcentajeTrashingAlg);
        printf("Fragmentacion alg: %d\n", fragmentacionInternaAlg);
        printf("----------------------------------------\n");

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
        // printQueue(colaPaginas);
        sleep(1);
        // printf("Lista de procesosos a Futuro :");
        // imprimirLista(&futuroOPT);
        // printf("Ram de los OPT\n");
        // imprimirRAMPaginas(&RamOPT);
        // imprimirVirtual(&HDD1);
        // printf("Ram de los algoritmos\n");
        // imprimirRAMPaginas(&RamAlg);
        // imprimirVirtual(&HDD2);
        
        // imprimirLista(&listaProcesos);
        // printf("Tabla de Paginas OPT\n");
        // printMatrix(&tablaPaginasOPT);
        // printf("Tabla de Paginas ALG\n");
        // printMatrix(&tablaPaginasAlg);
    }
    //liberar memoria
    regfree(&regexDelete);
    regfree(&regexKill);
    regfree(&regexUse);
    regfree(&regexNew);
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
    // imprimirRAMPaginas(&RamAlg);
    // imprimirVirtual(&HDD2);
    liberarMemoriaPrograma();
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
