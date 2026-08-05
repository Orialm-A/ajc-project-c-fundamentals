#include "alertes.h"

#include <stdbool.h>
#include <stdio.h>

void analyser_alertes(float *tab, int n, Config *cfg)
{
    float temp_min = 0.0, temp_max = 0.0;

    bool alerte_canicule = false;
    bool alerte_gel = false;
    bool alerte_amplitude = false;


    for(int i = 0; i < n; i++)
    {
        printf("Heure %2d : %.2f", i, tab[i]);

        if(tab[i] > cfg->seuil_chaud)
        {
            printf(" [CANICULE]\n");
            alerte_canicule = true;
        }
        else if(tab[i] < cfg->seuil_froid)
        {
            printf(" [GEL]\n");
            alerte_gel = true;
        }
        else
        {
            printf("\n");
        }

        if(temp_min > tab[i])
        {
            temp_min = tab[i];
        }

        if(temp_max < tab[i])
        {
            temp_max = tab[i];
        }

    }

    printf("Amplitude globale : %.2f C", (temp_max - temp_min));

    if((temp_max - temp_min) > cfg->seuil_amplitude)
    {
        printf(" [ECART]\n");
        alerte_amplitude = true;
    }
    else
    {
        printf("\n");
    }

    if(!alerte_amplitude && !alerte_canicule && !alerte_gel)
    {
        printf("Aucune alerte");
    }
}


void modification_alertes(Config * cfg)
{
    printf("Nouveau seuil de canicule : ");
    scanf("%f", &cfg->seuil_chaud);

    printf("Nouveau seuil de gel : ");
    scanf("%f", &cfg->seuil_froid);

    printf("Nouveau seuil d'amplitude : ");
    scanf("%f", &cfg->seuil_amplitude);
}