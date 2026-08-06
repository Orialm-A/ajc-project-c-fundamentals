#include <stdio.h>

#include "saisie.h"
#include "alertes.h"
#include "affichage.h"
#include "capteur.h"
#include "config.h"


int main(void)
{
    int choix_utilisateur;

    float temperatures[24] = {0.0f};

    Config config = {.seuil_chaud = 35.0, .seuil_froid = 0.0, .seuil_amplitude = 20.0};

    int nb_releves = 0;

    do
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
                afficher_rapport(temperatures, nb_releves);
                break;

            case 3:
                analyser_alertes(temperatures, nb_releves, &config);
            break;

            case 4:
                modification_alertes(&config);
            break;

            case 5:
                afficher_historigramme(temperatures, nb_releves);
            break;

            default:
                printf("ERREUR !! La valeur doit etre entre 0 et 6\n");
            break;
        }

        printf("\n");
    } while(choix_utilisateur != 0);

    return 0;
}
