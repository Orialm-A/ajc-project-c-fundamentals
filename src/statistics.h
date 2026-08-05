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
#define DEFAULT_INDEX -1
// --- function prototypes (scope: public) -----------

/**
 * @brief Retourne la moyenne des relevés
 *
 * @pre tab != NULL
 * @pre n > 0
 */
float calculer_moyenne(
    float *tab, ///< Array of values
    int n ///< Length of the array
);

/**
 * @brief Retourne le minimum et stocke son index dans `*idx`
 *
 * @pre tab != NULL
 * @pre n > 0
 */
float trouver_minimum(
    float *tab, ///< Array of values
    int n, ///< Length of the array
    int *idx ///< Index of the minimum value

);

/**
 * @brief Retourne le maximum et stocke son index dans `*idx`
 *
 * @pre tab != NULL
 * @pre n > 0
 */
float trouver_maximum(
    float *tab, ///< Array of values
    int n, ///< Length of the array
    int *idx ///< Index of the maximum value

);

/**
 * @brief Retourne `max - min`
 *
 * @pre tab != NULL
 * @pre n > 0
 */
float calculer_amplitude(
    float *tab, ///< Array of values
    int n ///< Length of the array
);

#endif /* STATISTICS_H */
