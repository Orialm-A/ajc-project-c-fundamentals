#include "affichage.h"
#include <stdio.h>


void afficher_historigramme(float * tab, int taille, Config cfg)
{
    int degres_par_etoiles = 110 / 10;
    printf("degres par etoile : %d\n", degres_par_etoiles);

    int etoiles_par_colonnes[taille];


    for(int i = 0; i < taille; i++)
    {
        etoiles_par_colonnes[i] = (tab[i] + 50.0) / degres_par_etoiles;
        
        printf("Heure %d ", i);
        if(i < 10) printf(" ");
        printf("| ");

        for(int j = 0; j < etoiles_par_colonnes[i]; j++)
        {
            printf("* ");
        }
        printf("\n");
    }

}

void afficher_menu(void)
{
    printf("=== Station Meteo ===\n");
    printf("1. Saisir des releves\n");
    printf("2. Afficher les statistiques\n");
    printf("3. Afficher les alertes\n");
    printf("4. Modifier les seuils d'alerte\n");
    printf("5. Afficher l'histogramme\n");
    printf("0. Quitter\n");
    printf("Choix : ");
}
