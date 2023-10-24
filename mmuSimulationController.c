//gcc -o prueba  mmuSimulationController.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -rdynamic

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h> //pal hilo que gtk tiene jjiijjiij
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "util.h"
#include "matriz.h"
#include "mmu.h"

//Widgets del menu
GtkWidget* windowMenu;


GtkWidget* pidLabel;
GtkWidget* archivoLabel;

GtkWidget* semillaEntry;
GtkWidget* processSpinButton;
GtkWidget* operationSpinButton;
GtkWidget* cargarButton;
GtkWidget* iniciarSimulacionButton;

GtkWidget* fifoCheck;
GtkWidget* scCheck;
GtkWidget* mruCheck;
GtkWidget* rndCheck;

GtkGrid* selectAlgorithmTable;

//variables del menu
int numProcesos;
int numOperaciones;
int algoritmoSeleccionado;
int semilla;

//widgets de la simulacion
GtkWidget* windowSimulacion;

GtkWidget* pausarSimulacionButton;

GtkWidget* optRam;
GtkWidget* algRam;

GtkGrid* optTable;
GtkGrid* algTable;

//ESTADISTICAS OPTIMO
GtkWidget* processOPT;
GtkWidget* timeOPT;
GtkWidget* ramKbOPT;
GtkWidget* ramPOPT;
GtkWidget* VRamOPT;
GtkWidget* vRamPOPT;
GtkWidget* loadedOPT;
GtkWidget* unloadedOPT;
GtkWidget* tarshIOPT;
GtkWidget* trashDOPT;
GtkWidget* fragOPT;

//ESTADISTICA ALGORITMO
GtkWidget* processALG;
GtkWidget* timeALG;
GtkWidget* ramKbALG;
GtkWidget* ramPALG;
GtkWidget* VRamALG;
GtkWidget* vRamPALG;
GtkWidget* loadedALG;
GtkWidget* unloadedALG;
GtkWidget* tarshIALG;
GtkWidget* trashDALG;
GtkWidget* fragALG;

void estadisticasAlg(){
    char processAlgText[20];
    char timeAlgText[20];
    char ramKbAlgText[20];
    char ramPAlgText[20];
    char VRamAlgText[20];
    char VRamPAlgText[20];
    char loadedAlgText[20];
    char unloadedAlgText[20];
    char trashIAlgText[20];
    char trashDAlgText[20];
    char fragAlgText[20];


    sprintf(processAlgText, "%d", procesos);
    sprintf(timeAlgText, "%d", tiempoAlg);
    sprintf(ramKbAlgText, "%d", ramKBAlg);
    sprintf(ramPAlgText, "%d", porcentajeAlg);
    sprintf(VRamAlgText, "%d", virtualKBAlg);
    sprintf(VRamPAlgText, "%d", porcentajeVirtualAlg);
    sprintf(loadedAlgText, "%d", paginasCargadaAlg);
    sprintf(unloadedAlgText, "%d", paginasFueraAlg);
    sprintf(trashIAlgText, "%d", trashingAlg);

    printf(processAlgText);

    gtk_label_set_text(GTK_LABEL(processALG), processAlgText);
    gtk_label_set_text(GTK_LABEL(timeALG), timeAlgText);
    gtk_label_set_text(GTK_LABEL(ramKbALG), ramKbAlgText);
    gtk_label_set_text(GTK_LABEL(ramPALG), ramPAlgText);
    gtk_label_set_text(GTK_LABEL(VRamALG), VRamAlgText);
    gtk_label_set_text(GTK_LABEL(vRamPALG), VRamPAlgText);
    gtk_label_set_text(GTK_LABEL(loadedALG), loadedAlgText);
    gtk_label_set_text(GTK_LABEL(unloadedALG), unloadedAlgText);
    gtk_label_set_text(GTK_LABEL(tarshIALG), trashIAlgText);
    
    
}

void estadisticasOpt(){
    char processOptText[20];
    char timeOptText[20];
    char ramKbOptText[20];
    char ramPOptText[20];
    char VRamOptText[20];
    char VRamPOptText[20];
    char loadedOptText[20];
    char unloadedOptText[20];
    char trashIOptText[20];
    char trashDOptText[20];
    char fragOptText[20];


    sprintf(processOptText, "%d", procesos);
    sprintf(timeOptText, "%d", tiempoOPT);
    sprintf(ramKbOptText, "%d", ramKBOPT);
    sprintf(ramPOptText, "%d", porcentajeOPT);
    sprintf(VRamOptText, "%d", virtualKBOPT);
    sprintf(VRamPOptText, "%d", porcentajeVirtualOPT);
    sprintf(loadedOptText, "%d", paginasCargadaOPT);
    sprintf(unloadedOptText, "%d", paginasFueraOPT);
    sprintf(trashIOptText, "%d", trashingOpt);

    gtk_label_set_text(GTK_LABEL(processOPT), processOptText);
    gtk_label_set_text(GTK_LABEL(timeOPT), timeOptText);
    gtk_label_set_text(GTK_LABEL(ramKbOPT), ramKbOptText);
    gtk_label_set_text(GTK_LABEL(ramPOPT), ramPOptText);
    gtk_label_set_text(GTK_LABEL(VRamOPT), VRamOptText);
    gtk_label_set_text(GTK_LABEL(vRamPOPT), VRamPOptText);
    gtk_label_set_text(GTK_LABEL(loadedOPT), loadedOptText);
    gtk_label_set_text(GTK_LABEL(unloadedOPT), unloadedOptText);
    gtk_label_set_text(GTK_LABEL(tarshIOPT), trashIOptText);
    

}

// // gboolean blockCreateTableOpt = FALSE;
// gboolean cleanTable3(gpointer data){ //limpia la tabla del display de la respuesta final
//     // printf("Clean table %d\n", tablaPaginasOPT.size);
// 	for (int i = 0; i < tablaPaginasOPT.size; i++) {
// 		gtk_grid_remove_column(optTable, 0);
//     }
//     return G_SOURCE_REMOVE; 
// }

// Función para limpiar una tabla de GTK sin destruir widgets
gboolean limpiarTabla(gpointer data) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(optTable));

    // Recorre la lista de widgets y quita cada uno de la tabla
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_container_remove(GTK_CONTAINER(optTable), GTK_WIDGET(iter->data));
    }

    g_list_free(children); // Libera la lista
    return G_SOURCE_REMOVE; 
}




gboolean createTableOpt(gpointer data){
    // cleanTable3();
    // g_usleep(1000000); 

    // printf("create table %d\n", tablaPaginasOPT.size);
    for(int i = 0; i < tablaPaginasOPT.size; i++){

        //Columna Page ID
        char buffer[10];
        sprintf(buffer, "%d", tablaPaginasOPT.data[i][0]);
        GtkWidget *labelPageID = gtk_label_new(buffer);
        gtk_widget_set_name(labelPageID, "neutro");
        gtk_grid_attach(GTK_GRID(optTable), labelPageID, 0, i, 1, 1);

        //Columna PID
        int procesoID = getProcessElementPtr(&tablaPunteros, tablaPaginasOPT.data[i][1]);
        sprintf(buffer, "%d", procesoID);
        GtkWidget *labelPID = gtk_label_new(buffer);
        gtk_widget_set_name(labelPID, "neutro");
        gtk_grid_attach(GTK_GRID(optTable), labelPID, 1, i, 1, 1);

        //Columna Loaded
        sprintf(buffer, "%d", tablaPaginasOPT.data[i][2]);
        if(tablaPaginasOPT.data[i][2] == 1){
            // muestra una X si esta cargada
            GtkWidget *labelLoaded = gtk_label_new("X");
            gtk_widget_set_name(labelLoaded, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelLoaded, 2, i, 1, 1);
            
            //Columna L-ADDR
            sprintf(buffer, "%d", tablaPaginasOPT.data[i][1]);
            GtkWidget *labelL_ADDR = gtk_label_new(buffer);
            gtk_widget_set_name(labelL_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelL_ADDR, 3, i, 1, 1);

            //Columna M-ADDR
            sprintf(buffer, "%d", tablaPaginasOPT.data[i][3]);
            GtkWidget *labelM_ADDR = gtk_label_new(buffer);
            gtk_widget_set_name(labelM_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelM_ADDR, 4, i, 1, 1);

            //Columna D-ADDR
            
            GtkWidget *labelD_ADDR = gtk_label_new("");
            gtk_widget_set_name(labelD_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelD_ADDR, 5, i, 1, 1);
            
            //Columna LOADED-T
            sprintf(buffer, "%d", tablaPaginasOPT.data[i][4]);
            GtkWidget *labelLoaded_T = gtk_label_new(buffer);
            gtk_widget_set_name(labelLoaded_T, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelLoaded_T, 6, i, 1, 1);

        }else{
            // muestra una X si esta cargada
            GtkWidget *labelLoaded = gtk_label_new("");
            gtk_widget_set_name(labelLoaded, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelLoaded, 2, i, 1, 1);
            
            //Columna L-ADDR
            sprintf(buffer, "%d", tablaPaginasOPT.data[i][1]);
            GtkWidget *labelL_ADDR = gtk_label_new(buffer);
            gtk_widget_set_name(labelL_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelL_ADDR, 3, i, 1, 1);

            //Columna M-ADDR
            GtkWidget *labelM_ADDR = gtk_label_new("");
            gtk_widget_set_name(labelM_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelM_ADDR, 4, i, 1, 1);

            //Columna D-ADDR
            sprintf(buffer, "%d", tablaPaginasOPT.data[i][3]);
            GtkWidget *labelD_ADDR = gtk_label_new(buffer);
            gtk_widget_set_name(labelD_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelD_ADDR, 5, i, 1, 1);
            
            //Columna LOADED-T
            GtkWidget *labelLoaded_T = gtk_label_new("");
            gtk_widget_set_name(labelLoaded_T, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelLoaded_T, 6, i, 1, 1);
        }

        //Columna MARK
        GtkWidget *labelMark = gtk_label_new("");
        gtk_widget_set_name(labelMark, "neutro");
        gtk_grid_attach(GTK_GRID(optTable), labelMark, 7, i, 1, 1);

    }
    // gtk_widget_show_all(optTable); 
    return G_SOURCE_REMOVE; 
}

gboolean mi_funcion_actualizar_interfaz(gpointer data) {
    // Actualiza la interfaz de usuario aquí
    gtk_widget_show_all(windowSimulacion); 
    return G_SOURCE_REMOVE;  // Indica que esta función se debe eliminar después de ejecutarse
}

void actualizarEstadisticas(){
    procesos = listaProcesos.longitud; // cantidad de procesos
        
    ramKBOPT = (RamAlg.capacidad - RamOPT.cantidadDatos)*4; // cantidad de KB en la RAM OPT
    ramKBAlg = (RamAlg.capacidad - RamAlg.cantidadDatos)*4; // cantidad de KB en la RAM de los demas algoritmos
    
    porcentajeOPT = ((double)ramKBOPT/(RamOPT.capacidad*4))*100; // porcentaje de la RAM OPT
    porcentajeAlg = ((double)ramKBAlg/(RamAlg.capacidad*4))*100; // porcentaje de la RAM de los demas algoritmos
    
    virtualKBOPT = HDD1.capacidad - HDD1.cantidadDatos; // cantidad de KB en la memoria virtual OPT
    virtualKBAlg =  HDD2.capacidad - HDD2.cantidadDatos; // cantidad de KB en la memoria virtual de los demas algoritmos
    
    porcentajeVirtualOPT = ((double)virtualKBOPT/RamOPT.capacidad)*100; // porcentaje de la memoria virtual OPT
    porcentajeVirtualAlg = ((double)virtualKBAlg/RamAlg.capacidad)*100; // porcentaje de la memoria virtual de los demas algoritmos
    
    contarPaginas();
}
//Back

void *prueba5(void *data){
    FILE *archivo;
    regex_t regexNew, regexUse, regexDelete, regexKill;
    char *nombreArchivo = "simulation.txt";
    archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL){
        printf("Error al abrir el archivo");
        return NULL;
    }

    int status;
    status = regcomp(&regexNew, "new\\([[:digit:]]+,[[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        // Manejar errores de compilación de la expresión regular
        return NULL;
    }

    status = regcomp(&regexUse, "use\\([[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        // Manejar errores de compilación de la expresión regular
        return NULL;
    }

    status = regcomp(&regexDelete, "delete\\([[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        // Manejar errores de compilación de la expresión regular
        return NULL;
    }

    status = regcomp(&regexKill, "kill\\([[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        // Manejar errores de compilación de la expresión regular
        return NULL;
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
    actualizarEstadisticas();
    estadisticasAlg();
    estadisticasOpt(); 
    
    archivo = fopen(nombreArchivo, "r");
    while(fgets(linea, sizeof(linea),archivo) != NULL){
        
        status = regexec(&regexNew, linea, 0, NULL, 0);
        if (status == 0) {
            g_idle_add(limpiarTabla, NULL);
            printf("%s", linea); // se hace operacion
            operacionNew(linea);
            g_idle_add(createTableOpt, NULL);
            g_idle_add(mi_funcion_actualizar_interfaz, NULL); 

        }

        status = regexec(&regexUse, linea, 0, NULL, 0);
        if (status == 0) {
            g_idle_add(limpiarTabla, NULL);
            printf("%s", linea); // se hace operacion
            operacionUse(linea);
            g_idle_add(createTableOpt, NULL);
            g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
        }

        status = regexec(&regexDelete, linea, 0, NULL, 0);
        if (status == 0) {
            g_idle_add(limpiarTabla, NULL);
            printf("%s", linea); // se hace operacion
            operacionDelete(linea);
            g_idle_add(createTableOpt, NULL);
            g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
        }

        status = regexec(&regexKill, linea, 0, NULL, 0);
        if (status == 0) {
            g_idle_add(limpiarTabla, NULL);
            printf("%s", linea); // se hace operacion
            operacionKill(linea);
            g_idle_add(createTableOpt, NULL);
            g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
        }

        g_usleep(1000000);

        actualizarEstadisticas();
        estadisticasAlg();
        estadisticasOpt(); 
        // g_idle_add(createTableOpt, NULL);
        // imprimirRAM(&RamOPT);
        // imprimirVirtual(&HDD1);
        
        g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
    }
    actualizarEstadisticas();
    estadisticasAlg();
    estadisticasOpt();
    // g_idle_add(cleanTable3, NULL);
    // g_idle_add(createTableOpt, NULL);
    // g_usleep(3000000); 
    g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
    printf("Tiempo opt: %d\n", tiempoOPT);
    printf("Tiempo alg: %d\n", tiempoAlg);
    printf("Trashing opt: %d\n", trashingOpt);
    printf("Trashing alg: %d\n", trashingAlg);
    //liberar memoria
    regfree(&regexDelete);
    regfree(&regexKill);
    regfree(&regexUse);
    regfree(&regexNew);
    fclose(archivo);
    return NULL;
}

// gboolean runSimulationInBackground(gpointer data) {
//     prueba5();
//     return G_SOURCE_REMOVE; // Indicar que el hilo ha terminado y puede ser removido
// }

void iniciarSimulacionEnHilo() {
    // Crear un hilo en segundo plano para ejecutar la simulación
    // GThread* thread = g_thread_new("SimulacionThread", runSimulationInBackground, NULL);
    // g_thread_unref(thread); // Liberar la referencia al hilo para que se autodestruya cuando termine
    g_thread_new(NULL, prueba5, NULL);
}

//css de toda la vida
static void load_css(void){
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen * screen;
    
    const gchar *css_style_file = "styles.css";
    
    GFile *css_fp = g_file_new_for_path(css_style_file);
    
    GError * error = 0;
    
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    
    gtk_style_context_add_provider_for_screen(screen,GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file(provider,css_fp,&error);
    g_object_unref(provider);
}

void actualizarCantProcesos( GtkWidget *widget, GdkEventButton *event, gpointer object )
{
    numProcesos = gtk_spin_button_get_value_as_int((GtkSpinButton*) processSpinButton);
    g_print("Cantidad de Procesos: %d\n", numProcesos);
}

void actualizarCantOperaciones( GtkWidget *widget, GdkEventButton *event, gpointer object )
{
    numOperaciones = gtk_spin_button_get_value_as_int((GtkSpinButton*) operationSpinButton);
    g_print("Cantidad de operaciones: %d\n", numOperaciones);
}

void actualizarSemilla(GtkEntry *entry, gpointer user_data) {
    // Obtener el texto del GtkEntry
    const gchar *semillaTexto = gtk_entry_get_text(entry);
    
    // Validar si el texto es un número entero
    char *endptr;
    long semillaP = strtol(semillaTexto, &endptr, 10);

    // Si el texto es un número entero válido, hacer algo con el valor (por ejemplo, almacenarlo en una variable global)
    if (*endptr == '\0') {
        semilla = semillaP;
        g_print("Semilla: %d\n", semilla);
    } else {
        // Si el texto no es un número entero válido, puedes mostrar un mensaje de error o manejarlo según tus necesidades
        g_print("Error: Semilla no válida\n");
    }
}


void selectAlgorithm(GtkWidget *widget, gpointer data) {
    // Obtener los estados de los checkboxes y sus identificadores
    gboolean fifoChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(fifoCheck));
    gboolean scChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(scCheck));
    gboolean mruChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(mruCheck));
    gboolean rndChecked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(rndCheck));

    // Desmarcar los checkboxes que no están seleccionados
    if (fifoChecked) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(scCheck), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mruCheck), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rndCheck), FALSE);
    } else if (scChecked) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fifoCheck), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mruCheck), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rndCheck), FALSE);
    } else if (mruChecked) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fifoCheck), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(scCheck), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rndCheck), FALSE);
    } else if (rndChecked) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(fifoCheck), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(scCheck), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mruCheck), FALSE);
    }

    // Almacenar el valor correspondiente
    if (fifoChecked) {
        algoritmoSeleccionado = 1;
    } else if (scChecked) {
        algoritmoSeleccionado = 2;
    } else if (mruChecked) {
        algoritmoSeleccionado = 3;
    } else if (rndChecked) {
        algoritmoSeleccionado = 4;
    }


    g_print("Algoritmo seleccionado: %d\n", algoritmoSeleccionado);
}
void simulacion(){
    iniciarSimulacionEnHilo();
}

void init_simulacion(){
    // Se crea una structura que guarda los datos de la simulacion
    struct start preparacion = crearPreparación(semilla, algoritmoSeleccionado, NULL, numProcesos, numOperaciones);
    printf("Se ha creado el archivo de simulacion semilla: %d, option: %d, numero de proceso: %d, numero de Operaciones: %d\n", preparacion.seed, preparacion.option, preparacion.NumberProcess, preparacion.amoutOperations);
    srand(preparacion.seed); // Semilla para generar numeros aleatorios
    struct Matrix mat = createMatrix(preparacion.NumberProcess, 2); // Se crea la matriz de los punteros y procesos
    createSimulationTxt(preparacion.NumberProcess, preparacion.amoutOperations, &mat); // Se crea el archivo de simulacion
    preparacion.file = fopen("simulation.txt", "r"); // Se abre el archivo de simulacion
    if(preparacion.file == NULL){
        printf("Error al abrir el archivo");
        return;
    }
    freeMatrix(&mat); // Se libera la memoria de la matriz

    
    //se activa la ventana de simulacion
    gtk_widget_show(windowSimulacion);
    simulacion();

    // FILE *archivo;
    // regex_t regexNew, regexUse, regexDelete, regexKill;
    // char *nombreArchivo = "simulation.txt";
    // archivo = fopen(nombreArchivo, "r");

    // if(archivo == NULL){
    //     printf("Error al abrir el archivo");
    //     return;
    // }

    // int status;
    // status = regcomp(&regexNew, "new\\([[:digit:]]+,[[:digit:]]+\\)", REG_EXTENDED);
    // if (status != 0) {
    //     // Manejar errores de compilación de la expresión regular
    //     return;
    // }

    // status = regcomp(&regexUse, "use\\([[:digit:]]+\\)", REG_EXTENDED);
    // if (status != 0) {
    //     // Manejar errores de compilación de la expresión regular
    //     return;
    // }

    // status = regcomp(&regexDelete, "delete\\([[:digit:]]+\\)", REG_EXTENDED);
    // if (status != 0) {
    //     // Manejar errores de compilación de la expresión regular
    //     return;
    // }

    // status = regcomp(&regexKill, "kill\\([[:digit:]]+\\)", REG_EXTENDED);
    // if (status != 0) {
    //     // Manejar errores de compilación de la expresión regular
    //     return;
    // }

    // char linea[100];
    // printf("Se encontraron las siguientes operaciones:\n");
    // inicializarRAM(&RamOPT, 20);
    // inicializarRAM(&RamAlg, 20);
    // int numeroProcesos = 10;
    // inicializarLista(&listaProcesos, numeroProcesos); // Inicializar lista de procesos
    // tablaPunteros = createMatrix(numeroProcesos, 3); // crear tabla de punteros
    // tablaPaginasOPT = createMatrix(5, 6); //crear tabla de paginas OPT
    // tablaPaginasAlg = createMatrix(5, 6); //crear tabla de paginas demas algoritmos
    // inicializarVirtual(&HDD1, 4); // Memoria virtual para el OPT
    // inicializarVirtual(&HDD2, 4); // Memoria virtual para los demas algoritmos
    // inicializarLista(&futuroOPT, 10); // lista a futuro de procesos
    
    // while(fgets(linea, sizeof(linea),archivo) != NULL){
    //     status = regexec(&regexUse, linea, 0, NULL, 0);
    //     if (status == 0) {
    //         int ptr = 0;
    //         obtenerNumeroOper(&ptr, linea);
    //         agregarElemento(&futuroOPT, ptr);
    //     }

    // }
    // // imprimirLista(&futuroOPT);
    // fclose(archivo);

    // archivo = fopen(nombreArchivo, "r");
    // while(fgets(linea, sizeof(linea),archivo) != NULL){
        
    //     procesos = listaProcesos.longitud; // cantidad de procesos
        
    //     ramKBOPT = (RamAlg.capacidad - RamOPT.cantidadDatos)*4; // cantidad de KB en la RAM OPT
    //     ramKBAlg = (RamAlg.capacidad - RamAlg.cantidadDatos)*4; // cantidad de KB en la RAM de los demas algoritmos
        
    //     porcentajeOPT = ((double)ramKBOPT/(RamOPT.capacidad*4))*100; // porcentaje de la RAM OPT
    //     porcentajeAlg = ((double)ramKBAlg/(RamAlg.capacidad*4))*100; // porcentaje de la RAM de los demas algoritmos
        
    //     virtualKBOPT = HDD1.capacidad - HDD1.cantidadDatos; // cantidad de KB en la memoria virtual OPT
    //     virtualKBAlg =  HDD2.capacidad - HDD2.cantidadDatos; // cantidad de KB en la memoria virtual de los demas algoritmos
        
    //     porcentajeVirtualOPT = ((double)virtualKBOPT/RamOPT.capacidad)*100; // porcentaje de la memoria virtual OPT
    //     porcentajeVirtualAlg = ((double)virtualKBAlg/RamAlg.capacidad)*100; // porcentaje de la memoria virtual de los demas algoritmos
        
    //     contarPaginas();
        
    //     status = regexec(&regexNew, linea, 0, NULL, 0);
    //     if (status == 0) {
    //         printf("%s", linea); // se hace operacion
    //         operacionNew(linea);
    //         createTableOpt();
    //     }

    //     status = regexec(&regexUse, linea, 0, NULL, 0);
    //     if (status == 0) {
    //         cleanTable3();
    //         printf("%s", linea); // se hace operacion
    //         operacionUse(linea);
    //         createTableOpt(); 
    //     }

    //     status = regexec(&regexDelete, linea, 0, NULL, 0);
    //     if (status == 0) {
    //         cleanTable3();
    //         printf("%s", linea); // se hace operacion
    //         operacionDelete(linea);
    //         createTableOpt(); 
    //     }

    //     status = regexec(&regexKill, linea, 0, NULL, 0);
    //     if (status == 0) {
    //         cleanTable3();
    //         printf("%s", linea); // se hace operacion
    //         operacionKill(linea); 
    //         createTableOpt();
    //         break;
    //     }

    //     sleep(2);
    //     estadisticasAlg();
    //     estadisticasOpt();
    //     gtk_widget_show_all(windowSimulacion); 

    // }
    // gtk_widget_show_all(windowSimulacion); 

    // printf("Tiempo opt: %d\n", tiempoOPT);
    // printf("Tiempo alg: %d\n", tiempoAlg);
    // printf("Trashing opt: %d\n", trashingOpt);
    // printf("Trashing alg: %d\n", trashingAlg);
    // //liberar memoria
    // regfree(&regexDelete);
    // regfree(&regexKill);
    // regfree(&regexUse);
    // regfree(&regexNew);
    // fclose(archivo);
    liberarMemoriaPrograma();
}

int main(int argc, char *argv[]){
    GtkBuilder *builder; //GTK builder
    gtk_init(&argc, &argv); //start gtk


    builder = gtk_builder_new(); //create gtk ui builder
    gtk_builder_add_from_file(builder, "mmuSimulation.glade", NULL); //LOAD UI FILE
    
    load_css();
    //ventana
    windowMenu = GTK_WIDGET(gtk_builder_get_object(builder, "window_menu")); //load window named MyWindow
    windowSimulacion = GTK_WIDGET(gtk_builder_get_object(builder, "window_simulacion")); //load window named MyWindow
    //entry
    semillaEntry = GTK_WIDGET(gtk_builder_get_object(builder, "semilla_entry")); 

    //label
    //estadisticas opt
    processOPT = GTK_WIDGET(gtk_builder_get_object(builder, "cant_processes_opt")); 
    timeOPT = GTK_WIDGET(gtk_builder_get_object(builder, "time_opt")); 
    ramKbOPT = GTK_WIDGET(gtk_builder_get_object(builder, "ram_kb_opt")); 
    ramPOPT = GTK_WIDGET(gtk_builder_get_object(builder, "ram_p_opt")); 
    VRamOPT = GTK_WIDGET(gtk_builder_get_object(builder, "ram_v_opt")); 
    vRamPOPT = GTK_WIDGET(gtk_builder_get_object(builder, "ram_p_v_opt")); 
    loadedOPT = GTK_WIDGET(gtk_builder_get_object(builder, "loaded_opt")); 
    unloadedOPT = GTK_WIDGET(gtk_builder_get_object(builder, "unloaded_opt")); 
    tarshIOPT = GTK_WIDGET(gtk_builder_get_object(builder, "trash_izq_opt")); 
    trashDOPT = GTK_WIDGET(gtk_builder_get_object(builder, "trash_der_opt")); 
    fragOPT = GTK_WIDGET(gtk_builder_get_object(builder, "frag_opt")); 

    //estadostocas alg
    processALG = GTK_WIDGET(gtk_builder_get_object(builder, "cant_processes_alg")); 
    timeALG = GTK_WIDGET(gtk_builder_get_object(builder, "time_alg")); 
    ramKbALG = GTK_WIDGET(gtk_builder_get_object(builder, "ram_kb_alg")); 
    ramPALG = GTK_WIDGET(gtk_builder_get_object(builder, "ram_p_alg")); 
    VRamALG = GTK_WIDGET(gtk_builder_get_object(builder, "ram_v_alg")); 
    vRamPALG = GTK_WIDGET(gtk_builder_get_object(builder, "ram_p_v_alg")); 
    loadedALG = GTK_WIDGET(gtk_builder_get_object(builder, "loaded_alg")); 
    unloadedALG = GTK_WIDGET(gtk_builder_get_object(builder, "unloaded_alg")); 
    tarshIALG = GTK_WIDGET(gtk_builder_get_object(builder, "trash_izq_alg")); 
    trashDALG = GTK_WIDGET(gtk_builder_get_object(builder, "trash_der_alg")); 
    fragALG = GTK_WIDGET(gtk_builder_get_object(builder, "frag_alg")); 

    //botones
    processSpinButton = GTK_WIDGET(gtk_builder_get_object(builder, "num_Proceso")); 
	operationSpinButton = GTK_WIDGET(gtk_builder_get_object(builder, "num_operaciones"));
    cargarButton = GTK_WIDGET(gtk_builder_get_object(builder, "cargar_archivo_menu"));
    iniciarSimulacionButton = GTK_WIDGET(gtk_builder_get_object(builder, "init"));

    fifoCheck = GTK_WIDGET(gtk_builder_get_object(builder, "fifo_check")); 
	scCheck = GTK_WIDGET(gtk_builder_get_object(builder, "sc_check"));
    mruCheck = GTK_WIDGET(gtk_builder_get_object(builder, "mru_check"));
    rndCheck = GTK_WIDGET(gtk_builder_get_object(builder, "rnd_check"));

    
    //GRIDS
    selectAlgorithmTable = GTK_GRID(gtk_builder_get_object(builder, "select_algorithm"));
    optTable = GTK_GRID(gtk_builder_get_object(builder, "opt_table"));

    algTable = GTK_GRID(gtk_builder_get_object(builder, "alg_table"));


    //ASIGN VARIABLES


    //connect signals
    g_signal_connect( processSpinButton, "value-changed", G_CALLBACK(actualizarCantProcesos), NULL );
    g_signal_connect( operationSpinButton, "value-changed", G_CALLBACK(actualizarCantOperaciones), NULL );

    g_signal_connect( fifoCheck, "toggled", G_CALLBACK(selectAlgorithm), NULL );
    g_signal_connect( scCheck, "toggled", G_CALLBACK(selectAlgorithm), NULL );
    g_signal_connect( mruCheck, "toggled", G_CALLBACK(selectAlgorithm), NULL );
    g_signal_connect( rndCheck, "toggled", G_CALLBACK(selectAlgorithm), NULL );

    g_signal_connect(semillaEntry, "changed", G_CALLBACK(actualizarSemilla), NULL);

    g_signal_connect( iniciarSimulacionButton, "clicked", G_CALLBACK(init_simulacion), NULL );


    gtk_builder_connect_signals(builder, NULL);
    

    g_object_unref(builder);
    
    g_signal_connect(windowMenu, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(windowMenu); //show window
    //gtk_widget_show_all(windowSimulacion); 
    gtk_main(); //run

    return 0;
}



//PARA MANEJO DE ARCHIVOS; por definir
/*void cargar_estado(){
	char *filename;
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(FileChooserWindow));
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(filename, "r");
    if (fp == NULL){
        exit(EXIT_FAILURE);
	}
	int n = -1;
    while ((read = getline(&line, &len, fp)) != -1) {
		char*arrays = strtok(line, " ");
		int j = 0;
		while (arrays != NULL){
			if(n==-1){
			    games = atoi(arrays);
    			probGanarCasa = s_to_f(strtok(NULL, " "));
                probGanarVisita = s_to_f(strtok(NULL, " "));
				break;
			}
			juegoEnCasa[j+1] = atoi(arrays);
			arrays = strtok(NULL, " ");
			j++;
		}
		n++;
    }
	cargar();
    fclose(fp);
    if(line){
        free(line);
	}
	gtk_widget_set_visible(FileChooserWindow, false);
}

void open_fileChooser(GtkWidget* widget, gpointer data)
{
	filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.std");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(FileChooserWindow), filter);
    gtk_dialog_run (GTK_DIALOG (FileChooserWindow));
}*/
