#ifndef CONFIG_H
#define CONFIG_H

#define MAX_RELEVES 24
#define TEMP_MIN  (-50.0f)
#define TEMP_MAX  60.0f
#define T_BASE    8.0f
#define AMPLITUDE 15.0f
#define H_MIN     5.0f
#define BRUIT_MAX 1.5f
#define PI        3.14159265f
#define RANDOM_SEED 0u


typedef struct
{
    float seuil_chaud;      /* alerte CANICULE si température >= seuil */
    float seuil_froid;      /* alerte GEL si température <= seuil      */
    float seuil_amplitude;  /* alerte ECART si amplitude >= seuil      */
} Config;

#endif