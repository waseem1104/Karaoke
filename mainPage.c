#include "recovery.h"
#include "lyricsPage.h"
void playRecord(GtkWidget *widget,gpointer *data){

    char * getLink = getInfoConfFile("[URL]","[/URL]","linkRecord");
    gtk_widget_destroy(data[1]);
    play(getLink,".wav",data[2]);
    free(getLink);


}
void deleteRecord(GtkWidget * widget, gpointer *data){

    gtk_widget_destroy(data[1]);
    remove(data[0]);

}
void recordMessage(GtkWidget *widget,int number){


    GtkWidget *recordWindow;
    GtkWidget *pVBox;
    GtkWidget *pHBox;
    GtkWidget* labelConfirm;
    GtkWidget *buttons[2];
    char *styleConfirm;
    char *markup;
    char **listRecords;
    char getRecord[100];
    char link[40] = "./records/";
    char * getLink = getInfoConfFile("[URL]","[/URL]","linkRecord");

    gpointer *information = g_new (gpointer, 2);


    recordWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(recordWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(recordWindow), "KARAOKE");
    gtk_window_set_default_size(GTK_WINDOW(recordWindow), 600, 300);
    g_signal_connect(G_OBJECT(recordWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);


    buttons[0] = gtk_button_new_with_label("DELETE");
    buttons[1] = gtk_button_new_with_label("PLAY RECORD !");

    listRecords = listFiles(getLink,".wav");
    free(getLink);
    strcpy(getRecord,listRecords[number]);
    strcat(link,getRecord);
    free(listRecords);
    information[0] = link;
    information[1] = recordWindow;
    information[2] = number;

    pVBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(recordWindow), pVBox);


    pHBox = gtk_hbox_new(TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, TRUE, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(pHBox), buttons[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), buttons[1], TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(buttons[0]), "clicked",G_CALLBACK(deleteRecord), information);
    g_signal_connect(G_OBJECT(buttons[1]), "clicked",G_CALLBACK(playRecord), information);
    gtk_widget_show_all(recordWindow);

    gtk_main();


}
void menuPage(){

    GtkWidget *musicsWindow;
    GtkWidget *pVBox;
    GtkWidget *pHBox;
    GtkWidget *pVBox2;
    GtkWidget *pVBox3;
    GtkWidget **buttonsMusics;
    GtkWidget **buttonsRecords;
    GtkWidget *scroll;
    GtkWidget *scroll2;
    GtkWidget *image;
    char ** listMusics;
    char ** listRecords;
    int i;
    int countMusics;
    int countRecords;
    unsigned int width;
    unsigned int height;
    char * resultWidth = getInfoConfFile("[WINDOW]","[/WINDOW]","width");
    char * resultHeight = getInfoConfFile("[WINDOW]","[/WINDOW]","height");
    char * getLinkR = getInfoConfFile("[URL]","[/URL]","linkRecord");
    char * getLinkM = getInfoConfFile("[URL]","[/URL]","linkMusics");
    sscanf(resultWidth,"%d",&width);
    sscanf(resultHeight,"%d",&height);
    free(resultWidth);
    free(resultWidth);

    gpointer *information = g_new (gpointer, 2);

    musicsWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(musicsWindow), "KARAOKE");
    gtk_window_set_position(GTK_WINDOW(musicsWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(musicsWindow), width, height);
    g_signal_connect(G_OBJECT(musicsWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    information[0] = musicsWindow;

    countMusics = countFiles(getLinkM,".mp3");
    countRecords = countFiles(getLinkR,".wav");


    buttonsMusics = malloc(sizeof(char*)* countMusics);
    buttonsRecords = malloc(sizeof(char*)* countRecords);


    listMusics = listFiles(getLinkM,".mp3");
    listRecords = listFiles(getLinkR,".wav");



    for (i=0; i < countMusics;i++){
        buttonsMusics[i] = malloc(sizeof(char ) * 255);
        buttonsMusics[i] = gtk_button_new_with_label(listMusics[i]);

    }

    for (i=0; i < countRecords;i++){
        buttonsRecords[i] = malloc(sizeof(char ) * 255);
        buttonsRecords[i] = gtk_button_new_with_label(listRecords[i]);

    }

    free(listMusics);
    free(listRecords);

    pVBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(musicsWindow), pVBox);

    image = gtk_image_new_from_file("./img.jpg");
    gtk_box_pack_start(GTK_BOX(pVBox), image, FALSE, FALSE, 0);

    pHBox = gtk_hbox_new(TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, TRUE, TRUE, 0);
    pVBox2 = gtk_vbox_new(FALSE, 0);
    pVBox3 = gtk_vbox_new(FALSE, 0);


    scroll = gtk_scrolled_window_new(NULL,NULL);
    scroll2 = gtk_scrolled_window_new(NULL,NULL);
    gtk_box_pack_start(GTK_BOX(pHBox), scroll, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), scroll2, TRUE, TRUE, 0);

    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll),pVBox2);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),GTK_POLICY_NEVER,GTK_POLICY_ALWAYS);


    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scroll2),pVBox3);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll2),GTK_POLICY_NEVER,GTK_POLICY_ALWAYS);


    for(i = 0; i < countMusics;i++){

        gtk_box_pack_start(GTK_BOX(pVBox2), buttonsMusics[i], FALSE, TRUE, 0);

    }

    for(i = 0; i < countRecords;i++){

        gtk_box_pack_start(GTK_BOX(pVBox3), buttonsRecords[i], FALSE, TRUE, 0);

    }


    for( i = 0; i < countMusics; i++){


        g_signal_connect(G_OBJECT(buttonsMusics[i]), "clicked",G_CALLBACK(lyrics), i);

    }

    for( i = 0; i < countRecords; i++){

        g_signal_connect(G_OBJECT(buttonsRecords[i]), "clicked",G_CALLBACK(recordMessage), i);

    }

    free(getLinkM);
    free(getLinkR);
    free(buttonsMusics);
    free(buttonsRecords);

    gtk_widget_show_all(musicsWindow);


    gtk_main();


}
