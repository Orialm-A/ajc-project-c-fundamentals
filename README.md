# TP - C Weather Station

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
- GitHub Actions CI (runs the unit tests on every push/PR)

## Known limitations

- The CSV sensor backend (`capteur_csv`/`capteur_csv_init`/`capteur_csv_fermer` in
  `src/capteur.c`) is not implemented yet — manual entry and random simulation work end to end.
- No `data/*.csv` sample files are checked in yet, so the CSV mode can't be exercised even once
  the backend is done without adding one.

## Todo

See [resources/TODO.md](resources/TODO.md)