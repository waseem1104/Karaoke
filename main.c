#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <fmodex/fmod.h>
#include <fmodex/fmod_errors.h>
#include <windows.h>
#include <time.h>
#include "mainPage.h"




int main(int argc, char **argv)
{


    GtkWidget *homePage;
    GtkWidget *table;
    GtkWidget *button[3];
    GtkWidget *vBox;
    GtkWidget *image;
    unsigned int width;
    unsigned int height;
    char * resultWidth = getInfoConfFile("[WINDOW]","[/WINDOW]","width");
    char * resultHeight = getInfoConfFile("[WINDOW]","[/WINDOW]","height");
    sscanf(resultWidth,"%d",&width);
    sscanf(resultHeight,"%d",&height);
    free(resultWidth);
    free(resultWidth);

    gtk_init(&argc,&argv);


    homePage = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(homePage), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(homePage), width, height);
    gtk_window_set_title(GTK_WINDOW(homePage), "KARAOKE");
    g_signal_connect(G_OBJECT(homePage), "destroy", G_CALLBACK(gtk_main_quit), NULL);


    table=gtk_table_new(3,3,TRUE);
    gtk_container_add(GTK_CONTAINER(homePage), GTK_WIDGET(table));

    vBox = gtk_vbox_new(TRUE, 0);

    button[0]= gtk_button_new_with_label("Jouer");
    button[2]= gtk_button_new_with_label("Quitter");

    image = gtk_image_new_from_file("./img.jpg");


    gtk_table_attach(GTK_TABLE(table), vBox,1,2 , 2, 3,GTK_EXPAND | GTK_FILL, GTK_EXPAND,0, 0);
    gtk_table_attach(GTK_TABLE(table), image,1,2 , 0, 2,GTK_EXPAND | GTK_FILL, GTK_EXPAND,0, 0);

    gtk_box_pack_start(GTK_BOX(vBox), button[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vBox), button[2], TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(button[0]), "clicked", G_CALLBACK(menuPage), NULL);
    g_signal_connect(G_OBJECT(button[2]), "clicked", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(homePage);



    gtk_main();
    return EXIT_SUCCESS;
}
