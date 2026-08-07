/**
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 */

// --- includes --------------------------------------
#include "unity.h"             // Needed for Unity assertions
#include "capteur.h"        // Module under test
#include "test_random_sensor.h"

// --- defines and macros ----------------------------
#define TESTS_SEED 42u

// --- typedefs and structures -----------------------

// Helper macro to count test cases cleanly

// --- function implementations (scope: public) ------

void test_get_random_temperature_stays_in_allowed_range(void)
{
    capteur_aleatoire_init(TESTS_SEED);

    for (int hour = 0; hour < 24; ++hour) {
        for (int i = 0; i < 100; ++i) {
            float temperature = capteur_aleatoire(hour);

            TEST_ASSERT_TRUE(temperature >= TEMP_MIN);
            TEST_ASSERT_TRUE(temperature <= TEMP_MAX);
        }
    }
}

void test_capteur_aleatoire_is_reproducible_with_same_seed(void)
{
    float first_run[24];
    float second_run[24];

    capteur_aleatoire_init(TESTS_SEED);
    for (int hour = 0; hour < 24; ++hour) {
        first_run[hour] = capteur_aleatoire(hour);
    }

    capteur_aleatoire_init(TESTS_SEED);
    for (int hour = 0; hour < 24; ++hour) {
        second_run[hour] = capteur_aleatoire(hour);
    }

    for (int hour = 0; hour < 24; ++hour) {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, first_run[hour], second_run[hour]);
    }
}

static float stub_capteur(int heure)
{
    return (float)heure;
}

void test_collecter_releves_fills_expected_values(void)
{
    float tab[6];

    collecter_releves(tab, 6, stub_capteur);

    for (int i = 0; i < 6; ++i) {
        TEST_ASSERT_FLOAT_WITHIN(0.0001f, (float)i, tab[i]);
    }
}

void test_collecter_releves_does_not_write_past_n(void)
{
    float tab[6];
    for (int i = 0; i < 6; ++i) {
        tab[i] = -1.0f;
    }

    collecter_releves(tab, 3, stub_capteur);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, tab[0]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, tab[1]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 2.0f, tab[2]);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -1.0f, tab[3]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -1.0f, tab[4]);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, -1.0f, tab[5]);
}
