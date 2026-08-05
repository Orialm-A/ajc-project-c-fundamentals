#ifndef SAISIE_H
#define SAISIE_H

#include "capteur.h"

void saisir_releves(float * tab);

void action_saisir(float *tab, int *nb_releves, int n_max, fn_capteur fn);

#endif /* SAISIE_H */
