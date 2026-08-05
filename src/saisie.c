#include "saisie.h"
#include "capteur.h"
#include <stdio.h>

fn_capteur choisir_capteur(void) {

    int choix_utilisateur = 1u;
    int user_seed;

    printf("--- Source de donnees ---\n");
    printf("  1. Saisie manuelle (clavier)\n");
    printf("  2. Simulation aleatoire\n");
    printf("  3. Fichier CSV (data/releves.csv)\n");
    printf("  0. Annuler\n");

    scanf("%d", &choix_utilisateur);
    printf("\n");

    switch(choix_utilisateur)
    {
        case 1:
            return capteur_manuel;
            break;

        case 2:
            printf("Select seed (int) - Leave empty for random:");
            if(scanf("%d", &user_seed) == 0) {
                user_seed = RANDOM_SEED;
            }
            capteur_aleatoire_init(user_seed);
            return capteur_aleatoire;
            break;

        case 3:
            int error_code;
            error_code = capteur_csv_init("data/releves.csv");
            if(error_code == 1) {
                return NULL;
            }
            return capteur_csv;
            break;

        case 0:
            return NULL;
            break;

        default:
            return NULL;
            break;
    }

}

void saisir_releves(float * tab)
{
    float temp_user;
    int nb_releves = 0;

    while((nb_releves < 1) || (nb_releves > 24))
    {
        printf("Nombre de releves (24 max) ?\n");
        scanf("%d", &nb_releves);
    }

    for(int i = 0; i < nb_releves; i++)
    {
        printf("Heure %2d : ", i);
        scanf("%f", &temp_user);

        while((temp_user < -50.0) || (temp_user > 60.0))
        {
            printf("ERREUR : valeur hors plage [-50.0 ; 60.0] ! Veuillez resaisir : \n");

            scanf("%f", &temp_user);
        }

        tab[i] = temp_user;
    }

    printf("--- Releves enregistres---\n");
    for(int i = 0; i < nb_releves; i++)
    {
        printf("Heure %2d : %.2f C\n", i, tab[i]);
    }
}

void action_saisir(float *tab, int *nb_releves, int n_max, fn_capteur fn) {

    *nb_releves = 0;
    while((*nb_releves < 1) || (*nb_releves > n_max))
    {
        printf("Nombre de releves (24 max) ?\n");
        scanf("%d", nb_releves);
    }
    collecter_releves(tab, *nb_releves, fn);
}
