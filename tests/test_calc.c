/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Shehriyar Qureshi (thatdevsherry)
 */

#include "suzuki_sdl/suzuki_sdl.h"
#include "unity.h"

void setUp() {}
void tearDown() {}

struct RawProcessPair {
  uint8_t raw;
  float_t processed;
};

void test_desired_idle() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = 0},      {.raw = 1, .processed = 8},
      {.raw = 32, .processed = 251},   {.raw = 64, .processed = 502},
      {.raw = 96, .processed = 753},   {.raw = 128, .processed = 1004},
      {.raw = 160, .processed = 1255}, {.raw = 192, .processed = 1506},
      {.raw = 224, .processed = 1757}, {.raw = 255, .processed = 2000},
  };
  for (int i = 0; i < 10; i++) {
    uint8_t raw = inputs[i].raw;
    uint16_t processed = (uint16_t)inputs[i].processed;
    uint16_t result = sdl_calculate_desired_idle(raw);
    TEST_ASSERT_EQUAL_UINT16(processed, result);
  }
}

void test_rpm_high() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = 0},       {.raw = 1, .processed = 50},
      {.raw = 32, .processed = 1606},   {.raw = 64, .processed = 3213},
      {.raw = 96, .processed = 4819},   {.raw = 128, .processed = 6425},
      {.raw = 160, .processed = 8031},  {.raw = 192, .processed = 9638},
      {.raw = 224, .processed = 11244}, {.raw = 255, .processed = 12800},
  };
  for (int i = 0; i < 10; i++) {
    uint8_t raw = inputs[i].raw;
    uint16_t processed = (uint16_t)inputs[i].processed;
    uint16_t result = sdl_calculate_rpm_high(raw);
    TEST_ASSERT_EQUAL_UINT16(processed, result);
  }
}

void test_rpm_low() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = 0},    {.raw = 1, .processed = 0},
      {.raw = 32, .processed = 6},   {.raw = 64, .processed = 13},
      {.raw = 96, .processed = 19},  {.raw = 128, .processed = 25},
      {.raw = 160, .processed = 31}, {.raw = 192, .processed = 38},
      {.raw = 224, .processed = 44}, {.raw = 255, .processed = 50},
  };
  for (int i = 0; i < 10; i++) {
    uint8_t raw = inputs[i].raw;
    uint16_t processed = (uint16_t)inputs[i].processed;
    uint16_t result = sdl_calculate_rpm_low(raw);
    TEST_ASSERT_EQUAL_UINT16(processed, result);
  }
}

void test_isc_flow_duty() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = 0},
      {.raw = 128, .processed = 50},
      {.raw = 255, .processed = 100},
  };
  for (int i = 0; i < 3; i++) {
    uint8_t raw = inputs[i].raw;
    uint8_t processed = (uint8_t)inputs[i].processed;
    uint8_t result = sdl_calculate_isc_flow_duty(raw);
    TEST_ASSERT_EQUAL_UINT8(processed, result);
  }
}

void test_absolute_throttle_position() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = 0},
      {.raw = 128, .processed = 50},
      {.raw = 255, .processed = 100},
  };
  for (int i = 0; i < 3; i++) {
    uint8_t raw = inputs[i].raw;
    uint8_t processed = (uint8_t)inputs[i].processed;
    uint8_t result = sdl_calculate_absolute_throttle_position(raw);
    TEST_ASSERT_EQUAL_UINT8(processed, result);
  }
}

void test_tps_angle() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = 0},     {.raw = 1, .processed = 0},
      {.raw = 32, .processed = 16},   {.raw = 64, .processed = 31},
      {.raw = 96, .processed = 47},   {.raw = 128, .processed = 63},
      {.raw = 160, .processed = 78},  {.raw = 192, .processed = 94},
      {.raw = 224, .processed = 110}, {.raw = 255, .processed = 125},

  };
  for (int i = 0; i < 10; i++) {
    uint8_t raw = inputs[i].raw;
    uint8_t processed = (uint8_t)inputs[i].processed;
    uint8_t result = sdl_calculate_tps_angle(raw);
    TEST_ASSERT_EQUAL_UINT8(processed, result);
  }
}

void test_temperature() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = -40},   {.raw = 1, .processed = -39},
      {.raw = 2, .processed = -39},   {.raw = 4, .processed = -38},
      {.raw = 8, .processed = -35},   {.raw = 16, .processed = -30},
      {.raw = 32, .processed = -20},  {.raw = 64, .processed = 0},
      {.raw = 96, .processed = 20},   {.raw = 128, .processed = 40},
      {.raw = 160, .processed = 60},  {.raw = 176, .processed = 70},
      {.raw = 192, .processed = 80},  {.raw = 208, .processed = 90},
      {.raw = 224, .processed = 100}, {.raw = 240, .processed = 110},
      {.raw = 255, .processed = 119},
  };
  for (int i = 0; i < 17; i++) {
    uint8_t raw = inputs[i].raw;
    int8_t processed = (int8_t)inputs[i].processed;
    int8_t result = sdl_calculate_engine_coolant_temperature(raw);
    TEST_ASSERT_EQUAL_INT8(processed, result);
  }
}

void test_vehicle_speed() {
  for (uint8_t i = 0; i < 255; i++) {
    uint8_t result = sdl_calculate_vehicle_speed(i);
    TEST_ASSERT_EQUAL_UINT8(i, result);
  }
}

void test_ignition_advance() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = -12},  {.raw = 1, .processed = -12},
      {.raw = 32, .processed = -1},  {.raw = 64, .processed = 11},
      {.raw = 96, .processed = 22},  {.raw = 128, .processed = 33},
      {.raw = 160, .processed = 44}, {.raw = 192, .processed = 56},
      {.raw = 224, .processed = 67}, {.raw = 255, .processed = 78},

  };
  for (int i = 0; i < 10; i++) {
    uint8_t raw = inputs[i].raw;
    int8_t processed = (int8_t)inputs[i].processed;
    int8_t result = sdl_calculate_ignition_advance(raw);
    TEST_ASSERT_EQUAL_INT8(processed, result);
  }
}

void test_injector_pulse_width_high() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = 0.000f},     {.raw = 1, .processed = 0.512f},
      {.raw = 32, .processed = 16.384f},   {.raw = 64, .processed = 32.768f},
      {.raw = 96, .processed = 49.152f},   {.raw = 128, .processed = 65.536f},
      {.raw = 160, .processed = 81.920f},  {.raw = 192, .processed = 98.304f},
      {.raw = 224, .processed = 114.688f}, {.raw = 255, .processed = 130.560f},
  };
  for (int i = 0; i < 10; i++) {
    uint8_t raw = inputs[i].raw;
    float_t processed = inputs[i].processed;
    float_t result = sdl_calculate_injector_pulse_width_high(raw);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, processed, result);
  }
}

void test_injector_pulse_width_low() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = 0.000f},   {.raw = 1, .processed = 0.002f},
      {.raw = 32, .processed = 0.064f},  {.raw = 64, .processed = 0.128f},
      {.raw = 96, .processed = 0.192f},  {.raw = 128, .processed = 0.256f},
      {.raw = 160, .processed = 0.320f}, {.raw = 192, .processed = 0.384f},
      {.raw = 224, .processed = 0.448f}, {.raw = 255, .processed = 0.510f},
  };
  for (int i = 0; i < 10; i++) {
    uint8_t raw = inputs[i].raw;
    float_t processed = inputs[i].processed;
    float_t result = sdl_calculate_injector_pulse_width_low(raw);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, processed, result);
  }
}

void test_pressure() {
  struct RawProcessPair inputs[] = {
      {.raw = 0, .processed = -20},   {.raw = 1, .processed = -19},
      {.raw = 32, .processed = 1},    {.raw = 64, .processed = 22},
      {.raw = 96, .processed = 43},   {.raw = 128, .processed = 64},
      {.raw = 160, .processed = 85},  {.raw = 192, .processed = 105},
      {.raw = 224, .processed = 126}, {.raw = 255, .processed = 147},
  };
  for (int i = 0; i < 10; i++) {
    uint8_t raw = inputs[i].raw;
    int16_t processed = (int16_t)inputs[i].processed;
    int16_t result = (int16_t)roundf(sdl_calculate_pressure(raw));
    TEST_ASSERT_EQUAL_INT16(result, processed);
  }
}

void test_battery_voltage() {
  struct RawProcessPair inputs[] = {{.raw = 0, .processed = 0},
                                    {.raw = 128, .processed = 10.0f},
                                    {.raw = 255, .processed = 20.0f}};
  for (int i = 0; i < 3; i++) {
    uint8_t raw = inputs[i].raw;
    float_t processed = inputs[i].processed;
    float_t result = sdl_calculate_battery_voltage(raw);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, processed, result);
  }
}
void test_radiator_fan() {
  bool result_off = sdl_calculate_radiator_fan_switch(0);
  bool result_on = sdl_calculate_radiator_fan_switch(128);
  TEST_ASSERT_EQUAL_UINT8(false, result_off);
  TEST_ASSERT_EQUAL_UINT8(true, result_on);
}

void test_status_flag_psp_switch() {
  uint8_t raw_on = 1 << 1;
  uint8_t raw_off = 0;
  bool result_on = sdl_calculate_psp_switch(raw_on);
  bool result_off = sdl_calculate_psp_switch(raw_off);
  TEST_ASSERT_EQUAL(true, result_on);
  TEST_ASSERT_EQUAL(false, result_off);
}

void test_status_flag_ac_switch() {
  uint8_t raw_on = 1 << 2;
  uint8_t raw_off = 0;
  bool result_on = sdl_calculate_ac_switch(raw_on);
  bool result_off = sdl_calculate_ac_switch(raw_off);
  TEST_ASSERT_EQUAL(true, result_on);
  TEST_ASSERT_EQUAL(false, result_off);
}

void test_status_flag_closed_throttle_position() {
  uint8_t raw_on = 1 << 4;
  uint8_t raw_off = 0;
  bool result_on = sdl_calculate_closed_throttle_position_switch(raw_on);
  bool result_off = sdl_calculate_closed_throttle_position_switch(raw_off);
  TEST_ASSERT_EQUAL(true, result_on);
  TEST_ASSERT_EQUAL(false, result_off);
}

void test_status_flag_electric_load_switch() {
  uint8_t raw_on = 1 << 6;
  uint8_t raw_off = 0;
  bool result_on = sdl_calculate_electric_load_switch(raw_on);
  bool result_off = sdl_calculate_electric_load_switch(raw_off);
  TEST_ASSERT_EQUAL(true, result_on);
  TEST_ASSERT_EQUAL(false, result_off);
}

void test_fault_codes() {}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_desired_idle);
  RUN_TEST(test_rpm_high);
  RUN_TEST(test_rpm_low);
  RUN_TEST(test_isc_flow_duty);
  RUN_TEST(test_absolute_throttle_position);
  RUN_TEST(test_tps_angle);
  RUN_TEST(test_temperature);
  RUN_TEST(test_vehicle_speed);
  RUN_TEST(test_ignition_advance);
  RUN_TEST(test_injector_pulse_width_high);
  RUN_TEST(test_injector_pulse_width_low);
  RUN_TEST(test_pressure);
  RUN_TEST(test_battery_voltage);
  RUN_TEST(test_radiator_fan);
  RUN_TEST(test_status_flag_ac_switch);
  RUN_TEST(test_status_flag_psp_switch);
  RUN_TEST(test_status_flag_closed_throttle_position);
  RUN_TEST(test_status_flag_electric_load_switch);
  return UNITY_END();
}
