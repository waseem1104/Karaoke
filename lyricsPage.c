#include "record.h"

void removeWindow(GtkWidget *widget, gpointer*data){

    char * getLink = getInfoConfFile("[URL]","[/URL]","linkMusics");
    int number = data[1];
    gtk_widget_destroy(data[0]);
    play(getLink,".mp3",number);
    free(getLink);
}

void play(char *path,char* extension,int number){

    FMOD_RESULT result;
    FMOD_SYSTEM *system;
    FMOD_SOUND *music;
    char **listMusics;
    char linkMusic[200] = "";

    listMusics = listFiles(path,extension);
    strcpy(linkMusic,path);
    strcat(linkMusic,listMusics[number]);
    free(listMusics);


    FMOD_System_Create(&system);
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);

    /* On ouvre la musique */
    result = FMOD_System_CreateSound(system, linkMusic, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &music);

    /* On vérifie si elle a bien été ouverte (IMPORTANT) */
    if (result != FMOD_OK)
    {
        printf("Impossible de lire le fichier mp3\n");
        exit(EXIT_FAILURE);
    }

    /* On joue la musique */
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, music, 0, NULL);




}

void recordConfirm(GtkWidget *widget, int number){

    GtkWidget *confirmWindow;
    GtkWidget *pVBox;
    GtkWidget *pHBox;
    GtkWidget* labelConfirm;
    GtkWidget *buttons[2];
    char *markup;

    gpointer *information = g_new (gpointer, 2);


    confirmWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(confirmWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(confirmWindow), "KARAOKE");
    gtk_window_set_default_size(GTK_WINDOW(confirmWindow), 600, 300);
    g_signal_connect(G_OBJECT(confirmWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    pVBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(confirmWindow), pVBox);

    information[0] = confirmWindow;
    information[1] = number;

    buttons[0] = gtk_button_new_with_label("OUI");
    buttons[1] = gtk_button_new_with_label("NON");

    labelConfirm = gtk_label_new(NULL);

    const char *format = "<span face=\"Verdana\" foreground=\"#39b500\" size=\"large\"><b>%s</b></span>";
    markup = g_markup_printf_escaped (format, "Voulez-vous enregistrer votre son ?");

    gtk_label_set_markup (GTK_LABEL (labelConfirm), markup);

    g_free (markup);


    gtk_label_set_use_markup(GTK_LABEL(labelConfirm), TRUE);
    gtk_label_set_justify(GTK_LABEL(labelConfirm), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(confirmWindow), labelConfirm);
    gtk_box_pack_start(GTK_BOX(pVBox), labelConfirm, FALSE, TRUE, 0);


    pHBox = gtk_hbox_new(TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, TRUE, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(pHBox), buttons[0], TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(pHBox), buttons[1], TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(buttons[0]), "clicked",G_CALLBACK(record), information);
    g_signal_connect(G_OBJECT(buttons[1]), "clicked",G_CALLBACK(removeWindow), information);


    gtk_widget_show_all(confirmWindow);



    gtk_main();

}




void createLyricsPage(char * lyricsArray,int sizefile, int number){

    GtkWidget *lyricsWindow;
    GtkWidget *pVBox;
    GtkWidget *pHBox;
    GtkWidget* lyrics;
    GtkWidget *buttons[2];
    char *styleLyrics;
    char *markup;

    unsigned int width;
    unsigned int height;
    char * resultWidth = getInfoConfFile("[WINDOW]","[/WINDOW]","width");
    char * resultHeight = getInfoConfFile("[WINDOW]","[/WINDOW]","height");
    sscanf(resultWidth,"%d",&width);
    sscanf(resultHeight,"%d",&height);
    free(resultWidth);
    free(resultWidth);

    lyricsWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(lyricsWindow), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(lyricsWindow), "Le Karaoke");
    gtk_window_set_default_size(GTK_WINDOW(lyricsWindow), width, height);
    g_signal_connect(G_OBJECT(lyricsWindow), "destroy", G_CALLBACK(gtk_main_quit), NULL);


    styleLyrics = malloc(sizeof(char) * sizefile);
    strcpy(styleLyrics,lyricsArray);

    buttons[0] = gtk_button_new_with_label("LANCER !");


    pVBox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(lyricsWindow), pVBox);

    lyrics=gtk_label_new(NULL);

    const char *format = "<span face=\"Verdana\" foreground=\"#39b500\" size=\"large\"><b>%s</b></span>";
    markup = g_markup_printf_escaped (format, styleLyrics);

    gtk_label_set_markup (GTK_LABEL (lyrics), markup);
    free(styleLyrics);
    g_free (markup);


    gtk_label_set_use_markup(GTK_LABEL(lyrics), TRUE);
    gtk_label_set_justify(GTK_LABEL(lyrics), GTK_JUSTIFY_CENTER);
    gtk_container_add(GTK_CONTAINER(lyricsWindow), lyrics);
    gtk_box_pack_start(GTK_BOX(pVBox), lyrics, FALSE, TRUE, 0);


    pHBox = gtk_hbox_new(TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pVBox), pHBox, TRUE, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(pHBox), buttons[0], TRUE, TRUE, 0);


    g_signal_connect(G_OBJECT(buttons[0]), "clicked",G_CALLBACK(recordConfirm), number);



    gtk_widget_show_all(lyricsWindow);


    gtk_main();

}
