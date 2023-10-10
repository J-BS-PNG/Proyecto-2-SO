//gcc -o prueba  mmuSimulationController.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic -rdynamic

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Widgets del menu
GtkWidget* windowMenu;


GtkWidget* pidLabel;
GtkWidget* archivoLabel;

GtkWidget* pidButton;
GtkWidget* processSpinButton;
GtkWidget* operationSpinButton;
GtkWidget* cargarButton;
GtkWidget* iniciarSimulacionButton;

GtkGrid* selectAlgorithmTable;

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

int main(int argc, char *argv[]){
    GtkBuilder *builder; //GTK builder
    gtk_init(&argc, &argv); //start gtk

    builder = gtk_builder_new(); //create gtk ui builder
    gtk_builder_add_from_file(builder, "arbolesBinariosDeBusqueda.glade", NULL); //LOAD UI FILE
    
    load_css();
    //ventana
    window = GTK_WIDGET(gtk_builder_get_object(builder, "MyWindow")); //load window named MyWindow
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 500);


    //botones
    cantKeys = GTK_WIDGET(gtk_builder_get_object(builder, "cant_Key")); 
    respuesta = GTK_WIDGET(gtk_builder_get_object(builder, "respuesta_button")); 
	cargar = GTK_WIDGET(gtk_builder_get_object(builder, "cargar_button"));
    guardar = GTK_WIDGET(gtk_builder_get_object(builder, "guardar_button"));
    salir = GTK_WIDGET(gtk_builder_get_object(builder, "salir_button"));

    

    //GRIDS
    treeData = GTK_GRID(gtk_builder_get_object(builder, "tree_Data"));
    treeTable = GTK_GRID(gtk_builder_get_object(builder, "respuesta_table"));

    //ASIGN VARIABLES


    //connect signals
    g_signal_connect( cantKeys, "activate", G_CALLBACK(actualizarCantJuego), NULL );

    gtk_builder_connect_signals(builder, NULL);
    

    g_object_unref(builder);
    


    gtk_widget_show_all(window); //show window
    gtk_main(); //run

    return 0;
}