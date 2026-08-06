#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "alertes.h"

void afficher_menu(void);

void afficher_historigramme(const float * tab, int taille);

void modification_alertes(Config * cfg);

void afficher_rapport(const float *tab, int n);

#endif
