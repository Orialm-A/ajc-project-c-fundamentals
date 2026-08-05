/**
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 */

// --- includes --------------------------------------
#include "unity.h"             // Needed for Unity assertions
#include "statistics.h"        // Module under test
#include "test_statistics.h"   // Our test declarations

// --- defines and macros ----------------------------
#define NUMBER_TEMPERATURE 5u

// --- typedefs and structures -----------------------
typedef struct TemperatureTests {
    float temperatures[NUMBER_TEMPERATURE];
    float expected_average;
    float expected_min;
    float expected_max;
    float expected_amplitude;
} temperatures_tests_t;

// Place in test_statistics.c (file scope)
static temperatures_tests_t TEST_CASES[] = {
    {   // easy
        .temperatures = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f},
        .expected_average = 30.0f,
        .expected_min = 10.0f,
        .expected_max = 50.0f,
        .expected_amplitude = 40.0f
    },
    {   // Mixed
        .temperatures = {-5.3f, 0.0f, 51.0f, -23.7f, 28.9f},
        .expected_average = 10.18f,
        .expected_min = -23.7f,
        .expected_max = 51.0f,
        .expected_amplitude = 74.7f
    },
    {   // limits
        .temperatures = {100.0f, 100.0f, 100.0f, 100.0f, 100.0f},
        .expected_average = 100.0f,
        .expected_min = 100.0f,
        .expected_max = 100.0f,
        .expected_amplitude = 0.0f
    }
};

// Helper macro to count test cases cleanly
#define NUM_TEST_CASES (sizeof(TEST_CASES) / sizeof(TEST_CASES[0]))

// --- function implementations (scope: public) ------

void tests_calculer_moyenne(void) {
    for (size_t test_id = 0; test_id < NUM_TEST_CASES; test_id++) {
        float average = calculer_moyenne(TEST_CASES[test_id].temperatures, NUMBER_TEMPERATURE);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, TEST_CASES[test_id].expected_average, average);
    }
}

void tests_trouver_minimum(void) {
    for (size_t test_id = 0; test_id < NUM_TEST_CASES; test_id++) {
        int min_index = DEFAULT_INDEX;
        float min =  trouver_minimum(TEST_CASES[test_id].temperatures, NUMBER_TEMPERATURE, &min_index);

        TEST_ASSERT_FLOAT_WITHIN(0.001f, TEST_CASES[test_id].expected_min, min);

        TEST_ASSERT_NOT_EQUAL(-1, min_index);
    }
}

void tests_trouver_maximum(void) {
    for (size_t test_id = 0; test_id < NUM_TEST_CASES; test_id++) {
        int max_index = DEFAULT_INDEX;
        float max = trouver_maximum(TEST_CASES[test_id].temperatures, NUMBER_TEMPERATURE, &max_index);

        TEST_ASSERT_FLOAT_WITHIN(0.001f, TEST_CASES[test_id].expected_max, max);

        TEST_ASSERT_NOT_EQUAL(-1, max_index);
    }
}

void tests_calculer_amplitude(void) {
    for (size_t test_id = 0; test_id < NUM_TEST_CASES; test_id++) {
        float amplitude = calculer_amplitude(TEST_CASES[test_id].temperatures, NUMBER_TEMPERATURE);
        TEST_ASSERT_FLOAT_WITHIN(0.001f, TEST_CASES[test_id].expected_amplitude, amplitude);
    }
}
