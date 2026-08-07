#include "affichage.h"
#include "statistics.h"
#include <stdio.h>


void afficher_historigramme(const float * tab, int taille)
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

void modification_alertes(Config * cfg)
{
    printf("Nouveau seuil de canicule : ");
    scanf("%f", &cfg->seuil_chaud);

    printf("Nouveau seuil de gel : ");
    scanf("%f", &cfg->seuil_froid);

    printf("Nouveau seuil d'amplitude : ");
    scanf("%f", &cfg->seuil_amplitude);
}

void afficher_rapport(const float *tab, int n) {

    int min_idx, max_idx;

    /* I separated the call from the printf. Why?
    * tldr, C doesn't guarantee call/evaluation order,
    * so `max_idx` could have any value when evaluated for the
    * print. */
    float moyenne = calculer_moyenne(tab, n);
    float min = trouver_minimum(tab, n, &min_idx);
    float max = trouver_maximum(tab, n, &max_idx);
    float amplitude = calculer_amplitude(tab, n);

    printf("--- Rapport d'analyse ---\n");

    printf("Moyenne  : %.2f°C\n", moyenne);
    printf("Minimum  : %.2f°C Heure : %d\n", min, min_idx);
    printf("Maximum  : %.2f°C Heure : %d\n", max, max_idx);
    printf("Amplitude: %.2f°C Entre %dh et %dh\n", amplitude, min_idx, max_idx);

}
