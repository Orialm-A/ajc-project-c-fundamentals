/**
 *
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 *
 */

/* This file contains the implementation for the module. Only content
 * declared in the corresponding header is part of the public API. */

// --- includes --------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include "unity.h"
#include "test_statistics.h"
// --- typedefs and structures -----------------------

// --- external declarations -------------------------

// --- defines and macros ----------------------------

// --- functions prototypes --- (scope: module) ------

// --- static global variable (scope: module) --------

// --- function implementations (scope: module) ------
void setUp(void) {}
void tearDown(void) {}

// --- function implementations (scope: public) ------
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(tests_calculer_moyenne);
    RUN_TEST(tests_trouver_minimum);
    RUN_TEST(tests_trouver_maximum);
    RUN_TEST(tests_calculer_amplitude);

    return UNITY_END();
}
