#ifndef CAPTEUR_H
#define CAPTEUR_H

typedef float (*fn_capteur)(int heure);

/* Implémentation 1 — clavier */
float capteur_manuel(int heure);

/* Implémentation 2 — simulation aléatoire */
void  capteur_aleatoire_init(unsigned int graine);
float capteur_aleatoire(int heure);

/* Implémentation 3 — fichier CSV */
int   capteur_csv_init(const char *chemin);
float capteur_csv(int heure);
void  capteur_csv_fermer(void);

/* Collecteur générique */
void  collecter_releves(float *tab, int n, fn_capteur fn);

#endif