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


GtkCssProvider *provider;
GFile *css_fp;
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

int n = 0;

//widgets de la simulacion
GtkWidget* windowSimulacion;

GtkWidget* pausarSimulacionButton;
GtkWidget* reanudarSimulacionButton;
GtkWidget* detenerSimulacionButton;

GtkWidget* labelRamAlg;
GtkWidget* labelMMUAlg;

GtkGrid* optRam;
GtkGrid* algRam;

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
GtkWidget* trashTitleALG;
//ESTADISTICA ALGORITMO
GtkWidget* processALG;
GtkWidget* timeALG;
GtkWidget* ramKbALG;
GtkWidget* ramPALG;
GtkWidget* VRamALG;
GtkWidget* vRamPALG;
GtkWidget* loadedALG;
GtkWidget* unloadedALG;
GtkWidget* trashTitleOPT;
GtkWidget* tarshIALG;
GtkWidget* trashDALG;
GtkWidget* fragALG;

//FILE CHOOSER THINGYS
GtkWidget* fileChooser;
GtkWidget* fileButton;
GtkWidget* exitChooserButton;

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
    sprintf(trashDAlgText, "%d", porcentajeTrashingAlg);
    sprintf(fragAlgText, "%d", fragmentacionInternaAlg);


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
    gtk_label_set_text(GTK_LABEL(trashDALG), trashDAlgText);
    gtk_label_set_text(GTK_LABEL(fragALG), fragAlgText);
    
    
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
    sprintf(trashDOptText, "%d", porcentajeTrashingOpt);
    sprintf(fragOptText, "%d", fragmentacionInternaOPT);

    gtk_label_set_text(GTK_LABEL(processOPT), processOptText);
    gtk_label_set_text(GTK_LABEL(timeOPT), timeOptText);
    gtk_label_set_text(GTK_LABEL(ramKbOPT), ramKbOptText);
    gtk_label_set_text(GTK_LABEL(ramPOPT), ramPOptText);
    gtk_label_set_text(GTK_LABEL(VRamOPT), VRamOptText);
    gtk_label_set_text(GTK_LABEL(vRamPOPT), VRamPOptText);
    gtk_label_set_text(GTK_LABEL(loadedOPT), loadedOptText);
    gtk_label_set_text(GTK_LABEL(unloadedOPT), unloadedOptText);
    gtk_label_set_text(GTK_LABEL(tarshIOPT), trashIOptText);
    gtk_label_set_text(GTK_LABEL(trashDOPT), trashDOptText);
    gtk_label_set_text(GTK_LABEL(fragOPT), fragOptText);
    

}

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

gboolean limpiarTabla2(gpointer data) {
    GList *children = gtk_container_get_children(GTK_CONTAINER(algTable));

    // Recorre la lista de widgets y quita cada uno de la tabla
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_container_remove(GTK_CONTAINER(algTable), GTK_WIDGET(iter->data));
    }

    g_list_free(children); // Libera la lista
    return G_SOURCE_REMOVE; 
}

int keysPid[150];
GdkRGBA valuesColor[150];
int sizePid = 0;

void addKeyValue(int key, GdkRGBA value) {
    keysPid[sizePid] = key;
    valuesColor[sizePid] = value;
    sizePid++;
}

GdkRGBA getValueByKey(int key) {
    for (int i = 0; i < sizePid; i++) {
        if (keysPid[i] == key) {
            return valuesColor[i];
        }
    }
    
    GdkRGBA defaultColor = {0, 0, 0, 1}; 
    return defaultColor;
}

bool isKeyInDic(int key) {
    for (int i = 0; i < sizePid; i++) {
        if (keysPid[i] == key) {
            return true; 
        }
    }
    return false; 
}

void set_widget_color(GtkWidget *widget, const gchar *css_class, const gchar *color) {
    GtkCssProvider *provider = gtk_css_provider_new();
    gchar *css_data = g_strdup_printf("#%s { background: %s; }", css_class, color);

    gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
    g_free(css_data);

    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}


gboolean setRamAlg(){
    gtk_grid_remove_row(algRam, 0);
    for(int i = 0; i < RamAlg.capacidad; i++){
        GtkWidget *espacio = gtk_label_new(" ");
        gtk_widget_set_name(espacio, "neutro");
        GdkRGBA rgbaColor = {0, 0, 0, 1}; 
        int pid = RamAlg.datos[i];

        if(pid != -1){
            rgbaColor = getValueByKey(pid);
        }


        //el color de fondo en formato hexadecimal
        int red = (int)(rgbaColor.red * 255);
        int green = (int)(rgbaColor.green * 255);
        int blue = (int)(rgbaColor.blue * 255);


        gchar hexColor[8] = " "; 
        g_snprintf(hexColor, sizeof(hexColor), "#%02X%02X%02X", red, green, blue);

        set_widget_color(espacio, "neutro", hexColor);
        gtk_grid_attach(GTK_GRID(algRam), espacio, i, 0, 1, 1);
    }
    return G_SOURCE_REMOVE; 
}

gboolean setRamOpt(){
    gtk_grid_remove_row(optRam, 0);
    for(int i = 0; i < RamOPT.capacidad; i++){
        GtkWidget *espacio = gtk_label_new(" ");
        gtk_widget_set_name(espacio, "neutro");
        GdkRGBA rgbaColor = {0, 0, 0, 1}; 
        int pid = RamOPT.datos[i];

        if(pid != -1){
            rgbaColor = getValueByKey(pid);
        }


        //el color de fondo en formato hexadecimal
        int red = (int)(rgbaColor.red * 255);
        int green = (int)(rgbaColor.green * 255);
        int blue = (int)(rgbaColor.blue * 255);


        gchar hexColor[8] = " "; 
        g_snprintf(hexColor, sizeof(hexColor), "#%02X%02X%02X", red, green, blue);

        set_widget_color(espacio, "neutro", hexColor);
        gtk_grid_attach(GTK_GRID(optRam), espacio, i, 0, 1, 1);

    }
    return G_SOURCE_REMOVE; 
}

gboolean createTableAlg(gpointer data){

    for(int i = 0; i < tablaPaginasAlg.size; i++){
        //Columna Page ID
        char buffer[10];
        sprintf(buffer, "%d", tablaPaginasAlg.data[i][0]);
        GtkWidget *labelPageID = gtk_label_new(buffer);
        gtk_widget_set_name(labelPageID, "neutro");
        gtk_grid_attach(GTK_GRID(algTable), labelPageID, 0, i, 1, 1);

        //Columna PID
        int procesoID = getProcessElementPtr(&tablaPunteros, tablaPaginasAlg.data[i][1]);
        if(!isKeyInDic(procesoID)) {
            GdkRGBA color;
            // Generar un color aleatorio 
            color.red = g_random_double_range(50, 256);  
            color.green = g_random_double_range(50, 256); 
            color.blue = g_random_double_range(50, 256);  
            color.alpha = g_random_double_range(1, 256) / 255.0; 
            addKeyValue(procesoID, color);
        }

        GdkRGBA rgbaColor = getValueByKey(procesoID);

        //el color de fondo en formato hexadecimal
        int red = (int)(rgbaColor.red * 255);
        int green = (int)(rgbaColor.green * 255);
        int blue = (int)(rgbaColor.blue * 255);


        gchar hexColor[8] = " "; 
        g_snprintf(hexColor, sizeof(hexColor), "#%02X%02X%02X", red, green, blue);

        //printf("Proceso ID: %d, Hex Color: %s\n", procesoID, hexColor);

        set_widget_color(labelPageID, "neutro", hexColor);
        gtk_widget_set_name(labelPageID, "neutro");
        


        sprintf(buffer, "%d", procesoID);
        GtkWidget *labelPID = gtk_label_new(buffer);
        set_widget_color(labelPID, "neutro", hexColor);
        gtk_widget_set_name(labelPID, "neutro");
        gtk_grid_attach(GTK_GRID(algTable), labelPID, 1, i, 1, 1);

        //Columna Loaded
        sprintf(buffer, "%d", tablaPaginasAlg.data[i][2]);
        if(tablaPaginasAlg.data[i][2] == 1){
            // muestra una X si esta cargada
            GtkWidget *labelLoaded = gtk_label_new("X");
            set_widget_color(labelLoaded, "neutro", hexColor);
            gtk_widget_set_name(labelLoaded, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelLoaded, 2, i, 1, 1);
            
            //Columna L-ADDR
            sprintf(buffer, "%d", tablaPaginasAlg.data[i][1]);
            GtkWidget *labelL_ADDR = gtk_label_new(buffer);
            set_widget_color(labelL_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelL_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelL_ADDR, 3, i, 1, 1);

            //Columna M-ADDR
            sprintf(buffer, "%d", tablaPaginasAlg.data[i][3]);
            GtkWidget *labelM_ADDR = gtk_label_new(buffer);
            set_widget_color(labelM_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelM_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelM_ADDR, 4, i, 1, 1);

            //Columna D-ADDR
            
            GtkWidget *labelD_ADDR = gtk_label_new("");
            set_widget_color(labelD_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelD_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelD_ADDR, 5, i, 1, 1);
            
            //Columna LOADED-T
            sprintf(buffer, "%d", tablaPaginasAlg.data[i][4]);
            GtkWidget *labelLoaded_T = gtk_label_new(buffer);
            set_widget_color(labelLoaded_T, "neutro", hexColor);
            gtk_widget_set_name(labelLoaded_T, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelLoaded_T, 6, i, 1, 1);

        }else{
            // muestra una X si esta cargada
            GtkWidget *labelLoaded = gtk_label_new("");
            set_widget_color(labelLoaded, "neutro", hexColor);
            gtk_widget_set_name(labelLoaded, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelLoaded, 2, i, 1, 1);
            
            //Columna L-ADDR
            sprintf(buffer, "%d", tablaPaginasAlg.data[i][1]);
            GtkWidget *labelL_ADDR = gtk_label_new(buffer);
            set_widget_color(labelL_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelL_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelL_ADDR, 3, i, 1, 1);

            //Columna M-ADDR
            GtkWidget *labelM_ADDR = gtk_label_new("");
            set_widget_color(labelM_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelM_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelM_ADDR, 4, i, 1, 1);

            //Columna D-ADDR
            sprintf(buffer, "%d", tablaPaginasAlg.data[i][3]);
            GtkWidget *labelD_ADDR = gtk_label_new(buffer);
            set_widget_color(labelD_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelD_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelD_ADDR, 5, i, 1, 1);
            
            //Columna LOADED-T
            GtkWidget *labelLoaded_T = gtk_label_new("");
            set_widget_color(labelLoaded_T, "neutro", hexColor);
            gtk_widget_set_name(labelLoaded_T, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelLoaded_T, 6, i, 1, 1);
        }

        //Columna MARK
        if(algoritmoSeleccionado == 2 || algoritmoSeleccionado == 3){
            sprintf(buffer, "%d", tablaPaginasAlg.data[i][5]);
            GtkWidget *labelMark = gtk_label_new(buffer);
            set_widget_color(labelMark, "neutro", hexColor);
            gtk_widget_set_name(labelMark, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelMark, 7, i, 1, 1);
        } else {
            GtkWidget *labelMark = gtk_label_new("");
            set_widget_color(labelMark, "neutro", hexColor);
            gtk_widget_set_name(labelMark, "neutro");
            gtk_grid_attach(GTK_GRID(algTable), labelMark, 7, i, 1, 1);
        }


    }

    return G_SOURCE_REMOVE; 
}

gboolean createTableOpt(gpointer data){

    for(int i = 0; i < tablaPaginasOPT.size; i++){

        //Columna Page ID
        char buffer[10];
        sprintf(buffer, "%d", tablaPaginasOPT.data[i][0]);
        GtkWidget *labelPageID = gtk_label_new(buffer);
        gtk_widget_set_name(labelPageID, "neutro");
        gtk_grid_attach(GTK_GRID(optTable), labelPageID, 0, i, 1, 1);

        //Columna PID
        int procesoID = getProcessElementPtr(&tablaPunteros, tablaPaginasOPT.data[i][1]);

        if(!isKeyInDic(procesoID)) {
            GdkRGBA color;

            color.red = g_random_double_range(50, 256);   
            color.green = g_random_double_range(50, 256);
            color.blue = g_random_double_range(50, 256);  
            color.alpha = g_random_double_range(1, 256) / 255.0; 
            addKeyValue(procesoID, color);
        }

    

        GdkRGBA rgbaColor = getValueByKey(procesoID);

        int red = (int)(rgbaColor.red * 255);
        int green = (int)(rgbaColor.green * 255);
        int blue = (int)(rgbaColor.blue * 255);

        gchar hexColor[8] = " "; 
        g_snprintf(hexColor, sizeof(hexColor), "#%02X%02X%02X", red, green, blue);

        //printf("Proceso ID: %d, Hex Color: %s\n", procesoID, hexColor);

        set_widget_color(labelPageID, "neutro", hexColor);
        gtk_widget_set_name(labelPageID, "neutro");


        sprintf(buffer, "%d", procesoID);
        GtkWidget *labelPID = gtk_label_new(buffer);
        set_widget_color(labelPID, "neutro", hexColor);
        gtk_widget_set_name(labelPID, "neutro");
        gtk_grid_attach(GTK_GRID(optTable), labelPID, 1, i, 1, 1);

        //Columna Loaded
        sprintf(buffer, "%d", tablaPaginasOPT.data[i][2]);
        if(tablaPaginasOPT.data[i][2] == 1){
            // muestra una X si esta cargada
            GtkWidget *labelLoaded = gtk_label_new("X");
            set_widget_color(labelLoaded, "neutro", hexColor);
            gtk_widget_set_name(labelLoaded, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelLoaded, 2, i, 1, 1);
            
            //Columna L-ADDR
            sprintf(buffer, "%d", tablaPaginasOPT.data[i][1]);
            GtkWidget *labelL_ADDR = gtk_label_new(buffer);
            set_widget_color(labelL_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelL_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelL_ADDR, 3, i, 1, 1);

            //Columna M-ADDR
            sprintf(buffer, "%d", tablaPaginasOPT.data[i][3]);
            GtkWidget *labelM_ADDR = gtk_label_new(buffer);
            set_widget_color(labelM_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelM_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelM_ADDR, 4, i, 1, 1);

            //Columna D-ADDR
            
            GtkWidget *labelD_ADDR = gtk_label_new("");
            set_widget_color(labelD_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelD_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelD_ADDR, 5, i, 1, 1);
            
            //Columna LOADED-T
            sprintf(buffer, "%d", tablaPaginasOPT.data[i][4]);
            GtkWidget *labelLoaded_T = gtk_label_new(buffer);
            set_widget_color(labelLoaded_T, "neutro", hexColor);
            gtk_widget_set_name(labelLoaded_T, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelLoaded_T, 6, i, 1, 1);

        }else{
            // muestra una X si esta cargada
            GtkWidget *labelLoaded = gtk_label_new("");
            set_widget_color(labelLoaded, "neutro", hexColor);
            gtk_widget_set_name(labelLoaded, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelLoaded, 2, i, 1, 1);
            
            //Columna L-ADDR
            sprintf(buffer, "%d", tablaPaginasAlg.data[i][1]);
            GtkWidget *labelL_ADDR = gtk_label_new(buffer);
            set_widget_color(labelL_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelL_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelL_ADDR, 3, i, 1, 1);

            //Columna M-ADDR
            GtkWidget *labelM_ADDR = gtk_label_new("");
            set_widget_color(labelM_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelM_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelM_ADDR, 4, i, 1, 1);

            //Columna D-ADDR
            sprintf(buffer, "%d", tablaPaginasOPT.data[i][3]);
            GtkWidget *labelD_ADDR = gtk_label_new(buffer);
            set_widget_color(labelD_ADDR, "neutro", hexColor);
            gtk_widget_set_name(labelD_ADDR, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelD_ADDR, 5, i, 1, 1);
            
            //Columna LOADED-T
            GtkWidget *labelLoaded_T = gtk_label_new("");
            set_widget_color(labelLoaded_T, "neutro", hexColor);
            gtk_widget_set_name(labelLoaded_T, "neutro");
            gtk_grid_attach(GTK_GRID(optTable), labelLoaded_T, 6, i, 1, 1);
        }

        //Columna MARK
        GtkWidget *labelMark = gtk_label_new("");
        set_widget_color(labelMark, "neutro", hexColor);
        gtk_widget_set_name(labelMark, "neutro");
        gtk_grid_attach(GTK_GRID(optTable), labelMark, 7, i, 1, 1);

    }
    // gtk_widget_show_all(optTable); 
    return G_SOURCE_REMOVE; 
}

gboolean mi_funcion_actualizar_interfaz(gpointer data) {
   
    gtk_widget_show_all(windowSimulacion); 
    return G_SOURCE_REMOVE; 
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
    if(tiempoAlg != 0 ){
        porcentajeTrashingAlg = ((double)trashingAlg/tiempoAlg)*100; // porcentaje de trashing de los demas algoritmos
        porcentajeTrashingOpt = ((double)trashingOpt/tiempoOPT)*100; // porcentaje de trashing de OPT

        if(porcentajeTrashingAlg >= 50){
            gtk_widget_set_name(tarshIALG, "rojo");
            gtk_widget_set_name(trashDALG, "rojo");
            gtk_widget_set_name(trashTitleALG, "rojo");
        } else{
            gtk_widget_set_name(tarshIALG, "neutro");
            gtk_widget_set_name(trashDALG, "neutro");
            gtk_widget_set_name(trashTitleALG, "neutro");
        }

        if(porcentajeTrashingOpt >= 50){
            gtk_widget_set_name(tarshIOPT, "rojo");
            gtk_widget_set_name(trashDOPT, "rojo");
            gtk_widget_set_name(trashTitleOPT, "rojo");

        }else{
            gtk_widget_set_name(tarshIOPT, "neutro");
            gtk_widget_set_name(trashDOPT, "neutro");
            gtk_widget_set_name(trashTitleOPT, "neutro");
        }
    }

    contarPaginas();
    contarFragmentacion();
}
//Back
pthread_mutex_t mutex;
pthread_cond_t cond;
int pausado = 0;

gpointer prueba5(gpointer data){
    FILE *archivo;
    regex_t regexNew, regexUse, regexDelete, regexKill;
    archivo = fopen(nombreArchivo, "r");

    if(archivo == NULL){
        printf("Error al abrir el archivo");
        return NULL;
    }

    int status;
    status = regcomp(&regexNew, "new\\([[:digit:]]+,[[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        
        return NULL;
    }

    status = regcomp(&regexUse, "use\\([[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        
        return NULL;
    }

    status = regcomp(&regexDelete, "delete\\([[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {
        
        return NULL;
    }

    status = regcomp(&regexKill, "kill\\([[:digit:]]+\\)", REG_EXTENDED);
    if (status != 0) {

        return NULL;
    }

    char linea[100];
    inicializarRAM(&RamOPT, 100);
    inicializarRAM(&RamAlg, 100);
    int numeroProcesos = 10;
    inicializarLista(&listaProcesos, numeroProcesos); // Inicializar lista de procesos
    tablaPunteros = createMatrix(numeroProcesos, 3); // crear tabla de punteros
    tablaPaginasOPT = createMatrix(5, 6); //crear tabla de paginas OPT
    tablaPaginasAlg = createMatrix(5, 6); //crear tabla de paginas demas algoritmos
    inicializarVirtual(&HDD1, 4); // Memoria virtual para el OPT
    inicializarVirtual(&HDD2, 4); // Memoria virtual para los demas algoritmos
    inicializarLista(&futuroOPT, 10); // lista a futuro de procesos
    
    if(algoritmoSeleccionado == 1 || algoritmoSeleccionado == 2 || algoritmoSeleccionado == 3){
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

    fclose(archivo);
    actualizarEstadisticas();
    estadisticasAlg();
    estadisticasOpt(); 
    
    archivo = fopen(nombreArchivo, "r");
    while(fgets(linea, sizeof(linea),archivo) != NULL){

        pthread_mutex_lock(&mutex);
        while (pausado) {
    
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        
        status = regexec(&regexNew, linea, 0, NULL, 0);
        if (status == 0) {
            g_idle_add(limpiarTabla, NULL);
            g_idle_add(limpiarTabla2, NULL);
            operacionNew(linea);

            g_idle_add(createTableOpt, NULL);
            g_idle_add(createTableAlg, NULL);
            g_idle_add(setRamAlg, NULL);
            g_idle_add(setRamOpt, NULL);
            g_idle_add(mi_funcion_actualizar_interfaz, NULL); 

        }

        status = regexec(&regexUse, linea, 0, NULL, 0);
        if (status == 0) {
            g_idle_add(limpiarTabla, NULL);
            g_idle_add(limpiarTabla2, NULL);
            operacionUse(linea);
            g_idle_add(createTableOpt, NULL);
            g_idle_add(createTableAlg, NULL);
            g_idle_add(setRamAlg, NULL);
            g_idle_add(setRamOpt, NULL);
            g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
        }

        status = regexec(&regexDelete, linea, 0, NULL, 0);
        if (status == 0) {
            g_idle_add(limpiarTabla, NULL);
            g_idle_add(limpiarTabla2, NULL);
            operacionDelete(linea);
            g_idle_add(createTableOpt, NULL);
            g_idle_add(createTableAlg, NULL);
            g_idle_add(setRamAlg, NULL);
            g_idle_add(setRamOpt, NULL);
            g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
        }

        status = regexec(&regexKill, linea, 0, NULL, 0);
        if (status == 0) {
            g_idle_add(limpiarTabla, NULL);
            g_idle_add(limpiarTabla2, NULL);
            operacionKill(linea);
            g_idle_add(createTableOpt, NULL);
            g_idle_add(createTableAlg, NULL);
            g_idle_add(setRamAlg, NULL);
            g_idle_add(setRamOpt, NULL);
            g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
        }

        g_usleep(1000000);

        actualizarEstadisticas();
        estadisticasAlg();
        estadisticasOpt(); 

        
        g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
    }
    actualizarEstadisticas();
    estadisticasAlg();
    estadisticasOpt();

    g_idle_add(mi_funcion_actualizar_interfaz, NULL); 
    //liberar memoria
    regfree(&regexDelete);
    regfree(&regexKill);
    regfree(&regexUse);
    regfree(&regexNew);
    fclose(archivo);
    return NULL;
}



void iniciarSimulacionEnHilo() {

    GThread *simulacionM =g_thread_new(NULL, prueba5, NULL);
    g_thread_unref(simulacionM);
}

//css de toda la vida
static void load_css(void){
    
    GdkDisplay *display;
    GdkScreen * screen;
    
    const gchar *css_style_file = "styles.css";
    
    css_fp = g_file_new_for_path(css_style_file);
    
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

    if (*endptr == '\0') {
        semilla = semillaP;
        g_print("Semilla: %d\n", semilla);
    } else {
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
        gtk_label_set_text (labelRamAlg, "RAM - FIFO");
        gtk_label_set_text (labelMMUAlg, "MMU - FIFO");
    } else if (scChecked) {
        algoritmoSeleccionado = 2;
        gtk_label_set_text (labelRamAlg, "RAM - SECOND CHANCE");
        gtk_label_set_text (labelMMUAlg, "MMU - SECOND CHANCE");
    } else if (mruChecked) {
        algoritmoSeleccionado = 3;
        gtk_label_set_text (labelRamAlg, "RAM - MRU");
        gtk_label_set_text (labelMMUAlg, "MMU - MRU");
    } else if (rndChecked) {
        algoritmoSeleccionado = 4;
        gtk_label_set_text (labelRamAlg, "RAM - RANDOM");
        gtk_label_set_text (labelMMUAlg, "MMU - RANDOM");
    }


    g_print("Algoritmo seleccionado: %d\n", algoritmoSeleccionado);
}
void simulacion(){
    iniciarSimulacionEnHilo();
}

char *fileNameSimulacion = NULL;
void init_simulacion(){
    // Se crea una structura que guarda los datos de la simulacion
    struct start preparacion = crearPreparación(semilla, algoritmoSeleccionado, fileNameSimulacion, numProcesos, numOperaciones);
    srand(preparacion.seed); // Semilla para generar numeros aleatorios
    if(preparacion.file != NULL){
        nombreArchivo = preparacion.file;
        printf("Se ha cargado el archivo de simulacion %s\n", nombreArchivo);
    }else{
        struct Matrix mat = createMatrix(preparacion.NumberProcess, 2); // Se crea la matriz de los punteros y procesos
        createSimulationTxt(preparacion.NumberProcess, preparacion.amoutOperations, &mat); // Se crea el archivo de simulacion
        preparacion.file = fopen("simulation.txt", "r"); // Se abre el archivo de simulacion
        nombreArchivo = "simulation.txt";
        freeMatrix(&mat); // Se libera la memoria de la matriz
    }
    //se activa la ventana de simulacion
    gtk_widget_show(windowSimulacion);
    simulacion();

    liberarMemoriaPrograma();
}

void on_file_chooser_button_clicked(GtkWidget *button, gpointer window) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Abrir archivo", GTK_WINDOW(window), action, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        fileNameSimulacion = gtk_file_chooser_get_filename(chooser);

        FILE *file = fopen(filename, "r");
        if (file != NULL) {
            char line[256];
            while (fgets(line, sizeof(line), file) != NULL) {
                //haz lu tuyo amiguito :)
            }
            fclose(file);
            gtk_label_set_text (archivoLabel, "Archivo Cargado");
            gtk_widget_set_name(archivoLabel, "verde");
        } else {
            g_print("Error opening the file\n");
        }

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}


void pausa_callback(GtkWidget *widget, gpointer data) {
    pthread_mutex_lock(&mutex);
    pausado = 1;
    pthread_mutex_unlock(&mutex);
}

void reanudar_callback(GtkWidget *widget, gpointer data) {
    pthread_mutex_lock(&mutex);
    pausado = 0;
    pthread_cond_signal(&cond); 
    pthread_mutex_unlock(&mutex);
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

    archivoLabel = GTK_WIDGET(gtk_builder_get_object(builder, "estado_archivo")); 

    labelRamAlg = GTK_WIDGET(gtk_builder_get_object(builder, "label_ram_alg")); 
    labelMMUAlg = GTK_WIDGET(gtk_builder_get_object(builder, "label_mmu_alg")); 
    
    trashTitleOPT = GTK_WIDGET(gtk_builder_get_object(builder, "trashing_opt")); 
    trashTitleALG = GTK_WIDGET(gtk_builder_get_object(builder, "trashing_alg")); 
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
    
    pausarSimulacionButton = GTK_WIDGET(gtk_builder_get_object(builder, "pausar_simulation"));
    reanudarSimulacionButton = GTK_WIDGET(gtk_builder_get_object(builder, "continuar_simulation"));
    detenerSimulacionButton = GTK_WIDGET(gtk_builder_get_object(builder, "detener_simulation"));

    fifoCheck = GTK_WIDGET(gtk_builder_get_object(builder, "fifo_check")); 
	scCheck = GTK_WIDGET(gtk_builder_get_object(builder, "sc_check"));
    mruCheck = GTK_WIDGET(gtk_builder_get_object(builder, "mru_check"));
    rndCheck = GTK_WIDGET(gtk_builder_get_object(builder, "rnd_check"));
    
    //GRIDS
    selectAlgorithmTable = GTK_GRID(gtk_builder_get_object(builder, "select_algorithm"));
    optTable = GTK_GRID(gtk_builder_get_object(builder, "opt_table"));
    algTable = GTK_GRID(gtk_builder_get_object(builder, "alg_table"));

    optRam = GTK_GRID(gtk_builder_get_object(builder, "ram_opt"));
    algRam = GTK_GRID(gtk_builder_get_object(builder, "ram_alg"));


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
    
    g_signal_connect( pausarSimulacionButton, "clicked", G_CALLBACK(pausa_callback), NULL );
    g_signal_connect( reanudarSimulacionButton, "clicked", G_CALLBACK(reanudar_callback), NULL );


    g_signal_connect(cargarButton, "clicked", G_CALLBACK(on_file_chooser_button_clicked), (gpointer)windowMenu);


    gtk_builder_connect_signals(builder, NULL);
    

    g_object_unref(builder);
    
    g_signal_connect(windowMenu, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(windowSimulacion, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(windowMenu); //show window
    //gtk_widget_show_all(windowSimulacion); 
    gtk_main(); //run

    return 0;
}

