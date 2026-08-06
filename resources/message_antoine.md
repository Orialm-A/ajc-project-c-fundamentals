Salut Antoine,

Merci d'avoir accepté de repartir sur une branche propre, ça évite qu'on se marche dessus. J'ai intégré sur `fix_todo` les points qu'on avait identifiés ensemble sur les seuils d'alerte et le rapport — ça build et les tests passent. Voici ce qu'il reste à traiter, avec ce qui te concerne plus directement en premier :

**Partie 5 — Capteur CSV (le plus gros morceau restant)**
- `capteur_csv()` : encore un stub, renvoie toujours `0.0`. Il faut qu'elle retourne la valeur lue à l'heure demandée (ou une sentinelle `TEMP_MIN - 1.0f` si hors bornes / pas encore initialisé).
- `capteur_csv_fermer()` : corps vide, doit remettre `csv_count`/`csv_ready` à zéro. **Confirmé avec gdb : ce n'est pas juste un oubli de nettoyage, ça plante réellement** — si on choisit CSV une deuxième fois dans la même exécution, `capteur_csv_init()` repart de l'ancien `csv_count` (24) au lieu de 0 et écrit hors des bornes de `csv_data` (buffer overflow) → segfault. Attention : remettre `csv_count` à zéro seulement dans `capteur_csv_fermer()` ne suffit pas si `capteur_csv_init()` est rappelée sans passer par `fermer()` entre-temps (ex. deux tentatives dans la même sélection) — le plus sûr est que `capteur_csv_init()` remette aussi `csv_count = 0` en tout début de fonction.
- `capteur_csv_init()` : la boucle de fin de fichier est déjà corrigée sur `fix_todo`. Il reste à vérifier le retour de `sscanf` (une ligne non numérique stocke actuellement une valeur indéterminée) et à clamper les valeurs hors plage `[TEMP_MIN, TEMP_MAX]` au lieu de juste avertir sur `stderr`.
- ~~`choisir_capteur()` ne relance pas de demande de chemin si `data/releves.csv` est introuvable — le sujet le demande explicitement.~~
- ~~Il manque `data/releves.csv` dans le repo pour pouvoir tester quoi que ce soit ici.~~ (ajouté sur `fix_todo`, profil été fictif, une valeur par ligne — attention si tu ajoutes des commentaires dans le fichier : `capteur_csv_init()` lit les lignes dans un buffer de 64 octets, un commentaire trop long se fait couper et le reste est lu comme une fausse donnée). `data/releves_hiver.csv` manque toujours.

**Ex.1 — Nombre de relevés (le bug de `bug??.md`)**
- ~~Les options 2/3/5 du menu utilisent encore `24` en dur au lieu de la variable `nb_releves`, du coup elles analysent toujours 24 cases même si moins de relevés ont été saisis.~~

**Ex.4 — Menu et histogramme**

- ~~Boucle `while` au lieu du `do { ... } while` demandé.~~
- ~~Le paramètre `Config cfg` de `afficher_historigramme` n'est pas utilisé.~~
