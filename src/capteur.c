#include "../include/capteur.h"
#include "../include/config.h"

#include <stdio.h>


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
        while(fgets(ligne, sizeof(ligne), ptr_file) != NULL)
        {
            if((ligne[0] != '#') && (ligne[0] != '\n') && (ligne[0] != '\r'))
            {
                if(sscanf(ligne, "%f", &val) != 1)
                {
                    printf("ERREUR de lecture de %s a la ligne %d. Passage a la ligne suivante\n", chemin, (csv_count+1));
                    csv_count++;
                }
                else
                {
                    if((val < TEMP_MIN) || (val > TEMP_MAX))
                    {
                        fwrite("VALEUR HORS PLAGE\n", sizeof(char), sizeof("VALEUR HORS PLAGE"), stderr);
                    }

                    csv_data[csv_count++] = val;
                }
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