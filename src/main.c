#include <stdio.h>

#include "saisie.h"
#include "alertes.h"
#include "affichage.h"
#include "statistics.h"
#include "capteur.h"


int main(void)
{
    int choix_utilisateur = 1u;

    float temperatures[24] = {-50.0, -45.0, -40.0, -35.0, -30.0, -25.0, -20.0, -15.0, -10.0, -5.0, 0.0, 5.0,
                              10.0, 15.0, 17.5, 20.0, 25.0, 30.0, 35.0, 40.0, 45.0, 50.0, 55.0, 60.0};

    Config config = {.seuil_chaud = 35.0, .seuil_froid = -10.0, .seuil_amplitude = 20.0};

    int min_idx, max_idx;
    int nb_releves = 0;

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

            case 1: {
                fn_capteur fn = choisir_capteur();
                if (fn == NULL) { printf("Annule.\n"); break; }
                action_saisir(temperatures, &nb_releves, MAX_RELEVES, fn);
                if (fn == capteur_csv) capteur_csv_fermer();
            }
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
