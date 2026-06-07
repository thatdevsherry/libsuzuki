/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Shehriyar Qureshi (thatdevsherry)
 */

#ifndef CALC_H
#define CALC_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

/*
 * Calculate temperature (in celsius) from raw value.
 */
int8_t sdl_calculate_temperature(uint8_t v);

/*
 * Calculate pressure (in kPa) from raw value.
 */
float_t sdl_calculate_pressure(uint8_t v);

/*
 * Calculate injector pulse width (in ms) from high and low byte.
 */
float_t sdl_calculate_injector_pulse_width(uint8_t high, uint8_t low);

/*
 * Calculate engine speed (in RPM) from high and low byte.
 */
uint16_t sdl_calculate_engine_speed(uint8_t high, uint8_t low);

/*
 * Calculate ISC flow duty (in percentage) from raw value.
 */
uint8_t sdl_calculate_isc_flow_duty(uint8_t v);

/*
 * Calculate absolute throttle position (in percentage) from raw value.
 */
uint8_t sdl_calculate_absolute_throttle_position(uint8_t v);

/*
 * Calculate engine speed (in RPM) high portion from high byte.
 */
uint16_t sdl_calculate_rpm_high(uint8_t v);

/*
 * Calculate engine speed (in RPM) low portion from low byte.
 */
uint16_t sdl_calculate_rpm_low(uint8_t v);

/*
 * Calculate injector pulse width (in ms) high portion from high byte.
 */
float_t sdl_calculate_injector_pulse_width_high(uint8_t v);

/*
 * Calculate injector pulse width (in ms) low portion from low byte.
 */
float_t sdl_calculate_injector_pulse_width_low(uint8_t v);

/*
 * Calculate engine coolant temperature (in celsius) from raw value.
 */
int8_t sdl_calculate_engine_coolant_temperature(uint8_t v);

/*
 * Calculate intake air temperature (in celsius) from raw value.
 */
int8_t sdl_calculate_intake_air_temperature(uint8_t v);

/*
 * Calculate manifold absolute pressure (in kPa) from raw value.
 */
float_t sdl_calculate_manifold_absolute_pressure(uint8_t v);

/*
 * Calculate barometric pressure (in kPa) from raw value.
 */
float_t sdl_calculate_barometric_pressure(uint8_t v);

/*
 * Calculate TPS angle (in degrees) from raw value.
 */
uint8_t sdl_calculate_tps_angle(uint8_t v);

/*
 * Calculate desired idle (in RPM) from raw value.
 */
uint16_t sdl_calculate_desired_idle(uint8_t v);

/*
 * Calculate ignition advance (in degrees BTDC) from raw value.
 */
int8_t sdl_calculate_ignition_advance(uint8_t v);

/*
 * Calculate battery voltage (in volts) reported by ECU.
 */
float_t sdl_calculate_battery_voltage(uint8_t v);

/*
 * Calculate vehicle speed (in km/h) from raw value.
 */
uint8_t sdl_calculate_vehicle_speed(uint8_t v);

/*
 * Calculate if PSP (Power Steering Pump) switch is closed.
 */
bool sdl_calculate_psp_switch(uint8_t v);

/*
 * Calculate if AC (Air conditioning) switch is closed.
 */
bool sdl_calculate_ac_switch(uint8_t v);

/*
 * Calculate if throttle is closed.
 */
bool sdl_calculate_closed_throttle_position_switch(uint8_t v);

/*
 * Calculate if electric load (EL) switch is closed.
 */
bool sdl_calculate_electric_load_switch(uint8_t v);

/*
 * Calculate if radiator fan switch is closed.
 */
bool sdl_calculate_radiator_fan_switch(uint8_t v);

/*
 * Calculate engine load from parameters.
 *
 * This is not provided by ECU and is a custom calculation.
 *
 * It is supposed to be an approximation.
 *
 * NOTE: Formula is AI generated. Treat it as a lie if something looks off.
 */
float_t sdl_calculate_engine_load(int8_t iat, float_t map, float_t baro);

#endif
