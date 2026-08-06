# TP - C Weather Station
Bonjour
## Context

This repository contains our work for the AJC training, the first TP on C fundamentals.

Developers:

- [Antoine Roziere](mailto:44396736+Aroz0@users.noreply.github.com)
- [Orialm Prieur](mailto:108948392+Orialm-A@users.noreply.github.com)

The goal is to simulate a weather station:
- Record hourly temperature readings
- Analyze the collected data and generate alerts based on configurable thresholds
- Provide an interactive menu
  - See [instructions](resources/INSTRUCTIONS.md) for more details

The exercise has three phases:
- **Phase 1**: a single-file, monolithic implementation.
- **Phase 2**: split into modules (`statistics`, `alertes`, `saisie`, `affichage`) with a CMake build.
- **Phase 2 v2**: a function-pointer based hardware abstraction layer (`capteur.h`) letting the
  same collection logic run against a manual (keyboard), simulated-random, or CSV-file "sensor".
  See [instructions part 2](resources/INSTRUCTIONS_PART_2.md).

The code was written with modularity in mind and a CMake-first approach, so phase 2 was straightforward.

## Work distribution

- Antoine implemented exercises 1 (Temperature measures), 3 (Analyze) and 4 (Menu).
- Orialm implemented exercise 2, the CMake build system, the Unity test suite, and the GitHub Actions CI workflow.
- The Phase 2 v2 sensor HAL (`capteur.h`/`capteur.c`) and its wiring into `main()` were built and
  debugged jointly.

## Getting started

Requires a C compiler, [CMake](https://cmake.org/) (>= 3.16), and [just](https://github.com/casey/just).

```sh
git clone --recurse-submodules git@github.com:Orialm-A/ajc-project-c-fundamentals.git
cd ajc-project-c-fundamentals
just build
just run
```

(`external/unity` is a git submodule; `--recurse-submodules` is only needed to build the tests.)

## Common commands

| Command            | Description                                      |
|---------------------|---------------------------------------------------|
| `just build`         | Configure and build the debug binary (`my_app`)   |
| `just run`           | Run the debug binary                              |
| `just rebuild`       | Clean and rebuild the debug binary                |
| `just utest-build`   | Configure and build the Unity test suite          |
| `just utest-run`     | Run the test suite via `ctest`                    |
| `just utest-rebuild` | Clean and rebuild the test suite                  |
| `just check`         | Run `cppcheck` static analysis over `src/`        |
| `just clean`         | Remove all build directories                      |

## Project layout

```
include/    Public headers (one per module)
src/        Module implementations + main.c
tests/      Unity test suite
external/   Vendored dependencies (Unity, as a submodule)
resources/  Exercise instructions
```

## Features

- Modular C architecture (`statistics`, `alertes`, `saisie`, `affichage`, `capteur`)
- Function-pointer based sensor abstraction: manual entry, random simulation, or CSV file
- CMake build system
- Unity unit tests
- GitHub Actions CI: build, unit tests, and static analysis on every push/PR

## Continuous Integration

Three workflows run on every push/PR (`.github/workflows/`):

| Workflow      | Runs                    | Blocks the pipeline on failure? |
|---------------|--------------------------|----------------------------------|
| `tests.yml`   | `just utest-build` + `just utest-run` | Yes |
| `build.yml`   | `just build`             | Yes |
| `quality.yml` | `just check` (`cppcheck --enable=all`) | **No — informational only** |

`quality.yml` is currently non-blocking on purpose: `cppcheck` exits `0` regardless of how many
issues it reports, unless told otherwise. We're keeping it that way while the existing findings
across the codebase get cleared gradually (see `resources/TODO.md`) — instead of turning every
push into a red X for a backlog that predates the check.

Once the codebase is clean, add `--error-exitcode=1` to the `cppcheck` invocation in the `check`
recipe (`justfile`) to make the workflow actually fail when `cppcheck` finds something — at that
point `quality.yml` becomes a real gate instead of a report nobody's forced to read. This is a
one-line change whenever we're ready to make that call.

This mirrors the staged approach `INSTRUCTIONS_PART_3.md` describes (its GitLab CI pipeline only
makes the `main` branch protection depend on a green pipeline in its Partie 5, after the BUILD/
TEST/QUALITY stages already exist) — adapted here to GitHub Actions instead of GitLab CI, since
the project already lives on GitHub.

## Known limitations

- The CSV sensor backend (`capteur_csv`/`capteur_csv_init`/`capteur_csv_fermer` in
  `src/capteur.c`) is not implemented yet — manual entry and random simulation work end to end.
- No `data/*.csv` sample files are checked in yet, so the CSV mode can't be exercised even once
  the backend is done without adding one.

## Todo

See [resources/TODO.md](resources/TODO.md)
