#ifndef STATISTICS_H
#define STATISTICS_H

/**
 *
 *
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 *
 */

/* Public API: visible to other modules */

// --- includes --------------------------------------

// --- typedefs and structures (scope: public) -------

// --- external declarations -------------------------

// --- defines and macros (scope: public) ------------

// --- function prototypes (scope: public) -----------

/**
 * @brief Retourne la moyenne des relevés
 */
float calculer_moyenne(
    float *tab, ///< Array of values
    int n ///< Length of the array
);

/**
 * @brief Retourne le minimum et stocke son index dans `*idx`
 */
float trouver_minimum(
    float *tab, ///< Array of values
    int n, ///< Length of the array
    int *idx ///< minimum's index'

);

/**
 * @brief Retourne le maximum et stocke son index dans `*idx`
 */
float trouver_maximum(
    float *tab, ///< Array of values
    int n, ///< Length of the array
    int *idx ///< maximum's index'

);

/**
 * @brief Retourne `max - min`
 */
float calculer_amplitude(
    float *tab, ///< Array of values
    int n ///< Length of the array
);

#endif /* STATISTICS_H */
