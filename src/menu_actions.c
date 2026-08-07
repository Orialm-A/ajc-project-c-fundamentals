#include "menu_actions.h"

#include "saisie.h"
#include "alertes.h"
#include "affichage.h"
#include "capteur.h"

#include <stdio.h>

// --- static global variable (scope: module) --------
static float temperatures[MAX_RELEVES] = {0.0f};
static Config config = {.seuil_chaud = 35.0, .seuil_froid = 0.0, .seuil_amplitude = 20.0};
static int nb_releves = 0;

// --- function implementations (scope: public) ------

void action_case_0(void) {
    printf("Fin du programme");
}

void action_case_1(void) {
    fn_capteur fn = choisir_capteur();
    if (fn == NULL) {
        printf("Annule.\n");
        return;
    }

    int n_max = MAX_RELEVES;
    if (fn == capteur_csv) {
        n_max = capteur_csv_nb_valeurs();
        if (n_max < 1) {
            printf("ERREUR : le fichier CSV ne contient aucune valeur exploitable.\n");
            capteur_csv_fermer();
            return;
        }
    }

    action_saisir(temperatures, &nb_releves, n_max, fn);
    if (fn == capteur_csv) capteur_csv_fermer();
}

void action_case_2(void) {
    afficher_rapport(temperatures, nb_releves);
}

void action_case_3(void) {
    analyser_alertes(temperatures, nb_releves, &config);
}

void action_case_4(void) {
    modification_alertes(&config);
}

void action_case_5(void) {
    afficher_historigramme(temperatures, nb_releves);
}

