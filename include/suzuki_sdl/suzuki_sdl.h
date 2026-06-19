/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Shehriyar Qureshi (thatdevsherry)
 */

#ifndef SUZUKI_SDL
#define SUZUKI_SDL

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SDL_MESSAGE_MIN 3
#define SDL_MESSAGE_MAX 255
#define SDL_MESSAGE_DATA_LEN_MAX 252
#define SDL_MESSAGE_DATA_ACTUATE_LEN 8

/*
 * Suzuki Serial Data Line (SDL) message format.
 *
 * It is an in-memory representation. See `sdl_message_serialize` for converting
 * to wire-format.
 */
struct SdlMessage {
  uint8_t header;    /* module + type nibble. */
  uint8_t length;    /* total length of message, from header to checksum. */
  uint8_t data[252]; /* Data fields, can never exceed 252 as length byte is
                        8-bit. */
  uint8_t checksum;  /* two's compliment checksum. */
};

/*
 * OBD address representation.
 */
struct SdlObdAddress {
  uint8_t map[SDL_MESSAGE_DATA_LEN_MAX];
  uint8_t len;
};

enum SdlMessageStatusCode {
  SDL_MESSAGE_OK = 0,
  SDL_MESSAGE_ERR_LEN_BELOW_MIN = -1,
  SDL_MESSAGE_ERR_INVALID_CHECKSUM = -2,
};

/*
 * First nibble of SdlMessage header. Denotes the unit/module the request is
 * for.
 */
enum SdlHeaderModule {
  SDL_HEADER_MODULE_ECU = 0x10,
};

/*
 * Second nibble of SdlMessage header. Denotes request type.
 */
enum SdlHeaderType {
  SDL_HEADER_TYPE_ID = 0x00,           /* Module ID */
  SDL_HEADER_TYPE_DATA_REQUEST = 0x03, /* For making parameter requests */
  SDL_HEADER_TYPE_DTC_CLEAR = 0x04,    /* Performing DTC clear */
  SDL_HEADER_TYPE_ACTUATE = 0x05,      /* Actuations controlled by module */
};

/*
 * Create sdl message of type data request for provided obd addresses.
 */
struct SdlMessage
sdl_message_data_request_create(enum SdlHeaderModule module,
                                const struct SdlObdAddress *map);

/*
 * Create sdl message of type id request for provided module.
 */
struct SdlMessage sdl_message_id_request_create(enum SdlHeaderModule module);

/*
 * Create sdl message of type actuation request with provided data.
 */
struct SdlMessage
sdl_message_actuation_request_create(enum SdlHeaderModule module,
                                     const uint8_t *data);

/*
 * Create sdl message of type 'dtc clear" for provided module.
 */
struct SdlMessage sdl_message_dtc_clear_create(enum SdlHeaderModule module);

/*
 * Serialize sdl message into provided buf.
 *
 * Ensure the buffer is big enough to hold the serialized message.
 *
 * If memory space is not an issue, it is safer to just pass a buf of 255 bytes
 * as the sdl message cannot exceed it.
 *
 * Returns length of serialized message in the buf.
 */
uint8_t sdl_message_serialize(uint8_t *buf, const struct SdlMessage *msg);

/*
 * Convert bytes into Sdl Message.
 *
 * Does not take length as it is present in Message format itself. Caller should
 * make sure to provide pointer to header byte, otherwise it will definitely
 * read out of bounds.
 */
enum SdlMessageStatusCode sdl_message_deserialize(const uint8_t *buf,
                                                  struct SdlMessage *msg);

/*
 * Generates checksum byte from buffer.
 *
 * Length is required because it is used for calculation. It should be correct
 * beforehand as this function does not modify the array.
 */
uint8_t sdl_message_checksum_generate(const uint8_t *buf, uint8_t len);

/*
 * Create checksum of a sdl message and add it to the sdl message as well.
 */
void sdl_message_checksum_update(struct SdlMessage *msg);

/*
 * Validate message using checksum.
 */
enum SdlMessageStatusCode sdl_message_is_valid(const struct SdlMessage *msg);

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
