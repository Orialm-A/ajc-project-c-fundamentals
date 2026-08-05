# TP - C Weather Station

## Context

This repository contains our work for the AJC training, the first TP on C fundamentals.

Developers:

- Antoine Roziere *(Contact TBD)*
- [Orialm Prieur](mailto:108948392+Orialm-A@users.noreply.github.com)

The goal is to simulate a weather station:
- Record hourly temperature readings
- Analyze the collected data and generate alerts based on configurable thresholds
- Provide an interactive menu
  - See [instructions](resources/INSTRUCTIONS.md) for more details

The code was written with modularity in mind and a CMake-first approach, so phase 2 was straightforward.

## Work distribution

- Antoine implemented exercises 1 (Temperature measures), 3 (Analyze) and 4 (Menu).
- Orialm implemented exercise 2, the CMake build system, the Unity test suite, and the GitHub Actions CI workflow.

## Features

- Modular C architecture
- CMake build system
- Unity unit tests
- GitHub Actions CI