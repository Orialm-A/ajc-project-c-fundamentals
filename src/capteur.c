#include "../include/capteur.h"

#include <stdio.h>

#define MAX_RELEVES 24

#define TEMP_MIN  (-50.0f)
#define TEMP_MAX  60.0f
#define T_BASE    8.0f
#define AMPLITUDE 15.0f
#define H_MIN     5.0f
#define BRUIT_MAX 1.5f
#define PI        3.14159265f

static float csv_data[MAX_RELEVES];
static int   csv_count = 0;
static int   csv_ready = 0;

/* Implémentation 1 — clavier */
float capteur_manuel(int heure)
{
    float temp = TEMP_MIN - 1;

    while((temp < TEMP_MIN) || (temp > TEMP_MAX))
    {
        printf("Heure %02d : ", heure);
        scanf("%f", &temp);
    }

    return temp;
}


/* Implémentation 2 — simulation aléatoire */
void capteur_aleatoire_init(unsigned int graine)
{

}


float capteur_aleatoire(int heure)
{
    return 0.0;
}


/* Implémentation 3 — fichier CSV */
int capteur_csv_init(const char *chemin)
{
    FILE * ptr_file = fopen(chemin, "r");
    char ligne[64] = {0};
    float val;

    if(ptr_file != NULL)
    {
        fgets(ligne, sizeof(ligne), ptr_file);

        while(ligne[0] != EOF)
        {
            if((ligne[0] != '#') && (ligne[0] != '\n') && (ligne[0] != '\r'))
            {
                sscanf(ligne, "%f", &val);

                if((val < TEMP_MIN) || (val > TEMP_MAX))
                {
                    fwrite("VALEUR HORS PLAGE\n", sizeof(char), sizeof("VALEUR HORS PLAGE"), stderr);
                }

                csv_data[csv_count++] = val;
            }

            fgets(ligne, sizeof(ligne), ptr_file);
        }

        csv_ready = 1;

        fclose(ptr_file);

        return csv_count;
    }
    else
    {
        perror("fopen");
        return -1;
    }
}


float capteur_csv(int heure)
{
    if((csv_ready == 1) && (heure >= 0) && (heure <= (csv_count - 1)))
    {
        return csv_data[heure];
    }
    else
    {
        return (TEMP_MIN - 1.0f);
    }
}


void  capteur_csv_fermer(void)
{
    csv_count = 0;
    csv_ready = 0;
}


/* Collecteur générique */
void collecter_releves(float *tab, int n, fn_capteur fn)
{

}