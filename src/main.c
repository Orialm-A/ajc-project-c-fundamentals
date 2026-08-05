#include <stdio.h>

#include "releves.h"
#include "alertes.h"
#include "historigramme.h"
#include "statistics.h"


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


int main(void)
{
    int choix_utilisateur = 1;
    float temperatures[24];
    Config config = {.seuil_chaud = 35.0, .seuil_froid = 0.0, .seuil_amplitude = 20.0 };
    int min_idx, max_idx;

    while(choix_utilisateur != 0)
    {

        afficher_menu();
        scanf("%d", &choix_utilisateur);
        
        switch (choix_utilisateur)
        {
            case 1:
                saisir_releves(temperatures);
            break;
            
            case 2:
                printf("Moyenne  : %.2f\n", calculer_moyenne(temperatures, 24));
                printf("Minimale : %.2f\n", trouver_minimum(temperatures, 24, &min_idx));
                printf("Maximale : %.2f\n", trouver_maximum(temperatures, 24, &max_idx));
            break;

            case 3:
                analyser_alertes(temperatures, 12, &config);
            break;

            case 4:
                modification_alertes(&config);
            break;

            case 5:
                temperatures[0] = 15.7;
                temperatures[1] = 32.4;
                afficher_historigramme(temperatures, 2, config);
            break;

            case 6:
                printf("Fin du programme\n");
            break;

            default:
                printf("ERREUR\n");
            break;
        }
    }

    return 0;
}
