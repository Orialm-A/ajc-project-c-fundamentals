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
#include "test_random_sensor.h"
#include "test_csv_sensor.h"
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

    RUN_TEST(test_get_random_temperature_stays_in_allowed_range);
    RUN_TEST(test_capteur_aleatoire_is_reproducible_with_same_seed);
    RUN_TEST(test_collecter_releves_fills_expected_values);
    RUN_TEST(test_collecter_releves_does_not_write_past_n);

    RUN_TEST(test_capteur_csv_init_valid_file_returns_count);
    RUN_TEST(test_capteur_csv_init_missing_file_returns_minus_one);
    RUN_TEST(test_capteur_csv_init_ignores_comments_and_blank_lines);
    RUN_TEST(test_capteur_csv_returns_value_for_valid_index);
    RUN_TEST(test_capteur_csv_returns_sentinel_for_out_of_bounds_index);
    RUN_TEST(test_capteur_csv_returns_sentinel_before_init);
    RUN_TEST(test_capteur_csv_init_clamps_out_of_range_values);
    RUN_TEST(test_capteur_csv_fermer_resets_state);
    RUN_TEST(test_capteur_csv_init_twice_without_fermer_does_not_overflow);

    return UNITY_END();
}
