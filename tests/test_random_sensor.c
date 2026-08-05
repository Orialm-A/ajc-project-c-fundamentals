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
