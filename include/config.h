#ifndef CONFIG_H
#define CONFIG_H

/**
 *
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 *
 */

/* Vocabulaire commun du projet : inclus par tous les autres modules. */

// --- typedefs and structures (scope: public) -------
typedef struct
{
    float seuil_chaud;      /* alerte CANICULE si température >= seuil */
    float seuil_froid;      /* alerte GEL si température <= seuil      */
    float seuil_amplitude;  /* alerte ECART si amplitude >= seuil      */
} Config;

// --- defines and macros (scope: public) ------------
#define MAX_RELEVES 24

#define TEMP_MIN (-50.0f)
#define TEMP_MAX 60.0f

#define HISTO_HAUTEUR 5

#endif /* CONFIG_H */
