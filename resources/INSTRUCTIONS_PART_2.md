# TP — Station Météo v2 : Simulation de Capteur Matériel

**Durée :** 1 journée  
**Niveau :** Intermédiaire / Avancé  
**Prérequis :** TP Station Météo v1 terminé (programme modulaire + Makefile)

---

## Contexte

Votre station météo v1 fonctionne, mais elle impose une saisie clavier.
Dans un projet embarqué réel, la **source de données change** selon l'environnement :

| Environnement | Source de données                            |
| ------------- | -------------------------------------------- |
| Développement | Saisie manuelle (tests rapides)              |
| Intégration   | Fichier CSV avec données préenregistrées     |
| Simulation    | Générateur aléatoire (test des cas extrêmes) |
| Production    | Vrai capteur I²C / SPI / UART                |

Le code métier (calculs, alertes, affichage) **ne doit pas changer** selon la source.
C'est le principe du **HAL — Hardware Abstraction Layer** :
une couche isole le matériel du logiciel applicatif.

En C, cette abstraction se réalise avec un **pointeur de fonction**.

---

## Architecture cible

```
main()
  │
  ├── choisir_capteur()     → retourne un fn_capteur
  │         │
  │   ┌─────┴────────────────────────────────┐
  │   │         Interface fn_capteur          │
  │   │  typedef float (*fn_capteur)(int h); │
  │   └──────┬──────────┬─────────┬──────────┘
  │          │          │         │
  │   capteur_    capteur_   capteur_
  │   manuel      aleatoire  csv
  │
  └── action_saisir(..., fn)
            │
            └── collecter_releves(tab, n, fn)
                      │
                      └── tab[i] = fn(i)   ← appel via pointeur
```

**Règle d'or :** `stats.c`, `alertes.c`, `affichage.c` ne sont **pas modifiés**.
Seuls `capteur.c` et `saisie.c` sont nouveaux ou modifiés.

---

## Structure du projet à produire

```
meteo_v2/
├── Makefile                  ← à modifier (ajouter -lm)
├── data/
│   ├── releves.csv           ← fourni : scénario été
│   └── releves_hiver.csv     ← fourni : scénario hiver
├── include/
│   ├── config.h              ← inchangé
│   ├── capteur.h             ← NOUVEAU à créer
│   ├── saisie.h              ← MODIFIÉ
│   ├── stats.h               ← inchangé
│   ├── alertes.h             ← inchangé
│   └── affichage.h           ← inchangé
└── src/
    ├── main.c                ← MODIFIÉ
    ├── capteur.c             ← NOUVEAU à créer
    ├── saisie.c              ← MODIFIÉ
    ├── stats.c               ← inchangé
    ├── alertes.c             ← inchangé
    └── affichage.c           ← inchangé
```

---

## Partie 1 — Interface générique (`capteur.h`)

Créez `include/capteur.h` avec la garde d'inclusion.

### 1.1 — Le type `fn_capteur`

Définissez le type pointeur de fonction :

```c
typedef float (*fn_capteur)(int heure);
```

**Questions à traiter (réponse orale ou écrite) :**

1. Lisez ce typedef de l'intérieur vers l'extérieur. Que signifie chaque partie ?
2. Quelle est la différence entre `fn(i)` et `(*fn)(i)` ?
3. Pourquoi écrit-on `capteur_manuel` (sans `()` ni `&`) pour affecter une fonction à une variable `fn_capteur` ?

### 1.2 — Prototypes à déclarer dans `capteur.h`

Déclarez les prototypes suivants (avec leur bloc de commentaire) :

```c
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
```

---

## Partie 2 — Collecteur générique (`capteur.c`)

### 2.1 — `collecter_releves()`

Implémentez dans `src/capteur.c` :

```c
void collecter_releves(float *tab, int n, fn_capteur fn)
{
    /* À compléter : boucle qui appelle fn(i) pour i = 0..n-1 */
}
```

**Question :** en quoi cette fonction illustre-t-elle le polymorphisme en C ?

---

## Partie 3 — Implémentation 1 : capteur manuel

### Objectif

Extraire la lecture d'**une seule température** dans une fonction dédiée
qui respecte la signature `float fn(int heure)`.

### Prototype

```c
float capteur_manuel(int heure);
```

### Travail demandé

Implémentez `capteur_manuel()` dans `src/capteur.c` :

1. Afficher `Heure 0X : ` avec le numéro d'heure (`%02d`)
2. Lire une valeur `float` avec `scanf`
3. Valider en boucle : rejeter toute valeur hors `[TEMP_MIN ; TEMP_MAX]`
4. Retourner la valeur validée

**Test rapide :** dans `main()`, appelez temporairement :

```c
float t = capteur_manuel(3);
printf("Valeur recue : %.1f\n", t);
```

Vérifiez que saisir `999.0` déclenche bien un message de rejet.

---

## Partie 4 — Implémentation 2 : capteur aléatoire

### Modèle physique

La température simulée suit le modèle :

```
T(h) = T_BASE + AMPLITUDE × sin( π × (h − H_MIN) / 12 ) + bruit
```

Constantes à définir dans `capteur.c` (`#define`, non exposées dans le `.h`) :

| Constante   | Valeur        | Rôle                              |
| ----------- | ------------- | --------------------------------- |
| `T_BASE`    | `8.0f`        | Température nocturne de référence |
| `AMPLITUDE` | `15.0f`       | Écart jour/nuit                   |
| `H_MIN`     | `5.0f`        | Heure du minimum thermique        |
| `BRUIT_MAX` | `1.5f`        | Bruit maximal ±1.5 °C             |
| `PI`        | `3.14159265f` | π                                 |

### 4.1 — Initialisation `capteur_aleatoire_init()`

```c
void capteur_aleatoire_init(unsigned int graine);
```

- Si `graine == 0` : appeler `srand(time(NULL))` → résultats différents à chaque run
- Si `graine != 0` : appeler `srand(graine)` → résultats reproductibles

**Question :** pourquoi une graine fixe est-elle utile en TP ou en test automatisé ?

### 4.2 — Génération `capteur_aleatoire()`

Étape par étape :

**a)** Calculer la composante sinusoïdale (tendance jour/nuit) :

```c
float angle    = PI * ((float)heure - H_MIN) / 12.0f;
float tendance = T_BASE + AMPLITUDE * sinf(angle);
```

`sinf()` est dans `<math.h>`. Ajoutez `#include <math.h>` dans `capteur.c`.

**b)** Calculer le bruit aléatoire.  
`rand()` retourne un entier dans `[0 ; RAND_MAX]`. Normalisez-le dans `[-1.0 ; +1.0]` :

```c
float bruit = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
bruit *= BRUIT_MAX;
```

**c)** Assembler et protéger par un clamp :

```c
float temperature = tendance + bruit;
if (temperature < TEMP_MIN) temperature = TEMP_MIN;
if (temperature > TEMP_MAX) temperature = TEMP_MAX;
return temperature;
```

### 4.3 — Makefile : ajouter `-lm`

`sinf()` nécessite la bibliothèque mathématique. Ajoutez dans le Makefile :

```makefile
LDFLAGS = -lm

$(TARGET) : $(OBJS)
    $(CC) $^ $(LDFLAGS) -o $@
```

**Question :** pourquoi `-lm` se place-t-il **après** les `.o` dans la commande de link ?

**Test :** générez 24 valeurs avec graine=42. La courbe doit montrer un minimum
vers 5h et un maximum vers 14h-15h.

---

## Partie 5 — Implémentation 3 : capteur CSV

### Format du fichier

```
# Commentaire (ligne ignorée si commence par #)
12.5
-3.2
22.0
```

Une valeur `float` par ligne, dans l'ordre horaire.  
Deux fichiers sont fournis dans `data/` : `releves.csv` et `releves_hiver.csv`.

### 5.1 — Variables internes (dans `capteur.c`)

Déclarez ces variables en `static` (invisibles hors de `capteur.c`) :

```c
static float csv_data[MAX_RELEVES];
static int   csv_count = 0;
static int   csv_ready = 0;
```

**Question :** quel problème surviendrait si ces variables étaient globales (sans `static`) ?

### 5.2 — `capteur_csv_init()`

```c
int capteur_csv_init(const char *chemin);
```

Implémentez la fonction :

1. Ouvrir le fichier avec `fopen(chemin, "r")`. Retourner `-1` si échec.
2. Lire ligne par ligne avec `fgets(ligne, sizeof(ligne), f)`
3. Ignorer les lignes commençant par `#`, `\n` ou `\r`
4. Parser chaque ligne avec `sscanf(ligne, "%f", &val)`. Ignorer si `sscanf` ne retourne pas `1`.
5. Clamper les valeurs hors plage avec un avertissement sur `stderr`
6. Stocker dans `csv_data[csv_count++]`
7. Fermer avec `fclose(f)`, poser `csv_ready = 1`, retourner `csv_count`

**Questions :**

- Quelle est la différence entre `fscanf` et `fgets` + `sscanf` ?
- Pourquoi écrire sur `stderr` plutôt que `stdout` pour les avertissements ?
- Que fait `perror("fopen")` ?

### 5.3 — `capteur_csv()`

```c
float capteur_csv(int heure);
```

Retournez `csv_data[heure]` après avoir vérifié :

- que `csv_ready == 1` (init effectuée)
- que `heure` est dans `[0 ; csv_count - 1]`

En cas d'erreur, retournez `TEMP_MIN - 1.0f` (valeur sentinelle).

**Question :** qu'est-ce qu'une valeur sentinelle ? Quelles autres stratégies de signalement d'erreur existe-t-il en C ?

### 5.4 — `capteur_csv_fermer()`

Remettez `csv_count` et `csv_ready` à zéro.

---

## Partie 6 — Menu de sélection (`saisie.c`)

### 6.1 — `choisir_capteur()`

Dans `src/saisie.c`, implémentez :

```c
fn_capteur choisir_capteur(void);
```

La fonction affiche le sous-menu, lit le choix, et retourne le pointeur de fonction :

```
--- Source de donnees ---
  1. Saisie manuelle (clavier)
  2. Simulation aleatoire
  3. Fichier CSV (data/releves.csv)
  0. Annuler
```

Pour le mode aléatoire, demandez à l'utilisateur son choix de graine :

```
Graine : (1) fixe=42  (2) temporelle :
```

Pour le mode CSV, tentez d'abord `"data/releves.csv"`. Si échec, demandez un chemin.

Retournez `NULL` si l'opérateur annule ou si une initialisation échoue.

### 6.2 — `action_saisir()`

```c
void action_saisir(float *tab, int *nb_releves, int n_max, fn_capteur fn);
```

1. Demandez le nombre de relevés (validation `[1 ; n_max]`)
2. Appelez `collecter_releves(tab, nb, fn)`
3. Mettez à jour `*nb_releves`

---

## Partie 7 — Intégration dans `main.c`

Modifiez l'option `1` du menu principal pour y intégrer le choix de capteur :

```c
case 1: {
    fn_capteur fn = choisir_capteur();
    if (fn == NULL) { printf("Annule.\n"); break; }
    action_saisir(temperatures, &nb_releves, MAX_RELEVES, fn);
    if (fn == capteur_csv) capteur_csv_fermer();
    break;
}
```

**Question :** pourquoi appelle-t-on `capteur_csv_fermer()` après la collecte,
mais pas une fonction équivalente pour les autres capteurs ?

---

## Partie 8 — Tests de validation

Exécutez les tests suivants dans l'ordre et notez les résultats :

| #   | Action                                           | Résultat attendu                     |
| --- | ------------------------------------------------ | ------------------------------------ |
| 1   | Mode manuel, saisir `999.0`                      | Message de rejet, ressaisie demandée |
| 2   | Mode aléatoire graine=42, 24 relevés — lancer 2× | Valeurs identiques aux deux runs     |
| 3   | Mode aléatoire graine temporelle — lancer 2×     | Valeurs différentes aux deux runs    |
| 4   | CSV `releves.csv`, alertes                       | Plusieurs `[CANICULE]` et `[ECART]`  |
| 5   | CSV `releves_hiver.csv`, alertes                 | Plusieurs `[GEL]`                    |
| 6   | CSV chemin inexistant                            | Message d'erreur, pas de crash       |

---

## Bonus — Capteur `volatile` (registre matériel simulé)

Ajoutez une 4ème implémentation qui simule un **registre matériel** mis à jour
par une interruption fictive :

```c
volatile float SENSOR_REG = 0.0f;

static void simuler_irq(void)
{
    /* En embarqué réel : cette fonction serait un ISR */
    SENSOR_REG = 10.0f + ((float)rand() / RAND_MAX) * 30.0f;
}

float capteur_volatile(int heure)
{
    (void)heure;     /* paramètre non utilisé */
    simuler_irq();
    return SENSOR_REG;
}
```

**Questions :**

1. Pourquoi déclarer `SENSOR_REG` en `volatile` ? Que ferait le compilateur sans ce mot-clé sur un code optimisé (`-O2`) ?
2. Qu'est-ce qu'un ISR (Interrupt Service Routine) ?
3. Pourquoi écrire `(void)heure` ?
4. En embarqué réel, `simuler_irq()` n'existerait pas. Que la remplacerait-il ?

---

## Critères d'évaluation

| Critère                                               | Points |
| ----------------------------------------------------- | ------ |
| `capteur.h` : type `fn_capteur`, prototypes, garde    | 2      |
| `collecter_releves()` fonctionnel                     | 2      |
| `capteur_manuel()` avec validation                    | 2      |
| `capteur_aleatoire()` : modèle sin + bruit            | 4      |
| `capteur_csv_init()` : parsing, commentaires, erreurs | 4      |
| `capteur_csv()` : sentinelle, vérifications           | 2      |
| `choisir_capteur()` + `action_saisir()`               | 3      |
| Intégration `main.c`                                  | 1      |
| Makefile `-lm` correct                                | 1      |
| Tests 1 à 6 passants                                  | 3      |
| Qualité : `-Wall -Wextra` sans warnings, commentaires | 2      |
| Bonus : `capteur_volatile` + réponses questions       | 4      |
| **Total**                                             | **30** |

---

## Rappels syntaxiques

```c
/* Pointeur de fonction : déclaration et affectation */
typedef float (*fn_capteur)(int heure);
fn_capteur fn = capteur_manuel;   /* sans () ni & */
float t = fn(3);                  /* appel via pointeur */

/* rand() normalisé dans [0.0 ; 1.0] */
float r = (float)rand() / (float)RAND_MAX;

/* sinf() — nécessite #include <math.h> et -lm au link */
float s = sinf(3.14159f / 2.0f);  /* → 1.0 */

/* Ouverture / lecture / fermeture de fichier */
FILE *f = fopen("data/releves.csv", "r");
if (!f) { perror("fopen"); return -1; }
char ligne[64];
while (fgets(ligne, sizeof(ligne), f)) { ... }
fclose(f);

/* sscanf sur une ligne déjà lue */
float val;
if (sscanf(ligne, "%f", &val) == 1) { /* parsing ok */ }

/* Variable statique locale au fichier */
static float csv_data[24];   /* invisible hors de ce .c */

/* volatile : interdit au compilateur de cacher la valeur */
volatile float REG = 0.0f;
```
