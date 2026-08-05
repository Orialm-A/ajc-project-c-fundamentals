#include "historigramme.h"
#include <stdio.h>

float max(float * tab, int taille)
{
    float max = tab[0];

    for(int i = 0 ; i < taille; i++)
    {
        if(max < tab[i])
        {
            max = tab[i];
        }
    }

    return max;
}


float min(float * tab, int taille)
{
    float min = tab[0];

    for(int i = 0 ; i < taille; i++)
    {
        if(min > tab[i])
        {
            min = tab[i];
        }
    }

    return min;
}


void afficher_historigramme(float * tab, int taille, Config cfg)
{
    int degres_par_etoiles = 110 / 5;
    printf("degres par etoile : %d\n", degres_par_etoiles);

    int etoiles_par_colonnes[taille];

    for(int i = 0; i < taille; i++)
    {
        etoiles_par_colonnes[i] = tab[i] / degres_par_etoiles;
        printf("etoiles par colonne : %d\n", etoiles_par_colonnes[i]);
    }

    for(int i = 0; i < taille; i++)
    {
        
    }
}