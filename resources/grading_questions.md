# Questions du sujet — réponses centralisées

## `INSTRUCTIONS_PART_1.md` — Partie 1 (modularité)

### 1.1 — Rôle d'un fichier `.h`
**Question :** pourquoi ne met-on pas `#include "stats.h"` dans `stats.h` lui-même ?

*Réponse : Un fichier n'a pas besoin de s'inclure lui-même : son propre contenu (prototypes, types) est déjà présent dans le fichier en cours de compilation, il n'y a rien à aller chercher ailleurs pour lui-même. Un header sert à être inclus par les *autres* fichiers qui veulent utiliser ses déclarations.*

### 1.2 — Garde d'inclusion
**Question :** que se passerait-il sans cette garde si deux `.c` incluent le même `.h` ?

*Réponse : Le risque n'est pas que deux `.c` différents incluent le même `.h` chacun de leur côté (ça, c'est normal, chacun a sa propre copie et il n'y a pas de conflit). Le problème survient quand un **même** fichier `.c` inclut deux fois le même `.h`, typiquement de façon transitive (A.c inclut X.h et Y.h, et X.h et Y.h incluent tous les deux Z.h). Sans garde, le contenu de Z.h serait collé deux fois dans la même unité de compilation par le préprocesseur, ce qui donnerait des erreurs de redéfinition (struct/typedef redéfinis) à la compilation.*

### 1.3 — Compilation séparée
**Question :** quel est l'avantage de cette approche si l'on modifie uniquement `alertes.c` ?

*Réponse : On choisit quels fichiers sont recompilés. Ça évite de répéter des tâches déjà faites — seul `alertes.o` doit être régénéré, les autres `.o` déjà à jour sont réutilisés tels quels, puis tout est relié (link) ensemble.*

## `INSTRUCTIONS_PART_1.md` — Partie 4/5 (Makefile)

> Le projet utilise CMake plutôt qu'un Makefile écrit à la main (décision assumée). CMake génère quand même l'équivalent exact de `build/stats.d`, juste ailleurs : `build/debug/CMakeFiles/my_app.dir/src/statistics.c.o.d`.

**Question :** que contient le fichier `build/stats.d` après compilation ?

*Réponse : Une règle Make listant, pour un `.o` donné, la liste de tous les fichiers dont il dépend : le `.c` source, plus tous les `.h` inclus transitivement (y compris les headers système). Exemple réel avec l'équivalent CMake :*

```
CMakeFiles/my_app.dir/src/statistics.c.o: \
 .../src/statistics.c \
 /usr/include/stdc-predef.h \
 .../include/statistics.h
```
*Ça sert à ce que Make (ou son équivalent) sache recompiler `statistics.o` si `statistics.h` change, même si `statistics.c` lui-même n'a pas bougé.*

**Question :** après modification de `src/alertes.c` uniquement, combien de fichiers sont recompilés ? Pourquoi ?

*Réponse : Un seul, `alertes.c` lui-même. Les fichiers qui utilisent son code (via son header) n'ont pas changé : ils connaissent déjà la signature des fonctions qu'ils appellent, donc rien ne les oblige à être recompilés. C'est le linker qui doit ensuite retravailler pour relier le nouveau `alertes.o` au reste.*

**Question (bonus dépendances) :** après modification de `include/config.h` (ex. `MAX_RELEVES` 24→48), combien de `.o` sont recompilés ? Auriez-vous obtenu le même résultat sans le mécanisme `-MMD -MP` ?

*Réponse : Tous ceux qui font `#include "config.h"` (directement ou transitivement). Sans `-MMD -MP` : non, pas le même résultat — Make ne connaît les dépendances que via les règles explicites du Makefile (typiquement juste `stats.o: stats.c`, sans mention des headers inclus). Sans ce mécanisme, Make ignorerait totalement que `stats.o` dépend de `config.h`, donc modifier `config.h` ne déclencherait **aucune** recompilation — le build resterait silencieusement obsolète (`stats.o` utiliserait encore l'ancienne valeur de `MAX_RELEVES` sans que rien ne le signale). C'est exactement le genre de bug que `-MMD -MP` existe pour éviter.*

## `INSTRUCTIONS_PART_2.md` — Partie 1 (pointeurs de fonction / `capteur.h`)

**Questions à traiter :**
1. Lisez `typedef float (*fn_capteur)(int heure);` de l'intérieur vers l'extérieur. Que signifie chaque partie ?
   
   *Réponse : On définit un type nommé `fn_capteur`, qui est un pointeur vers une fonction prenant un `int` en paramètre et retournant un `float`.*
   
2. Quelle est la différence entre `fn(i)` et `(*fn)(i)` ?

   *Réponse : Aucune — ce sont exactement le même appel. En C, un pointeur de fonction est automatiquement déréférencé quand on l'appelle, donc écrire `fn(i)` ou `(*fn)(i)` (ou même `(**fn)(i)`, `(***fn)(i)`, etc.) produit strictement le même résultat. C'est une particularité du C : purement stylistique, pas fonctionnel. `fn` est un pointeur simple vers la fonction (un seul niveau d'indirection), pas un pointeur vers un pointeur.*
   
3. Pourquoi écrit-on `capteur_manuel` (sans `()` ni `&`) pour affecter une fonction à une variable `fn_capteur` ?
   
   *Réponse : `capteur_manuel` est déjà, en tant que nom de fonction non appelé, l'adresse mémoire où se situe le point d'entrée de la fonction — un nom de fonction "décaie" automatiquement en pointeur vers elle-même dans ce contexte, comme un tableau décaie en pointeur vers son premier élément. Ajouter `()` l'appellerait (et donnerait un `float`, pas un pointeur) ; ajouter `&` fonctionnerait aussi (même valeur), mais est redondant et donc inhabituel.*

## `INSTRUCTIONS_PART_2.md` — Partie 2 (`collecter_releves()`)

**Question :** en quoi la fonction `collecter_releves()` illustre-t-elle le polymorphisme en C ?

*Réponse : Elle peut avoir différents comportements selon la valeur de `fn` passée en paramètre, sans dépendre de `if`/`switch` codés en dur à l'intérieur. `collecter_releves()` est un wrapper générique pour `fn` : on peut lui passer différentes fonctions concrètes sans que son propre code, ni son site d'appel, aient besoin de changer — exactement le principe du polymorphisme (même interface, comportement différent selon l'implémentation fournie).*

## `INSTRUCTIONS_PART_2.md` — Partie 4 (capteur aléatoire)

**Question :** pourquoi une graine fixe est-elle utile en TP ou en test automatisé ?

*Réponse : Reproductibilité — avec la même graine, `capteur_aleatoire()` renvoie toujours la même séquence de valeurs, ce qui permet d'écrire des tests déterministes (vérifier une valeur exacte attendue) et de reproduire un bug observé lors d'un run précédent.*

**Question :** pourquoi `-lm` se place-t-il **après** les `.o` dans la commande de link ?

*Réponse : Le linker (`ld`) traite les fichiers de gauche à droite en une seule passe : pour chaque bibliothèque statique (`.a`), il ne conserve que les symboles nécessaires pour résoudre les références **déjà** rencontrées comme non définies à ce stade. Si `-lm` est placé avant les `.o` qui utilisent `sinf()`, le linker scanne `libm` alors qu'aucun symbole `sinf` n'est encore « manquant » — il ne reviendra pas en arrière une fois passé à la suite, donc l'appel à `sinf()` resterait non résolu (`undefined reference`). En plaçant `-lm` après les `.o`, le linker connaît déjà le besoin de `sinf` quand il atteint la bibliothèque, et peut donc l'y trouver.*

## `INSTRUCTIONS_PART_2.md` — Partie 5 (capteur CSV)

**Question :** quel problème surviendrait si `csv_data`/`csv_count`/`csv_ready` étaient globales (sans `static`) ?

*Réponse : Deux problèmes. D'abord, l'encapsulation serait cassée : n'importe quel autre `.c` du projet pourrait lire et modifier directement ces variables en contournant l'API prévue
(`capteur_csv_init`/`capteur_csv`/`capteur_csv_fermer`) — par exemple écrire dans `csv_data` au-delà de `MAX_RELEVES`, ou remettre `csv_count` à une valeur incohérente sans passer par `fermer()`. Ensuite, un risque plus classique : si un autre fichier définissait par erreur une variable globale portant le même nom, on obtiendrait une erreur de « symbole défini plusieurs fois » à l'édition de liens.*

**Questions (5.2 — `capteur_csv_init()`) :**
- Quelle est la différence entre `fscanf` et `fgets` + `sscanf` ?

  *Réponse : `fscanf` lit et parse directement depuis le flux, token par token ; en cas d'échec de parsing au milieu d'une ligne, il laisse le curseur de lecture au milieu de cette ligne, dans un état difficile à resynchroniser proprement pour "passer à la ligne suivante". `fgets` lit toujours une ligne entière (jusqu'au `\n` ou la taille du buffer), donc on avance toujours d'exactement une ligne dans le fichier, que son contenu soit valide ou non. `sscanf` parse ensuite cette ligne déjà isolée en mémoire, indépendamment de la position dans le fichier. La combinaison `fgets`+`sscanf` sépare proprement "avancer d'une ligne" (toujours sûr) de "interpréter cette ligne" (peut échouer sans dérégler la lecture du fichier).*
  
- Pourquoi écrire sur `stderr` plutôt que `stdout` pour les avertissements ?

  *Réponse : `stdout` est réservé à la sortie normale/utile du programme (souvent redirigée vers un fichier ou un pipe) ; `stderr` est réservé aux messages de diagnostic, n'est pas mis en tampon (buffered) par défaut, et reste visible à l'écran même si `stdout` est redirigé. Séparer les deux évite de polluer une sortie de données "propre" avec des messages d'erreur, tout en gardant ces derniers visibles immédiatement.*
  
- Que fait `perror("fopen")` ?

  *Réponse : Affiche sur `stderr` la chaîne donnée en argument, suivie de `: ` puis d'une description textuelle de la valeur actuelle de `errno` (positionnée par le dernier appel système/librairie en échec) — ex. `fopen: No such file or directory`. C'est un raccourci pour ne pas avoir à appeler `strerror(errno)` soi-même.*

**Question (5.3 — `capteur_csv()`) :** qu'est-ce qu'une valeur sentinelle ? Quelles autres stratégies de signalement d'erreur existe-t-il en C ?

*Réponse : Une valeur sentinelle est une valeur réservée, prise dans le même type que le résultat normal de la fonction, utilisée pour signaler une erreur/absence de résultat — à condition qu'elle ne puisse jamais survenir comme résultat valide. Ici, `TEMP_MIN - 1.0f` ne peut jamais être une température réelle (toutes les valeurs valides sont clampées à `>= TEMP_MIN`), donc la recevoir signale sans ambiguïté "pas de valeur ici". Autres stratégies courantes en C : un code de retour entier séparé de la vraie donnée (ex. `capteur_csv_init()` qui retourne `-1` en erreur, un compte sinon) ; une donnée renvoyée via un paramètre de sortie combinée à un booléen de succès séparé (`bool lire(int h, float *out)`) ; la variable globale `errno` couplée à une valeur de retour ambiguë (comme `strtol`) ; ou, pour les erreurs non récupérables, terminer le programme (`exit`/`abort`).*

## `INSTRUCTIONS_PART_2.md` — Parties 6/7 (sélection de capteur)

**Question :** pourquoi appelle-t-on `capteur_csv_fermer()` après la collecte, mais pas une fonction équivalente pour les autres capteurs ?

*Réponse : Parce que ce capteur est le seul à détenir une ressource externe (un fichier ouvert par `fopen()`) qu'il faut explicitement relâcher une fois l'utilisation terminée — remettre l'état interne (`csv_count`/`csv_ready`) à zéro pour ne pas fausser une prochaine lecture. Les autres capteurs (manuel, aléatoire) ne détiennent aucune ressource externe : ils n'ont rien à libérer, donc pas besoin d'un `_fermer()` équivalent.*

## `INSTRUCTIONS_PART_2.md` — Bonus capteur `volatile`

1. Pourquoi déclarer `SENSOR_REG` en `volatile` ? Que ferait le compilateur sans ce mot-clé sur un code optimisé (`-O2`) ?
   
   *Réponse : Sans `volatile`, le compilateur verrait que rien dans le code visible ne modifie `SENSOR_REG` entre deux lectures, et l'optimiserait en conséquence : il ne lirait la valeur qu'une seule fois (en registre ou même à la compilation), en supposant qu'elle ne change jamais — alors qu'en réalité elle peut être modifiée de façon asynchrone par une IT. `volatile` force une vraie lecture mémoire à chaque accès.*
   
2. Qu'est-ce qu'un ISR (Interrupt Service Routine) ?

   *Réponse : Un morceau de code — le plus court et non bloquant possible — qui s'exécute pour gérer l'interruption matérielle correspondante sur le MCU. On préfère s'y limiter à utiliser les primitives de synchronisation du RTOS pour réveiller d'autres tâches plutôt que d'y faire du traitement lourd.*
   
3. Pourquoi écrire `(void)heure` ?

   *Réponse : Ce paramètre n'est pas utilisé dans le corps de la fonction. Le convertir en `(void)` est une façon standard de dire explicitement au compilateur (et à des outils comme cppcheck) "je sais que ce paramètre n'est pas utilisé, c'est volontaire" — ça supprime l'avertissement `-Wunused-parameter` sans changer le comportement du programme.*
   
4. En embarqué réel, `simuler_irq()` n'existerait pas. Que la remplacerait-il ?

   *Réponse : Une interruption matérielle réelle déclenchée par un périphérique du MCU. Le processeur détecte quelle IT s'est déclenchée, cherche le handler correspondant dans la table des vecteurs d'interruption, et fait sauter le PC (Program Counter) dessus.*

## `INSTRUCTIONS_PART_3.md` — Partie 5 (CI/CD, adapté GitHub Actions)

> Ce TP décrit un pipeline GitLab CI, qu'on a adapté en GitHub Actions dans ce projet (décision documentée dans le README, section "Continuous Integration"). La question reste directement testable avec notre équivalent (branch protection + required checks).

**Q13.** Testez : introduisez un bug avant de pousser la MR (PR). Le merge est-il possible quand le pipeline est rouge ?

*Réponse : on a déjà la réponse empirique — un push direct sur `main` avec un commit cassant volontairement quelque chose a été rejeté immédiatement par GitHub (`GH006: Protected branch update failed`), avant même qu'un pipeline ait pu tourner dessus.*
