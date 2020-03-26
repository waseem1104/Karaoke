
char ** listFiles(char * path, char * extension){

    int i;
    char ** listMusics;
    struct dirent *lecture;
    DIR *rep;
    char *ptrEnd;

    int count;

    count = countFiles(path, extension);

    listMusics = malloc(sizeof(char *) * count);

    rep = opendir(path );
    i = 0;
    while ((lecture = readdir(rep))) {

        ptrEnd = &lecture->d_name[strlen(lecture->d_name)];

        if (strcmp(ptrEnd-4,extension) == 0){

            listMusics[i] = malloc(sizeof(char ) * 255);
            strcpy(listMusics[i],lecture->d_name);
            i++;


        }

    }

    closedir(rep);
    return listMusics;

}

int countFiles(char * path, char * extension){

    int count = 0;
    struct dirent *lecture;
    DIR *rep;
    char *ptrEnd;


    rep = opendir(path );

    while ((lecture = readdir(rep))) {

        ptrEnd = &lecture->d_name[strlen(lecture->d_name)];

        if (strcmp(ptrEnd-4,extension) == 0){

            count++;


        }

    }

    closedir(rep);
    return count;

}

char * getInfoConfFile(char * groupProperty,char * groupPropertyEnd ,char * name ){

    char line[2000];
    char trash[20];
    char *value = malloc(sizeof(char) * 20);

    FILE *pfConf;

    pfConf = fopen("conf.txt","rb");
    if(pfConf == NULL){
        printf("Le fichier de configuration n'existe pas\n");
        exit(EXIT_FAILURE);
    }else{

        while(fgets(line,2000,pfConf) != NULL){

            if(strstr(line,groupProperty)){

                while(strstr(line,groupPropertyEnd) == NULL){

                    fgets(line,2000,pfConf);
                    if( strstr(line,name)){

                        sscanf(line,"%s : %s ;",trash,value);


                    }
                }


            }
        }

        fclose(pfConf);

        return value;


    }


}



void lyrics(GtkWidget *widget, int number){


    FILE *pf;

    char **listLyrics;
    char *lyricsArray;
    int sizeFile;
    char linkLyrics[200] = "";
    char * getLink = getInfoConfFile("[URL]","[/URL]","linkLyrics");
    listLyrics = listFiles(getLink,".txt");
    strcpy(linkLyrics,strcat(getLink,listLyrics[number]));
    free(getLink);
    free(listLyrics);
    pf = fopen(linkLyrics,"rb");
    if(pf == NULL){
        printf("Le fichier des paroles n'existe pas\n");
        exit(EXIT_FAILURE);
    }else{
       fseek(pf, 0, SEEK_END);
       sizeFile = ftell(pf);
       lyricsArray = malloc(sizeof(char)*sizeFile);
       fseek(pf, 0, SEEK_SET);
       fread(lyricsArray,sizeof (char), sizeFile, pf);
       fclose(pf);


       createLyricsPage(lyricsArray, sizeFile, number);

    }
}





