#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "alertes.h"

void afficher_menu(void);

void afficher_historigramme(float * tab, int taille, Config cfg);

void modification_alertes(Config * cfg);

void afficher_rapport(float *tab, int n);

#endif
