#ifndef TEST_RANDOM_SENSOR_H
#define TEST_RANDOM_SENSOR_H

void test_get_random_temperature_stays_in_allowed_range(void);
void test_capteur_aleatoire_is_reproducible_with_same_seed(void);
void test_collecter_releves_fills_expected_values(void);
void test_collecter_releves_does_not_write_past_n(void);

#endif // TEST_RANDOM_SENSOR_H
