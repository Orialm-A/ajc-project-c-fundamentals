# TODO — Station Météo : ce qui reste



## Ex.1 — Comptage des relevés jamais propagé (`bug??.md`)

- `saisir_releves()` existe toujours mais n'est plus jamais appelée depuis `main.c` (code
  mort depuis le branchement des Parties 6/7) — elle ne renvoie de toute façon jamais son
  compteur de relevés à l'appelant.
- Symptôme toujours actif : `main.c` a maintenant une vraie variable `nb_releves` (mise à
  jour correctement par `action_saisir()` pour l'option 1 du menu), mais les options 2, 3 et
  5 continuent d'utiliser `24` en dur au lieu de `nb_releves`. Peu importe le nombre de
  relevés réellement saisis (manuel, aléatoire ou CSV), les statistiques/alertes/histogramme
  analysent toujours les 24 cases, y compris celles jamais écrites.
- Fix : faire pointer les options 2/3/5 de `main.c` vers `nb_releves` au lieu de `24`.

## Ex.2 — Rapport d'analyse incomplet

- Le sujet demande un bloc `--- Rapport d'analyse ---` avec Moyenne / Minimum (+ heure) /
  Maximum (+ heure) / Amplitude. `main.c` (case 2) n'affiche que Moyenne/Minimale/Maximale,
  sans les heures et sans l'amplitude (`calculer_amplitude` n'est appelée nulle part dans
  `main.c`).
- Cette logique d'affichage est écrite en dur dans le `switch` de `main.c` au lieu d'une
  fonction dédiée dans `affichage.c` — contraire à la consigne "main.c ne doit contenir
  aucune logique métier".

## Ex.3 — Système d'alertes

- Seuils comparés avec `>`/`<` stricts au lieu de `>=`/`<=` (le sujet demande "≥
  seuil_chaud", "≤ seuil_froid") dans `src/alertes.c`. Une température exactement à 35.0°C
  ne déclenche pas `[CANICULE]`.
- `modification_alertes()` (= `action_modifier_seuils` du sujet) est dans `alertes.c`/`.h` au
  lieu d'`affichage.c`/`.h` comme demandé.
- `seuil_froid` par défaut dans `main.c` est `-10.0` au lieu du `0.0` du sujet — à confirmer
  si c'est voulu.

## Ex.4 — Menu et histogramme

- Boucle `while` avec variable pré-initialisée à `1` au lieu du `do { ... } while` demandé
  par le sujet.
- **Probable malentendu** : l'histogramme (`afficher_historigramme`) affiche une barre
  horizontale par heure, alors que le sujet demande un histogramme **vertical** (colonnes,
  hauteur normalisée sur 5 lignes max, axe + heures en légende en dessous). Format
  actuellement différent de ce qui est demandé — à trancher si ça vaut le coup de refaire.
- Le paramètre `Config cfg` de `afficher_historigramme` n'est jamais utilisé dans le corps de
  la fonction — signature qui ne correspond pas au prototype du sujet.
- Bonus "tableau de pointeurs de fonctions" pour le menu — non fait.

## Architecture — constantes partagées

- `include/config.h` n'existe pas. `MAX_RELEVES`/`TEMP_MIN`/`TEMP_MAX` sont maintenant
  publiques dans `capteur.h`, mais `main.c` et `saisie.c` continuent de recoder `24`/`-50.0`/
  `60.0` en dur au lieu de les utiliser. `HISTO_HAUTEUR` n'existe nulle part.
- `-Wall -Wextra -std=c11` demandés par le sujet ne sont pas configurés dans
  `CMakeLists.txt` (le standard C11 l'est via `CMAKE_C_STANDARD`, pas les warnings).

## Partie 5 (`INSTRUCTIONS_PART_2.md`) — Capteur CSV

Reste entièrement à faire — c'est ce qui bloque le test du mode CSV, pas les Parties 6/7
(déjà terminées, voir plus haut) :

- `capteur_csv()` : stub, retourne toujours `0.0` — aucune vérification `csv_ready`/bornes,
  pas de valeur sentinelle.
- `capteur_csv_fermer()` : corps vide, ne remet pas `csv_count`/`csv_ready` à zéro.
- `capteur_csv_init()` :
  - boucle de fin de fichier buggée (`ligne[0] != EOF` au lieu de vérifier que `fgets()`
    renvoie `NULL`),
  - ne vérifie pas le retour de `sscanf` (ligne non numérique → valeur indéterminée stockée),
  - ne clampe pas les valeurs hors plage (juste un avertissement `stderr`),
  - n'appelle jamais `fclose()`.
  - ℹ️ `origin/antoine` a une version de ces trois fonctions plus complète et correcte sur
    ces points précis — probablement récupérable plutôt qu'à réécrire de zéro.
- `choisir_capteur()` ne retente pas de demander un chemin si `data/releves.csv` échoue
  (le sujet le demande explicitement).
- `data/releves.csv` et `data/releves_hiver.csv` sont absents du repo — à recréer/récupérer
  pour pouvoir tester quoi que ce soit ici.
- Bonus `capteur_volatile` — non fait.

## Tests unitaires à écrire (faciles — pas de stdin/stdout à intercepter)

Seuls `statistics.c` et `capteur_aleatoire` (bornes) sont couverts aujourd'hui. Ce qui suit
est testable sans trucage de flux — fonctions pures ou lecture de fichier simple. Tout ce qui
pilote un prompt (`scanf`) ou dont le seul comportement observable est un `printf` est
volontairement exclu, cf. dernière section.

### `collecter_releves()` (`capteur.c`) — non testée du tout actuellement
- Avec un faux capteur trivial (ex : `float stub(int h) { return (float)h; }`), vérifier que
  `collecter_releves(tab, n, stub)` remplit bien `tab[0..n-1]` avec les valeurs attendues.
- Pré-remplir `tab` avec des sentinelles avant l'appel et vérifier que rien au-delà de
  l'index `n-1` n'est modifié.
- Fonction pure, capteur de test trivial : aucune astuce nécessaire.

### `capteur_aleatoire()` / `capteur_aleatoire_init()` — bornes déjà testées, à compléter
- Reproductibilité : `capteur_aleatoire_init(42)` suivi de 24 lectures, répété une seconde
  fois avec la même graine → les deux séries doivent être strictement identiques.
- (Optionnel, plus fragile) `capteur_aleatoire_init(0)` sur deux exécutions séparées du test
  devrait produire des valeurs différentes (basé sur `time(NULL)`) — à ne garder que si ça
  n'introduit pas de flakiness en CI.

### `capteur_csv_init()` / `capteur_csv()` / `capteur_csv_fermer()` — une fois la Partie 5 codée
Pas de prompt ici (contrairement à `capteur_manuel`), juste de la lecture de fichier — donc
testable en écrivant un petit fichier CSV de test dans le test lui-même (fichier temporaire
dédié, pas besoin des fixtures officielles `data/*.csv`) :
- Fichier valide de 3 lignes → `capteur_csv_init()` retourne bien `3`.
- Fichier inexistant → retourne `-1`.
- Lignes commentaires (`#...`) et lignes vides correctement ignorées (absentes du compte).
- `capteur_csv(heure)` retourne la bonne valeur pour un index valide, et la sentinelle
  (`TEMP_MIN - 1.0f`) pour un index hors bornes ou avant tout appel à `capteur_csv_init()`.
- Une valeur hors plage dans le fichier de test est bien clampée (une fois ce comportement
  codé).
- `capteur_csv_fermer()` remet l'état à zéro : `capteur_csv()` appelée après fermeture doit
  retourner la sentinelle, même pour un index auparavant valide.

### Volontairement pas prévu (nécessiterait d'intercepter stdin/stdout)
- `capteur_manuel`, `saisir_releves`, `action_saisir`, `choisir_capteur`,
  `modification_alertes` — toutes pilotées par `scanf`.
- `analyser_alertes` et tout `affichage.c` — leur seul comportement observable est du
  `printf`, rien à vérifier sans capturer stdout.

## Ordre de priorité

1. Décider du sort du bug `nb_releves` (Ex.1) — probablement le plus visible/gênant à
   l'usage, et le seul qui touche à "est-ce que le programme fait ce qu'il montre à l'écran".
2. Rapport d'analyse (Ex.2) — petit, isolé, aucune dépendance avec le reste.
3. Partie 5 (capteur CSV) — plus gros morceau, mais le seul qui débloque un mode entier.
4. Histogramme (Ex.4) — à trancher : refaire au format vertical du sujet, ou assumer le
   format actuel.
5. `config.h` + `-Wall -Wextra` — cosmétique/architecture, aucun impact fonctionnel immédiat.
6. Seuils stricts (Ex.3) et reste (do/while, tests manquants, bonus) — au choix, pas
   bloquant.
