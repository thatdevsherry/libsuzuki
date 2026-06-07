/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Shehriyar Qureshi (thatdevsherry)
 */

#include "suzuki_sdl/calc.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

int8_t sdl_calculate_temperature(uint8_t v) {
  return (int8_t)roundf(((float_t)v / 255.0f) * 159.0f - 40.0f);
}

float_t sdl_calculate_pressure(uint8_t v) {
  return ((float_t)v / 255.0f) * (146.63f - (-20.0f)) + (-20.0f);
}

float_t sdl_calculate_injector_pulse_width(uint8_t high, uint8_t low) {
  return sdl_calculate_injector_pulse_width_high(high) +
         sdl_calculate_injector_pulse_width_low(low);
}

uint16_t sdl_calculate_engine_speed(uint8_t high, uint8_t low) {
  return sdl_calculate_rpm_high(high) + sdl_calculate_rpm_low(low);
}

uint8_t sdl_calculate_isc_flow_duty(uint8_t v) {
  return (uint8_t)roundf(((float_t)v / 255.0f) * 100.0f);
}

uint8_t sdl_calculate_absolute_throttle_position(uint8_t v) {
  return (uint8_t)roundf(((float_t)v / 255.0f) * 100.0f);
}

uint16_t sdl_calculate_rpm_high(uint8_t v) {
  return (uint16_t)roundf((float_t)(((uint16_t)v << 8) | (uint16_t)0) / 5.1f);
}

uint16_t sdl_calculate_rpm_low(uint8_t v) {
  return (uint16_t)roundf((float_t)(((uint16_t)0 << 8) | (uint16_t)v) / 5.1f);
}

float_t sdl_calculate_injector_pulse_width_high(uint8_t v) {
  return (float_t)(((uint16_t)v << 8) | (uint16_t)0) * 0.002f;
}

float_t sdl_calculate_injector_pulse_width_low(uint8_t v) {
  return (float_t)((((uint16_t)0) << 8) | (uint16_t)v) * 0.002f;
}

int8_t sdl_calculate_engine_coolant_temperature(uint8_t v) {
  return sdl_calculate_temperature(v);
}

int8_t sdl_calculate_intake_air_temperature(uint8_t v) {
  return sdl_calculate_temperature(v);
}

float_t sdl_calculate_manifold_absolute_pressure(uint8_t v) {
  return sdl_calculate_pressure(v);
}

float_t sdl_calculate_barometric_pressure(uint8_t v) {
  return sdl_calculate_pressure(v);
}

uint8_t sdl_calculate_tps_angle(uint8_t v) {
  return (uint8_t)roundf(((float_t)v * 125.0f) / 255.0f);
}

uint16_t sdl_calculate_desired_idle(uint8_t v) {
  float_t result = roundf(v * 7.84375f);
  return (uint16_t)result;
}

int8_t sdl_calculate_ignition_advance(uint8_t v) {
  return (int8_t)roundf(((float_t)v / 255.0f) * (78.0f - (-12.0f)) + (-12.0f));
}

float_t sdl_calculate_battery_voltage(uint8_t v) {
  return (float_t)((float_t)v * 0.0787f);
}

uint8_t sdl_calculate_vehicle_speed(uint8_t v) { return v; }

bool sdl_calculate_psp_switch(uint8_t v) {
  return ((v & (1 << 1)) != 0) ? 1 : 0;
}

bool sdl_calculate_ac_switch(uint8_t v) {
  return ((v & (1 << 2)) != 0) ? 1 : 0;
}

bool sdl_calculate_closed_throttle_position_switch(uint8_t v) {
  return ((v & (1 << 4)) != 0) ? 1 : 0;
}

bool sdl_calculate_electric_load_switch(uint8_t v) {
  return ((v & (1 << 6)) != 0) ? 1 : 0;
}

bool sdl_calculate_radiator_fan_switch(uint8_t v) { return v == 128 ? 1 : 0; }

float_t sdl_calculate_engine_load(int8_t iat, float_t map, float_t baro) {
  return (map / baro) * (293.15f / ((float_t)iat + 273.15f)) * 100.0f;
}
