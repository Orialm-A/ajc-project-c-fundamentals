# TODO — Station Météo : ce qui reste

> Partie 5 (capteur CSV) est maintenant complète et fonctionnelle (voir `git log` pour le
> détail — `capteur_csv()` implémentée, bug de double-lecture corrigé, dépassement de
> `csv_data` impossible, valeurs `NaN`/hors plage gérées). Ce qui reste est optionnel :
> bonus, tests, polish cosmétique.

## Ex.2 — Rapport d'analyse (polish cosmétique, optionnel)

`afficher_rapport()` existe dans `affichage.c`/`.h` et affiche Moyenne/Min/Max (+ heure)/
Amplitude — fonctionnellement complet. Reste, si on veut coller exactement au format du sujet :
- Le sujet attend `--- Rapport d'analyse ---` en en-tête ; pas fait.
- Le sujet utilise "Minimum"/"Maximum", le code utilise "Minimale"/"Maximale".

## Ex.4 — Bonus tableau de pointeurs de fonctions

Non fait — bonus, pas bloquant.

## Architecture

- `include/config.h` n'existe toujours pas en tant que fichier dédié (le sujet le demande),
  mais `MAX_RELEVES`/`TEMP_MIN`/`TEMP_MAX` sont publiques dans `capteur.h` et effectivement
  utilisées partout où il faut. Impact fonctionnel nul, question purement d'organisation.

## Partie 5 (`INSTRUCTIONS_PART_2.md`) — Capteur CSV

Fonctionnellement complète. Reste, non-bloquant :
- `data/releves_hiver.csv` toujours absent (`data/releves.csv` présent et fonctionnel).
- Bonus `capteur_volatile` — non fait.

## Tests unitaires à écrire (faciles — pas de stdin/stdout à intercepter)

Seuls `statistics.c` et les bornes de `capteur_aleatoire` sont couverts aujourd'hui. Ce qui
suit est testable sans trucage de flux — fonctions pures ou lecture de fichier simple. Tout ce
qui pilote un prompt (`scanf`) ou dont le seul comportement observable est un `printf` est
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

### `capteur_csv_init()` / `capteur_csv()` / `capteur_csv_fermer()` — Partie 5 codée, tests à écrire
Pas de prompt ici (contrairement à `capteur_manuel`), juste de la lecture de fichier — donc
testable en écrivant un petit fichier CSV de test dans le test lui-même (fichier temporaire
dédié, pas besoin des fixtures officielles `data/*.csv`) :
- Fichier valide de 3 lignes → `capteur_csv_init()` retourne bien `3`.
- Fichier inexistant → retourne `-1`.
- Lignes commentaires (`#...`) et lignes vides correctement ignorées (absentes du compte).
- `capteur_csv(heure)` retourne la bonne valeur pour un index valide, et la sentinelle
  (`TEMP_MIN - 1.0f`) pour un index hors bornes ou avant tout appel à `capteur_csv_init()`.
- Une valeur hors plage dans le fichier de test est bien clampée.
- `capteur_csv_fermer()` remet l'état à zéro : `capteur_csv()` appelée après fermeture doit
  retourner la sentinelle, même pour un index auparavant valide.
- Appeler `capteur_csv_init()` deux fois de suite (sans `fermer()` entre les deux) ne doit pas
  dépasser les bornes de `csv_data` — reproduit le crash trouvé avec gdb, maintenant corrigé.

### Volontairement pas prévu (nécessiterait d'intercepter stdin/stdout)
- `capteur_manuel`, `saisir_releves`, `action_saisir`, `choisir_capteur`,
  `modification_alertes` — toutes pilotées par `scanf`.
- `analyser_alertes` et tout `affichage.c` — leur seul comportement observable est du
  `printf`, rien à vérifier sans capturer stdout.
