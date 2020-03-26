int timeMusics(int number){

    FMOD_RESULT result;
    FMOD_SYSTEM *system;
    FMOD_SOUND *timeMusic;
    int lenms = 0;
    char **listMusics;
    char link[100] = "musics//";
    char linkMusic[200] = "";

    listMusics = listFiles("./musics",".mp3");
    strcpy(linkMusic,strcat(link,listMusics[number]));

    free(listMusics);


    FMOD_System_Create(&system); //Allocation system dans la mémoire
    FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);

    /* On ouvre la musique */
    result = FMOD_System_CreateSound(system, linkMusic, FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &timeMusic);

    /* On vérifie si elle a bien été ouverte (IMPORTANT) */
    if (result != FMOD_OK)
    {
        printf("Impossible de lire le fichier mp3\n");
        exit(EXIT_FAILURE);
    }

    FMOD_Sound_GetLength(timeMusic, &lenms, FMOD_TIMEUNIT_MS);

    FMOD_Sound_Release(timeMusic);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    return lenms;

}



void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
    #define __PACKED
#else
    #define __PACKED __attribute__((packed))
#endif



void WriteWavHeader(FILE *fp, FMOD_SOUND *sound, int length)
{
    int channels, bits;
    float rate;

    if (!sound)
    {
        return;
    }

    fseek(fp, 0, SEEK_SET);

    FMOD_Sound_GetFormat  (sound, 0, 0, &channels, &bits);
    FMOD_Sound_GetDefaults(sound, &rate, 0, 0, 0);

    {


        typedef struct
        {
	        signed char id[4];
	        int 		size;
        } RiffChunk;

        struct
        {
            RiffChunk       chunk           __PACKED;
            unsigned short	wFormatTag      __PACKED;    /* format type  */
            unsigned short	nChannels       __PACKED;    /* nombre de channel (stereo...)  */
            unsigned int	nSamplesPerSec  __PACKED;    /* sample rate<  */
            unsigned int	nAvgBytesPerSec __PACKED;    /* stimation buffer  */
            unsigned short	nBlockAlign     __PACKED;    /* taille des blocks de data  */
            unsigned short	wBitsPerSample  __PACKED;    /* nombre de bits par échantillon de données mono */
        } FmtChunk  = { {{'f','m','t',' '}, sizeof(FmtChunk) - sizeof(RiffChunk) }, 1, channels, (int)rate, (int)rate * channels * bits / 8, 1 * channels * bits / 8, bits } __PACKED;

        struct
        {
            RiffChunk   chunk;
        } DataChunk = { {{'d','a','t','a'}, length } };

        struct
        {
            RiffChunk   chunk;
	        signed char rifftype[4];
        } WavHeader = { {{'R','I','F','F'}, sizeof(FmtChunk) + sizeof(RiffChunk) + length }, {'W','A','V','E'} };

        fwrite(&WavHeader, sizeof(WavHeader), 1, fp);
        fwrite(&FmtChunk, sizeof(FmtChunk), 1, fp);
        fwrite(&DataChunk, sizeof(DataChunk), 1, fp);
    }
}




void record(GtkWidget *widget, gpointer *data){

        FMOD_SYSTEM          *system  = 0;
        FMOD_SOUND           *sound   = 0;
        FMOD_RESULT            result;
        FMOD_CREATESOUNDEXINFO exinfo;
        int                    key, recorddriver, numdrivers, count;
        unsigned int           version;
        FILE                  *fp;
        unsigned int           datalength = 0, soundlength;
        int number = data[1];

        char fileRecord[50];

        time_t t;

        struct tm *tsy;

        GtkWidget *messageWindow = data[0];
        gtk_widget_destroy(messageWindow);

        t = time(NULL);
        tsy = localtime(&t);
        sprintf(fileRecord,"./records/%d-%d-%d_%d-%d-%d.wav", tsy->tm_hour,tsy->tm_min,tsy->tm_sec,tsy->tm_mday,tsy->tm_mon+1,tsy->tm_year + 1900);


        play("./musics/",".mp3",number);


        result = FMOD_System_Create(&system);
        ERRCHECK(result);

        result = FMOD_System_GetVersion(system, &version);
        ERRCHECK(result);




        result = FMOD_System_SetOutput(system, FMOD_OUTPUTTYPE_DSOUND);

        ERRCHECK(result);


        result = FMOD_System_GetRecordNumDrivers(system, &numdrivers);
        ERRCHECK(result);



        result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, 0);
        ERRCHECK(result);

        memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));

        exinfo.cbsize           = sizeof(FMOD_CREATESOUNDEXINFO);
        exinfo.numchannels      = 2;
        exinfo.format           = FMOD_SOUND_FORMAT_PCM16;
        exinfo.defaultfrequency = 44100;
        exinfo.length           = exinfo.defaultfrequency * sizeof(short) * exinfo.numchannels * 2;
#if defined(WIN32) || defined(__WATCOMC__) || defined(_WIN32) || defined(__WIN32__)
    #define __PACKED
#else
    #define __PACKED __attribute__((packed))
#endif

        result = FMOD_System_CreateSound(system, 0, FMOD_2D | FMOD_SOFTWARE | FMOD_OPENUSER, &exinfo, &sound);
        ERRCHECK(result);





        result = FMOD_System_RecordStart(system, 0, sound, TRUE);
        ERRCHECK(result);



        fp = fopen(fileRecord, "wb");
        if (!fp)
        {
            printf("ERROR : could not open record.wav for writing.\n");
            return 1;
        }


        WriteWavHeader(fp, sound, datalength);

        result = FMOD_Sound_GetLength(sound, &soundlength, FMOD_TIMEUNIT_PCM);
        ERRCHECK(result);

        int iCounter = (timeMusics(data[1])/1000);



        do
        {
            static unsigned int lastrecordpos = 0;
            unsigned int recordpos = 0;


            FMOD_System_GetRecordPosition(system, 0, &recordpos);
            ERRCHECK(result);

            if (recordpos != lastrecordpos)
            {
                void *ptr1, *ptr2;
                int blocklength;
                unsigned int len1, len2;

                blocklength = (int)recordpos - (int)lastrecordpos;
                if (blocklength < 0)
                {
                    blocklength += soundlength;
                }


                FMOD_Sound_Lock(sound, lastrecordpos * exinfo.numchannels * 2, blocklength * exinfo.numchannels * 2, &ptr1, &ptr2, &len1, &len2);   /* * exinfo.numchannels * 2 = stereo 16bit.  1 sample = 4 bytes. */


                if (ptr1 && len1)
                {
                    datalength += fwrite(ptr1, 1, len1, fp);
                }
                if (ptr2 && len2)
                {
                    datalength += fwrite(ptr2, 1, len2, fp);
                }

                FMOD_Sound_Unlock(sound, ptr1, ptr2, len1, len2);
            }

            lastrecordpos = recordpos;


            FMOD_System_Update(system);
            printf("\n%d", iCounter);
            iCounter--;
            Sleep(1000);

        } while (iCounter != 0 );


    WriteWavHeader(fp, sound, datalength);

    fclose(fp);


    result = FMOD_Sound_Release(sound);
    ERRCHECK(result);

    result = FMOD_System_Release(system);
    ERRCHECK(result);

}
