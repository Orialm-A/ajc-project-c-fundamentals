#ifndef CAPTEUR_H
#define CAPTEUR_H

/**
 *
 *
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 *
 */

/* Public API: visible to other modules */

// --- includes --------------------------------------
#include <stdlib.h>

// --- typedefs and structures (scope: public) -------
typedef float (*fn_capteur)(int heure);

// --- external declarations -------------------------

// --- defines and macros (scope: public) ------------
#define RANDOM_SEED 0u

#define T_BASE 8.0f
#define AMPLITUDE 15.0f
#define H_MIN 5.0f
#define BRUIT_MAX 1.5f
#define PI 3.14159265f

#define TEMP_MIN (-50.0f)
#define TEMP_MAX 60.0f

#define MAX_RELEVES 24

// --- function prototypes (scope: public) -----------
/* Implémentation 1 — clavier */
float capteur_manuel(int heure);

/* Implémentation 2 — simulation aléatoire */
void  capteur_aleatoire_init(unsigned int graine);
float capteur_aleatoire(int heure);

/* Implémentation 3 — fichier CSV */
int   capteur_csv_init(const char *chemin);
float capteur_csv(int heure);
void  capteur_csv_fermer(void);
int   capteur_csv_nb_valeurs(void);

/* Collecteur générique */
void  collecter_releves(float *tab, int n, fn_capteur fn);

#endif /* CAPTEUR_H */
