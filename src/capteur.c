/**
 *
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 *
 */

/* This file contains the implementation for the module. Only content
 * declared in the corresponding header is part of the public API. */

// --- includes --------------------------------------
#include "capteur.h"
#include <math.h>
#include <time.h>

// --- typedefs and structures -----------------------

// --- external declarations -------------------------

// --- defines and macros ----------------------------

// --- functions prototypes --- (scope: module) ------

// --- static global variable (scope: module) --------

// --- function implementations (scope: module) ------

// --- function implementations (scope: public) ------
void capteur_aleatoire_init(unsigned int graine) {
    if (graine == 0) {
        srand(time(NULL));
    } else {
        srand(graine);
    }
}

float get_random_temperature(int heure) {
    float angle    = PI * ((float)heure - H_MIN) / 12.0f;
    float tendance = T_BASE + AMPLITUDE * sinf(angle);

    float bruit = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
    bruit *= BRUIT_MAX;

    float temperature = tendance + bruit;

    if (temperature < TEMP_MIN) temperature = TEMP_MIN;
    if (temperature > TEMP_MAX) temperature = TEMP_MAX;

    return temperature;
}
