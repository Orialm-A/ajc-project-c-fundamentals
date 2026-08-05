/**
 *
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 *
 */

/* This file contains the implementation for the module. Only content
 * declared in the corresponding header is part of the public API. */

// --- includes --------------------------------------
#include "statistics.h"

// --- typedefs and structures -----------------------

// --- external declarations -------------------------

// --- defines and macros ----------------------------
#define MIN_TEMP_LOCAL -50.0  // Replace with whatever Antoine will have defined in his temperature module
#define MAX_TEMP_LOCAL 60.0  // Replace with whatever Antoine will have defined in his temperature module

// --- functions prototypes --- (scope: module) ------

// --- static global variable (scope: module) --------

// --- function implementations (scope: module) ------

// --- function implementations (scope: public) ------


float calculer_moyenne(
    float *tab,
    int n
) {

    // if(tab == NULL)
    // I Should make this test... But the prototype doesn't allow me to return an error code. I can't change it cause this module might be reused by coworkers. So I'll assume the pointer is OK

    float sum = 0.0;

    for(int i = 0; i < n; i++) {
        sum += tab[i];
    }

    float average = sum / (float)n;

    return average;
}

float trouver_minimum(
    float *tab,
    int n,
    int *idx
) {
    float min = MAX_TEMP_LOCAL;

    for(int i = 0; i < n; i++) {
        if(tab[i] < min) {
            min = tab[i];
            *idx = i;
        }
    }

    return min;
}

float trouver_maximum(
    float *tab,
    int n,
    int *idx

) {
    float max = MIN_TEMP_LOCAL;

    for(int i = 0; i < n; i++) {
        if(tab[i] > max) {
            max = tab[i];
            *idx = i;
        }
    }

    return max;
}

float calculer_amplitude(
    float *tab,
    int n
) {
    int min_index = -1;
    int max_index = -1;

    float min = trouver_minimum(tab, n, &min_index);
    // if(min_index == -1)

    float max = trouver_maximum(tab, n, &max_index);

    return (max - min);
}
