/**
 *
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 *
 */

/* This file contains the implementation for the module. Only content
 * declared in the corresponding header is part of the public API. */

// --- includes --------------------------------------
#include "capteur.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

// --- typedefs and structures -----------------------

// --- external declarations -------------------------

// --- defines and macros ----------------------------

// --- functions prototypes --- (scope: module) ------

// --- static global variable (scope: module) --------
static float csv_data[MAX_RELEVES];
static int   csv_count = 0;
static int   csv_ready = 0;

// --- function implementations (scope: module) ------

// --- function implementations (scope: public) ------

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
void capteur_aleatoire_init(unsigned int graine) {
    if (graine == 0) {
        srand(time(NULL));
    } else {
        srand(graine);
    }
}

float capteur_aleatoire(int heure) {
    float angle    = PI * ((float)heure - H_MIN) / 12.0f;
    float tendance = T_BASE + AMPLITUDE * sinf(angle);

    float bruit = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
    bruit *= BRUIT_MAX;

    float temperature = tendance + bruit;

    if (temperature < TEMP_MIN) temperature = TEMP_MIN;
    if (temperature > TEMP_MAX) temperature = TEMP_MAX;

    return temperature;
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
                        fwrite("VALEUR HORS PLAGE", sizeof(char), sizeof("VALEUR HORS PLAGE"), stderr);
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
    return 0.0;
}

void  capteur_csv_fermer(void)
{
    csv_count = 0;
    csv_ready = 0;
}

/* Collecteur générique */
void collecter_releves(float *tab, int n, fn_capteur fn)
{
    for (int i = 0; i < n; i++) {
        tab[i] = fn(i);
    }
}
