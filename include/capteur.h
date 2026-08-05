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

/* Collecteur générique */
void  collecter_releves(float *tab, int n, fn_capteur fn);

#endif /* CAPTEUR_H */
