//gcc -o prueba  mmuSimulationController.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -rdynamic

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h> //pal hilo que gtk tiene jjiijjiij
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    char timeAlgText[20];
    sprintf(timeAlgText, "%d", tiempoAlg);
    gtk_label_set_text(GTK_LABEL(timeALG), timeAlgText);
    
    
    gtk_widget_show(windowSimulacion);
}

void estadisticasOpt(){


}

//Back
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
        // imprimirRAMPaginas(&RamOPT);
        // imprimirVirtual(&HDD1);
        // printf("Ram de los algoritmos\n");
        // imprimirRAMPaginas(&RamAlg);
        // imprimirVirtual(&HDD2);
        
        // imprimirLista(&listaProcesos);
        // printMatrix(&tablaPaginasOPT);
        sleep(1);
        estadisticas();

    }
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
    return 0;
}

gboolean runSimulationInBackground(gpointer data) {
    prueba5();
    return G_SOURCE_REMOVE; // Indicar que el hilo ha terminado y puede ser removido
}

void iniciarSimulacionEnHilo() {
    // Crear un hilo en segundo plano para ejecutar la simulación
    GThread* thread = g_thread_new("SimulacionThread", runSimulationInBackground, NULL);
    g_thread_unref(thread); // Liberar la referencia al hilo para que se autodestruya cuando termine
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
    timeOPT = GTK_WIDGET(gtk_builder_get_object(builder, "time_opt")); 
    timeALG = GTK_WIDGET(gtk_builder_get_object(builder, "time_alg")); 

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
