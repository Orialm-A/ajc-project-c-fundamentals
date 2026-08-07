/**
 * SPDX-License-Identifier: MIT
 * Copyright © 2026
 */

// --- includes --------------------------------------
#include "unity.h"             // Needed for Unity assertions
#include "capteur.h"        // Module under test
#include "test_csv_sensor.h"

#include <stdio.h>

// --- defines and macros ----------------------------
#define TEST_CSV_PATH "test_csv_tmp.csv"

// --- function implementations (scope: module) ------
static void write_test_csv(const char *content)
{
    FILE *f = fopen(TEST_CSV_PATH, "w");
    fputs(content, f);
    fclose(f);
}

static void remove_test_csv(void)
{
    remove(TEST_CSV_PATH);
}

// --- function implementations (scope: public) ------

void test_capteur_csv_init_valid_file_returns_count(void)
{
    write_test_csv("10.0\n20.0\n30.0\n");

    int count = capteur_csv_init(TEST_CSV_PATH);

    TEST_ASSERT_EQUAL_INT(3, count);

    capteur_csv_fermer();
    remove_test_csv();
}

void test_capteur_csv_init_missing_file_returns_minus_one(void)
{
    int count = capteur_csv_init("this_file_does_not_exist.csv");

    TEST_ASSERT_EQUAL_INT(-1, count);
}

void test_capteur_csv_init_ignores_comments_and_blank_lines(void)
{
    write_test_csv("# commentaire\n\n10.0\n# encore un commentaire\n20.0\n");

    int count = capteur_csv_init(TEST_CSV_PATH);

    TEST_ASSERT_EQUAL_INT(2, count);

    capteur_csv_fermer();
    remove_test_csv();
}

void test_capteur_csv_returns_value_for_valid_index(void)
{
    write_test_csv("11.0\n22.0\n33.0\n");
    capteur_csv_init(TEST_CSV_PATH);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 11.0f, capteur_csv(0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 22.0f, capteur_csv(1));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 33.0f, capteur_csv(2));

    capteur_csv_fermer();
    remove_test_csv();
}

void test_capteur_csv_returns_sentinel_for_out_of_bounds_index(void)
{
    write_test_csv("11.0\n22.0\n");
    capteur_csv_init(TEST_CSV_PATH);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, TEMP_MIN - 1.0f, capteur_csv(5));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, TEMP_MIN - 1.0f, capteur_csv(-1));

    capteur_csv_fermer();
    remove_test_csv();
}

void test_capteur_csv_returns_sentinel_before_init(void)
{
    // Force a known "not ready" state regardless of what earlier tests left behind
    capteur_csv_fermer();

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, TEMP_MIN - 1.0f, capteur_csv(0));
}

void test_capteur_csv_init_clamps_out_of_range_values(void)
{
    write_test_csv("-999.0\n999.0\n");
    capteur_csv_init(TEST_CSV_PATH);

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, TEMP_MIN, capteur_csv(0));
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, TEMP_MAX, capteur_csv(1));

    capteur_csv_fermer();
    remove_test_csv();
}

void test_capteur_csv_fermer_resets_state(void)
{
    write_test_csv("11.0\n22.0\n");
    capteur_csv_init(TEST_CSV_PATH);

    capteur_csv_fermer();

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, TEMP_MIN - 1.0f, capteur_csv(0));

    remove_test_csv();
}

void test_capteur_csv_init_twice_without_fermer_does_not_overflow(void)
{
    write_test_csv("1.0\n2.0\n3.0\n");

    capteur_csv_init(TEST_CSV_PATH);
    int second_count = capteur_csv_init(TEST_CSV_PATH);

    TEST_ASSERT_EQUAL_INT(3, second_count);

    capteur_csv_fermer();
    remove_test_csv();
}
