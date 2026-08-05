# TP — Station Météo en C : du programme unique à l'architecture modulaire



---

## Contexte général

Vous êtes développeur embarqué dans une start-up spécialisée en capteurs environnementaux.
Votre mission : écrire le firmware d'une **station météo autonome** qui :

- collecte des relevés horaires de température,
- les stocke dans une mémoire fixe (pas d'allocation dynamique — contrainte matérielle),
- les analyse et génère des alertes selon des seuils configurables,
- propose un menu interactif à l'opérateur.

Le TP se déroule en **deux phases** : (OU UNE)

| Phase       | Contenu                                      | Durée     |
| ----------- | -------------------------------------------- | --------- |
| **Phase 1** | Écriture du programme dans un fichier unique | Journée 1 |
| **Phase 2** | Refactorisation en modules + Makefile        | Journée 2 |

---

# PHASE 1 — Programme monolithique

> Objectif : faire fonctionner la station météo en un seul fichier `meteo.c`.

---

## Exercice 1 — Saisie et stockage des relevés (matin)

### Objectif

Permettre à l'opérateur de saisir jusqu'à **24 relevés de température** (un par heure).
Les valeurs sont stockées dans un tableau statique de taille fixe.

### Contraintes

- Tableau déclaré en statique : `float temperatures[24]`
- Nombre de relevés saisi par l'utilisateur (entre 1 et 24)
- Toute valeur hors de la plage **[-50.0 ; +60.0]** doit être **rejetée** avec un message d'erreur, et l'utilisateur doit ressaisir la valeur

### Travail demandé

1. Déclarez les variables nécessaires (`nb_releves`, `temperatures[24]`, etc.)
2. Demandez à l'utilisateur combien de relevés il souhaite saisir (validation incluse)
3. Saisissez chaque température dans une boucle avec validation de la plage
4. Affichez le tableau complet une fois la saisie terminée, sous la forme :

```
Heure 00 :  12.5 °C
Heure 01 :  -3.2 °C
...
```

### Exemple d'exécution attendu

```
Nombre de releves (1-24) : 3

Heure 00 : 45.0
Valeur hors plage [-50.0 ; 60.0]. Ressaisir : 18.5
Heure 01 : -3.2
Heure 02 : 22.0

--- Releves enregistres ---
Heure 00 :  18.5 °C
Heure 01 :  -3.2 °C
Heure 02 :  22.0 °C
```

---

## Exercice 2 — Fonctions d'analyse (matin)

### Objectif

Écrire les fonctions qui calculent les **statistiques** du tableau de relevés.

### Fonctions à écrire

Toutes les fonctions reçoivent le tableau et le nombre de relevés en paramètres.

| Fonction  | Prototype                                            | Description                                         |
| --------- | ---------------------------------------------------- | --------------------------------------------------- |
| Moyenne   | `float calculer_moyenne(float *tab, int n)`          | Retourne la moyenne des relevés                     |
| Minimum   | `float trouver_minimum(float *tab, int n, int *idx)` | Retourne le minimum et stocke son index dans `*idx` |
| Maximum   | `float trouver_maximum(float *tab, int n, int *idx)` | Retourne le maximum et stocke son index dans `*idx` |
| Amplitude | `float calculer_amplitude(float *tab, int n)`        | Retourne `max - min`                                |

> **Attention :** `trouver_minimum` et `trouver_maximum` utilisent un **pointeur** pour retourner l'index en plus de la valeur. C'est le mécanisme de retour multiple en C.

### Travail demandé

1. Écrivez les 4 fonctions dans votre programme
2. Appelez-les depuis `main()` après la saisie de l'exercice 1
3. Affichez un rapport de ce type :

```
--- Rapport d'analyse ---
Moyenne     :  12.4 °C
Minimum     :  -3.2 °C  (heure 01)
Maximum     :  22.0 °C  (heure 02)
Amplitude   :  25.2 °C
```

---

## Exercice 3 — Système d'alertes configurable (après-midi)

### Objectif

Ajouter un système d'**alertes automatiques** basé sur des seuils configurables.

### Structure à définir

```c
typedef struct {
    float seuil_chaud;      /* alerte CANICULE si température >= seuil */
    float seuil_froid;      /* alerte GEL si température <= seuil      */
    float seuil_amplitude;  /* alerte ECART si amplitude >= seuil      */
} Config;
```

### Fonction à écrire

```c
void analyser_alertes(float *tab, int n, Config *cfg);
```

Cette fonction :

1. Parcourt le tableau et affiche `[CANICULE]` pour chaque relevé supérieur ou égal à `seuil_chaud`
2. Affiche `[GEL]` pour chaque relevé inférieur ou égal à `seuil_froid`
3. Calcule l'amplitude globale et affiche `[ECART]` si elle dépasse `seuil_amplitude`
4. Affiche `Aucune alerte` si aucune condition n'est déclenchée

> **Important :** la `Config` est passée **par pointeur** pour permettre sa modification ultérieure (exercice 4) sans copie.

### Valeurs par défaut à utiliser

```c
Config cfg = { .seuil_chaud = 35.0, .seuil_froid = 0.0, .seuil_amplitude = 20.0 };
```

### Exemple d'exécution attendu

```
--- Alertes ---
Heure 05 :  36.1 °C  [CANICULE]
Heure 11 :  -1.2 °C  [GEL]
Amplitude globale : 37.3 °C  [ECART]
```

---

## Exercice 4 — Menu interactif et histogramme ASCII (après-midi)

### Objectif

Encapsuler tout le programme dans un **menu interactif** en boucle, et ajouter un histogramme ASCII.

### Menu à implémenter

```
=== Station Meteo ===
1. Saisir des releves
2. Afficher les statistiques
3. Afficher les alertes
4. Modifier les seuils d'alerte
5. Afficher l'histogramme
0. Quitter
Choix :
```

### Travail demandé

1. Implémentez la boucle principale avec `do { ... } while (choix != 0)` et un `switch`
2. Chaque option appelle les fonctions écrites précédemment
3. L'option **4** permet de modifier les seuils de la `Config` (passage par pointeur)
4. L'option **5** affiche un histogramme ASCII vertical :

```
  *
  *        *
  *   *    *
  *   *    *   *
--------------------
 00  01   02   03
```

Chaque colonne représente une heure. La hauteur est proportionnelle à la température
(normalisée sur 5 lignes maximum).

### Contrainte

Toutes les options doivent appeler les **fonctions existantes** — aucun code dupliqué dans le `switch`.

---

## Bonus Phase 1 — Pointeurs de fonctions

Remplacez le `switch` du menu par un **tableau de pointeurs de fonctions**.

Définissez le type :

```c
typedef void (*action_t)(void);
```

Créez un tableau `action_t actions[6]` pointant vers des fonctions qui correspondent
aux options 0 à 5, puis remplacez le `switch` par :

```c
if (choix >= 0 && choix <= 5)
    actions[choix]();
```

---

## Critères d'évaluation — Phase 1

| Critère                                            | Points |
| -------------------------------------------------- | ------ |
| Saisie avec validation (Ex. 1)                     | 3      |
| Fonctions d'analyse + pointeurs d'index (Ex. 2)    | 4      |
| Structure Config + alertes (Ex. 3)                 | 4      |
| Menu interactif + histogramme ASCII (Ex. 4)        | 5      |
| Qualité du code (nommage, commentaires, structure) | 2      |
| Bonus : pointeurs de fonctions                     | 2      |
| **Total**                                          | **20** |

---

## Rappels syntaxiques

```c
/* Passage de tableau à une fonction */
float calculer_moyenne(float *tab, int n);   /* ou float tab[] */

/* Passage par pointeur pour retour multiple */
float trouver_minimum(float *tab, int n, int *idx) {
    /* stocker l'index : *idx = i; */
}

/* Passage d'une struct par pointeur */
void analyser_alertes(float *tab, int n, Config *cfg) {
    /* accès aux champs : cfg->seuil_chaud */
}
```

---

# PHASE 2 — Architecture modulaire + Makefile

> Objectif : refactoriser le programme en modules séparés et automatiser la compilation.

---

## Objectif de la phase 2

Le programme de la station météo fonctionne, mais tout le code est dans un seul fichier.
Cette organisation devient vite ingérable sur un projet réel.

Vous allez **éclater le projet en modules** (paires `.h` / `.c`) et écrire un **Makefile**
qui automatise la compilation, la gestion des dépendances et le nettoyage.

---

## Structure cible du projet

À la fin de la journée, votre répertoire de travail doit ressembler à ceci :

```
meteo/
├── Makefile
├── include/
│   ├── config.h        ← types partagés (Config, constantes)
│   ├── stats.h         ← prototypes des fonctions statistiques
│   ├── alertes.h       ← prototype de analyser_alertes()
│   ├── saisie.h        ← prototype de action_saisir()
│   └── affichage.h     ← prototypes menu, histogramme, rapport
├── src/
│   ├── main.c          ← boucle principale uniquement
│   ├── stats.c         ← implémentation des fonctions statistiques
│   ├── alertes.c       ← implémentation des alertes
│   ├── saisie.c        ← implémentation de la saisie
│   └── affichage.c     ← implémentation menu, histogramme, rapport
└── build/              ← répertoire créé par le Makefile (objets + binaire)
```

---

## Partie 1 — Comprendre la modularité (théorie + discussion)

Avant de coder, répondez aux questions suivantes (discussion orale ou écrite) :

### 1.1 — Rôle d'un fichier `.h`

Un fichier d'en-tête (header) contient :

- les **prototypes** des fonctions publiques du module
- les **types** (`typedef`, `struct`, `enum`) partagés entre plusieurs `.c`
- les **constantes** (`#define`) utiles à l'extérieur du module

Il ne contient **jamais** d'implémentation (corps de fonction), sauf les fonctions `static inline`.

**Question :** pourquoi ne met-on pas `#include "stats.h"` dans `stats.h` lui-même ?

### 1.2 — Garde d'inclusion

Tout fichier `.h` doit être protégé contre les inclusions multiples :

```c
#ifndef STATS_H
#define STATS_H

/* ... contenu ... */

#endif /* STATS_H */
```

**Question :** que se passerait-il sans cette garde si deux `.c` incluent le même `.h` ?

### 1.3 — Compilation séparée

Avec `gcc`, on peut compiler chaque `.c` en un fichier objet `.o` **indépendamment** :

```bash
gcc -c src/stats.c -I include -o build/stats.o
```

Puis lier tous les `.o` en un exécutable :

```bash
gcc build/stats.o build/alertes.o build/main.o -o build/meteo
```

**Question :** quel est l'avantage de cette approche si l'on modifie uniquement `alertes.c` ?

---

## Partie 2 — Créer les fichiers d'en-tête

### Exercice 2.1 — `include/config.h`

Créez ce fichier. Il doit contenir :

- Les constantes `MAX_RELEVES`, `TEMP_MIN`, `TEMP_MAX`, `HISTO_HAUTEUR`
- La définition de `typedef struct { ... } Config`
- La garde d'inclusion `#ifndef CONFIG_H / #define CONFIG_H / #endif`

> Ce fichier sera inclus par **tous** les autres modules. C'est le "vocabulaire commun" du projet.

### Exercice 2.2 — `include/stats.h`

Créez ce fichier. Il doit contenir :

- `#include "config.h"` (pour le type `float` et `int` — en réalité pour `MAX_RELEVES` si besoin)
- Les **4 prototypes** des fonctions statistiques :
  - `calculer_moyenne`
  - `trouver_minimum`
  - `trouver_maximum`
  - `calculer_amplitude`

### Exercice 2.3 — `include/alertes.h`

Créez ce fichier avec le prototype de `analyser_alertes()`.

### Exercice 2.4 — `include/saisie.h`

Créez ce fichier avec le prototype de `action_saisir()`.

> **Attention :** `action_saisir` modifie le tableau de températures et le compteur de relevés.
> Elle doit les recevoir en paramètres plutôt que d'accéder à des globales.
> Adaptez le prototype en conséquence :
> 
> ```c
> void action_saisir(float *temperatures, int *nb_releves);
> ```

### Exercice 2.5 — `include/affichage.h`

Créez ce fichier avec les prototypes de :

- `afficher_menu(void)`
- `afficher_rapport(float *tab, int n)`
- `afficher_histogramme(float *tab, int n)`
- `action_modifier_seuils(Config *cfg)`

---

## Partie 3 — Créer les fichiers source

### Exercice 3.1 — `src/stats.c`

Créez ce fichier. Il doit :

1. Inclure `"stats.h"` (et **uniquement** ce dont il a besoin)
2. Contenir les implémentations des 4 fonctions statistiques
3. Ne pas contenir de `main()`

### Exercice 3.2 — `src/alertes.c`

Créez `src/alertes.c` avec l'implémentation de `analyser_alertes()`.

### Exercice 3.3 — `src/saisie.c`

Créez `src/saisie.c`. Adaptez `action_saisir()` pour qu'elle reçoive
`float *temperatures` et `int *nb_releves` en paramètres (plus de globales).

### Exercice 3.4 — `src/affichage.c`

Créez `src/affichage.c` avec les implémentations de toutes les fonctions d'affichage.
Incluez les headers dont vous avez besoin (`stats.h` pour les calculs).

### Exercice 3.5 — `src/main.c`

Créez `src/main.c`. Ce fichier doit **uniquement** :

1. Inclure les headers nécessaires
2. Déclarer les variables locales à `main` (`temperatures[MAX_RELEVES]`, `nb_releves`, `cfg`)
3. Contenir la boucle `do/while` avec le `switch`
4. Appeler les fonctions des autres modules

> `main.c` ne doit contenir **aucune logique métier** — uniquement l'orchestration.

---

## Partie 4 — Écrire le Makefile

### 4.1 — Rappel de la syntaxe Makefile

Un Makefile est composé de **règles** :

```makefile
cible : dépendances
    commande        ← OBLIGATOIREMENT une tabulation, pas des espaces
```

Make exécute la `commande` uniquement si la `cible` est plus ancienne qu'une de ses `dépendances`
(ou si la cible n'existe pas).

**Variables utiles :**

| Variable automatique | Signification          |
| -------------------- | ---------------------- |
| `$@`                 | Nom de la cible        |
| `$<`                 | Première dépendance    |
| `$^`                 | Toutes les dépendances |

### 4.2 — Travail demandé

Écrivez un `Makefile` à la racine du projet avec les règles suivantes :

#### Variables à définir en haut du fichier

```makefile
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -I include
SRCDIR  = src
BUILDDIR= build
TARGET  = $(BUILDDIR)/meteo
```

#### Règle `all` (cible par défaut)

Doit construire l'exécutable final. C'est la première règle du fichier, donc celle
exécutée par `make` sans argument.

#### Règle de compilation d'un `.c` en `.o`

Compilez chaque `src/*.c` en `build/*.o`. Utilisez une **règle générique** :

```makefile
$(BUILDDIR)/%.o : $(SRCDIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@
```

#### Règle de liaison (link)

Liez tous les `.o` pour produire `build/meteo`.

#### Règle `clean`

Supprime le répertoire `build/` :

```makefile
clean :
    rm -rf $(BUILDDIR)
```

#### Règle `re`

Enchaîne `clean` puis `all` pour forcer une recompilation complète.

#### Cibles factices

Déclarez `all`, `clean`, `re` comme cibles **phony** (elles ne produisent pas de fichier) :

```makefile
.PHONY : all clean re
```

### 4.3 — Gestion automatique des dépendances (bonus)

Sans précaution, si vous modifiez `include/stats.h`, Make ne recompilera pas `stats.o`
car la dépendance entre `.o` et `.h` n'est pas déclarée explicitement.

`gcc` peut générer ces dépendances automatiquement avec l'option `-MMD -MP`.
Elle crée un fichier `.d` pour chaque `.c`, contenant les dépendances exactes.

Ajoutez au Makefile :

```makefile
CFLAGS += -MMD -MP
DEPS    = $(OBJS:.o=.d)
-include $(DEPS)
```

**Question :** que contient le fichier `build/stats.d` après compilation ?
Affichez-le avec `cat build/stats.d` et analysez son contenu.

---

## Partie 5 — Vérification et tests

Une fois le Makefile écrit, vérifiez le comportement attendu :

### Test 1 — Compilation complète

```bash
make
```

Résultat attendu : les 5 `.o` sont compilés, puis liés en `build/meteo`.

### Test 2 — Recompilation partielle

Modifiez uniquement `src/alertes.c` (ajoutez un commentaire, sauvegardez).

```bash
make
```

**Question :** combien de fichiers sont recompilés ? Pourquoi ?

### Test 3 — Nettoyage et reconstruction

```bash
make clean
make
```

Vérifiez que tout repart de zéro.

### Test 4 — Modification d'un header

Modifiez `include/config.h` (changez `MAX_RELEVES` de 24 à 48).

```bash
make
```

**Question (bonus dépendances) :** combien de `.o` sont recompilés ?
Auriez-vous obtenu le même résultat sans le mécanisme `-MMD -MP` ?

---

## Critères d'évaluation — Phase 2

| Critère                                                  | Points |
| -------------------------------------------------------- | ------ |
| Headers corrects avec gardes d'inclusion                 | 4      |
| Découpage en modules cohérent (pas de logique dans main) | 4      |
| Makefile : variables, règle générique, all/clean/re      | 5      |
| Compilation sans warnings (`-Wall -Wextra`)              | 3      |
| Recompilation partielle fonctionnelle                    | 2      |
| Bonus : gestion automatique des dépendances `.d`         | 2      |
| **Total**                                                | **20** |

---

## Aide-mémoire Makefile

```makefile
# Lister tous les .c du répertoire src/
SRCS = $(wildcard $(SRCDIR)/*.c)

# Transformer src/foo.c en build/foo.o
OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

# Créer le répertoire build/ s'il n'existe pas
$(BUILDDIR)/%.o : $(SRCDIR)/%.c | $(BUILDDIR)
    $(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR) :
    mkdir -p $(BUILDDIR)
```

> Les fonctions `wildcard` et `patsubst` évitent de lister manuellement chaque fichier.
> Ajoutez un nouveau `.c` dans `src/` → il est automatiquement pris en compte par `make`.
