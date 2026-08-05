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

#define T_BASE 8.0f
#define AMPLITUDE 15.0f
#define H_MIN 5.0f
#define BRUIT_MAX 1.5f
#define PI 3.14159265f

#define TEMP_MIN -50.0f
#define TEMP_MAX 60.0f

// --- function prototypes (scope: public) -----------
void capteur_aleatoire_init(unsigned int graine);

float get_random_temperature(int heure);
#endif /* CAPTEUR_H */
