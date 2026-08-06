#include "saisie.h"
#include "capteur.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_FILE_PATH_SIZE 128

static void helper_csv_choice(void);

fn_capteur choisir_capteur(void) {

    int choix_utilisateur = 1u;
    int user_seed;

    printf("--- Source de donnees ---\n");
    printf("  1. Saisie manuelle (clavier)\n");
    printf("  2. Simulation aleatoire\n");
    printf("  3. Fichier CSV (data/releves.csv)\n");
    printf("  0. Annuler\n");

    scanf("%d", &choix_utilisateur);
    putchar('\n');
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    switch(choix_utilisateur)
    {
        case 1:
            return capteur_manuel;
            break;

        case 2: {
            int seed_choice;

            printf("Graine : (1) fixe=42  (2) temporelle : ");
            scanf("%d", &seed_choice);

            if(seed_choice == 1) {
                user_seed = 42;
            } else {
                user_seed = RANDOM_SEED;
            }

            capteur_aleatoire_init(user_seed);
            return capteur_aleatoire;
        }
        break;

        case 3:
            helper_csv_choice();
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

static void helper_csv_choice(){
    int error_code;
    const char *DEFAULT_FILE = "data/releves.csv"; // 16 + 1
    char file_to_use[BUFFER_FILE_PATH_SIZE];
    strcpy(file_to_use, DEFAULT_FILE); // Safe as 16 + 1 < 128

    do {
        error_code = capteur_csv_init(file_to_use);

        if (error_code == -1) {
            printf("ERREUR : fichier introuvable (%s). Veuillez saisir un chemin valide : ", file_to_use);
            // No `scanf`!, this other API allow to enforce the size!!
            if(fgets(file_to_use, BUFFER_FILE_PATH_SIZE, stdin) != NULL) {
                file_to_use[strcspn(file_to_use, "\n")] = '\0';
            } else {
                printf("ERREUR : lecture invalide. Veuillez reessayer.\n");
            }
        }
    } while (error_code == -1);
}
