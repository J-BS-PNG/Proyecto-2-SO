//gcc -o prueba  mmuSimulationController.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -rdynamic

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "matriz.h"

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
int pID;

//widgets de la simulacion


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

void init_simulacion(){
    srand(1200); // Semilla para generar numeros aleatorios
    struct start preparacion = crearPreparación(1400, algoritmoSeleccionado, NULL, numProcesos, numOperaciones);
    printf("Se ha creado el archivo de simulacion semilla: %d, option: %d, numero de proceso: %d, numero de Operaciones: %d\n", preparacion.seed, preparacion.option, preparacion.NumberProcess, preparacion.amoutOperations);
}

int main(int argc, char *argv[]){
    GtkBuilder *builder; //GTK builder
    gtk_init(&argc, &argv); //start gtk

    /*GtkWidget* windowMenu;


GtkWidget* pidLabel;
GtkWidget* archivoLabel;

GtkWidget* pidButton;
GtkWidget* processSpinButton;
GtkWidget* operationSpinButton;
GtkWidget* cargarButton;
GtkWidget* iniciarSimulacionButton;

GtkWidget* fifoCheck;
GtkWidget* scCheck;
GtkWidget* mruCheck;
GtkWidget* rndCheck;

GtkGrid* selectAlgorithmTable;
*/

    builder = gtk_builder_new(); //create gtk ui builder
    gtk_builder_add_from_file(builder, "mmuSimulation.glade", NULL); //LOAD UI FILE
    
    load_css();
    //ventana
    windowMenu = GTK_WIDGET(gtk_builder_get_object(builder, "window_menu")); //load window named MyWindow
    gtk_window_set_default_size(GTK_WINDOW(windowMenu), 800, 500);

    //entry
    semillaEntry = GTK_WIDGET(gtk_builder_get_object(builder, "semilla_entry")); 
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
    g_signal_connect( processSpinButton, "activate", G_CALLBACK(actualizarCantProcesos), NULL );
    g_signal_connect( operationSpinButton, "activate", G_CALLBACK(actualizarCantOperaciones), NULL );

    g_signal_connect( fifoCheck, "toggled", G_CALLBACK(selectAlgorithm), NULL );
    g_signal_connect( scCheck, "toggled", G_CALLBACK(selectAlgorithm), NULL );
    g_signal_connect( mruCheck, "toggled", G_CALLBACK(selectAlgorithm), NULL );
    g_signal_connect( rndCheck, "toggled", G_CALLBACK(selectAlgorithm), NULL );
    
    g_signal_connect( iniciarSimulacionButton, "clicked", G_CALLBACK(init_simulacion), NULL );

    gtk_builder_connect_signals(builder, NULL);
    

    g_object_unref(builder);
    


    gtk_widget_show_all(windowMenu); //show window
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
