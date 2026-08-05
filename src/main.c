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
    int choix_utilisateur = 1u;

    float temperatures[24] = {-50.0, -45.0, -40.0, -35.0, -30.0, -25.0, -20.0, -15.0, -10.0, -5.0, 0.0, 5.0,
                              10.0, 15.0, 17.5, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0};

    Config config = {.seuil_chaud = 35.0, .seuil_froid = -10.0, .seuil_amplitude = 20.0};

    int min_idx, max_idx;

    while(choix_utilisateur != 0)
    {

        afficher_menu();
        scanf("%d", &choix_utilisateur);
        printf("\n");
        
        switch(choix_utilisateur)
        {
            case 0:
                printf("Fin du programme");
            break;

            case 1:
                saisir_releves(temperatures);
            break;
            
            case 2:
                printf("Moyenne  : %.2f\n", calculer_moyenne(temperatures, 24));
                printf("Minimale : %.2f\n", trouver_minimum(temperatures, 24, &min_idx));
                printf("Maximale : %.2f\n", trouver_maximum(temperatures, 24, &max_idx));
            break;

            case 3:
                analyser_alertes(temperatures, 24, &config);
            break;

            case 4:
                modification_alertes(&config);
            break;

            case 5:
                afficher_historigramme(temperatures, 24, config);
            break;

            default:
                printf("ERREUR !! La valeur doit etre entre 0 et 6\n");
            break;
        }

        printf("\n");
    }

    return 0;
}
