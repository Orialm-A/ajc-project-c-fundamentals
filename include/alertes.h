#ifndef ALERTES_H
#define ALERTES_H


typedef struct
{
    float seuil_chaud;      /* alerte CANICULE si température >= seuil */
    float seuil_froid;      /* alerte GEL si température <= seuil      */
    float seuil_amplitude;  /* alerte ECART si amplitude >= seuil      */
} Config;

void analyser_alertes(const float *tab, int n, const Config *cfg);

#endif