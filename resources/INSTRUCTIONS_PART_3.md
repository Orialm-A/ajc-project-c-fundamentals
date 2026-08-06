# TP — Pipeline CI/CD GitLab avec la Station Météo C


*Niveau :** Intermédiaire  
**Prérequis :** TP Station Météo v2 terminé, notions Git (commit, push, branch, merge request)  
**Infrastructure :** GitLab CE (Docker) + GitLab Runner (Docker executor)

---

## Contexte

Vous avez un projet C qui fonctionne. Mais comment garantir qu'il
**continue de fonctionner** quand un stagiaire modifie le code ?

Sans CI/CD :

- On compile localement, ça marche "chez moi"
- On pousse le code → un collègue casse quelque chose sans le savoir
- On découvre le bug en démo ou en production

Avec CI/CD :

- Chaque `git push` déclenche automatiquement : compilation, tests, analyse
- Si quelque chose casse → **le pipeline devient rouge** → tout le monde voit
- La branche `main` ne reçoit du code que si **tous les tests passent**

---

## Architecture du pipeline

```
git push
    │
    ▼
┌─────────────────────────────────────────────────────────────┐
│                     GitLab CI Pipeline                       │
│                                                             │
│  Stage 1        Stage 2           Stage 3      Stage 4      │
│  BUILD          TEST              QUALITY      PACKAGE      │
│                                                             │
│  compile   →  test:stats     →  cppcheck   →  release.tar  │
│            →  test:csv       →  format                      │
│            →  test:alertes   →  warnings                    │
│                                                             │
│  ✅ vert = merge autorisé    ❌ rouge = merge BLOQUÉ        │
└─────────────────────────────────────────────────────────────┘
```

Le fichier `.gitlab-ci.yml` à la racine du dépôt décrit ce pipeline.
Le **GitLab Runner** (Docker) lit ce fichier et exécute chaque job
dans un conteneur `gcc:latest` propre.

---

## Partie 0 — Préparation (15 min)

### 0.1 — Vérifier le runner

Dans GitLab : **Settings → CI/CD → Runners**

Vérifier qu'un runner est disponible (pastille verte).


### 0.2 — Pousser le projet sur GitLab

---

## Partie 1 — Creation de`.gitlab-ci.yml`



---

## Partie 2 — Premier pipeline (build uniquement)

### 

## Partie 3 — Stage TEST : les tests unitaires

### 

---

## Partie 4 — Stage QUALITY : analyse statique

### 4.1 — Observer cppcheck

Dans les logs du job `quality:cppcheck`, observez :

- La version de cppcheck installée
- Les fichiers analysés
- Le résultat (normalement : aucune erreur)

### 4.2 — Introduire une erreur détectée par cppcheck



## Partie 5 — Merge Request protégée par le pipeline

Cette partie illustre le cas d'usage principal du CI/CD en équipe.

### 5.1 — Créer une branche feature

```bash
git checkout -b feature/canicule-critique
```

### 5.2 — Implémenter une nouvelle fonctionnalité

Ajoutez dans `src/alertes.c` une alerte supplémentaire `[CRITIQUE]`
pour les valeurs ≥ 40°C :

```c
if (tab[i] >= 40.0f) {
    printf("Heure %02d : %6.1f °C  [CRITIQUE - DANGER]\n", i, tab[i]);
    nb_alertes++;
} else if (tab[i] >= cfg->seuil_chaud) {
    ...
```

### 5.3 — Écrire le test correspondant

Dans `tests/test_alertes.c`, ajoutez :

```c
MU_TEST(test_canicule_critique)
{
    float tab[2] = { 41.0f, 39.0f };
    /* 41.0 >= 40.0 → critique ; 39.0 < 40.0 → non critique */
    int critiques = compter_canicule(tab, 2, 40.0f);
    mu_assert_int_eq(1, critiques);
}
```

### 5.4 — Pousser la branche et créer la Merge Request

```bash
git add src/alertes.c tests/test_alertes.c
git commit -m "feat: alerte CRITIQUE pour températures >= 40°C"
git push origin feature/canicule-critique
```

Dans GitLab : **Merge Requests → New Merge Request**

- Source : `feature/canicule-critique`
- Destination : `main`

**Cochez** : "Pipelines must succeed" dans la protection de branche
(**Settings → Repository → Protected branches → main**)

### 5.5 — Observer la protection

Dans la Merge Request :

- Le pipeline se lance automatiquement
- Le bouton "Merge" est **grisé** tant que le pipeline tourne
- Si le pipeline est vert → le bouton "Merge" devient actif
- Si le pipeline est rouge → le merge est **bloqué**

**Q13.** Testez : introduisez un bug avant de pousser la MR.
Le merge est-il possible quand le pipeline est rouge ?

---

## Partie 6 — Stage PACKAGE : livrable versionné

### 6.1 — Créer un tag de version

```bash
git checkout main
git tag v1.0.0 -m "Version 1.0.0 — Station Météo avec pipeline CI/CD"
git push origin v1.0.0
```

Dans GitLab : **CI/CD → Pipelines** — observez qu'un nouveau pipeline
apparaît, déclenché par le tag, et que le stage `package` s'active.

### 6.2 — Télécharger l'artefact



---

## Partie 7 — Variables CI/CD et secrets

### 7.1 — Variables prédéfinies GitLab

Dans les logs de n'importe quel job, cherchez les valeurs de :

| Variable                | Exemple          | Description           |
| ----------------------- | ---------------- | --------------------- |
| `CI_COMMIT_SHORT_SHA`   | `a3f9b12`        | SHA court du commit   |
| `CI_COMMIT_REF_NAME`    | `main`           | Nom de la branche     |
| `CI_PIPELINE_ID`        | `42`             | Numéro du pipeline    |
| `CI_JOB_NAME`           | `test:stats`     | Nom du job courant    |
| `CI_RUNNER_DESCRIPTION` | `runner-c-meteo` | Description du runner |

### 7.2 — Ajouter une variable secrète

Dans GitLab : **Settings → CI/CD → Variables → Add variable**

```
Key   : METEO_MAX_RELEVES
Value : 48
Type  : Variable
Masked: true   ← invisible dans les logs
```

Modifiez le job `build:compile` pour afficher cette variable :

```yaml
script:
  - echo "MAX_RELEVES configuré à : $METEO_MAX_RELEVES"
  - make CFLAGS="... -DMAX_RELEVES=$METEO_MAX_RELEVES" all
```



---



## Rappels `.gitlab-ci.yml`

```yaml
# Structure de base d'un job
mon_job:
  stage: build          # à quel stage il appartient
  image: gcc:latest     # image Docker utilisée
  needs: ["autre_job"]  # dépendances (DAG)
  before_script:        # commandes avant script
    - echo "setup"
  script:               # commandes principales
    - make all
  after_script:         # toujours exécuté (nettoyage)
    - echo "fin"
  artifacts:            # fichiers à conserver
    paths:
      - build/
    expire_in: 1 hour
  rules:                # conditions de déclenchement
    - if: '$CI_COMMIT_BRANCH == "main"'
  allow_failure: false  # true = pipeline vert même si le job échoue

# Variables prédéfinies utiles
# $CI_COMMIT_SHORT_SHA  → SHA court du commit (ex: a3f9b12)
# $CI_COMMIT_REF_NAME   → branche ou tag (ex: main, v1.0.0)
# $CI_COMMIT_TAG        → tag si push de tag, sinon vide
# $CI_PIPELINE_SOURCE   → push, merge_request_event, schedule...
# $CI_JOB_NAME          → nom du job courant
```
