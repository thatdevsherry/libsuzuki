/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Shehriyar Qureshi (thatdevsherry)
 */

#include "suzuki_sdl/suzuki_sdl.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

struct SdlMessage
sdl_message_data_request_create(enum SdlHeaderModule module,
                                const struct SdlObdAddress *map) {
  uint8_t hdr = (uint8_t)module | (uint8_t)SDL_HEADER_TYPE_DATA_REQUEST;
  uint8_t len_msg = //
      1 +           // header byte
      1 +           // length byte
      map->len +    // data bytes
      1;            // checksum byte
  uint8_t data[len_msg - 3];

  for (int i = 0; i < len_msg - 3; i++) {
    data[i] = (uint8_t)map->map[i];
  }
  struct SdlMessage msg = {.header = hdr, .length = len_msg};
  memcpy(msg.data, data, len_msg - 3);
  sdl_message_checksum_update(&msg);
  return msg;
}

struct SdlMessage sdl_message_id_request_create(enum SdlHeaderModule module) {
  uint8_t hdr = (uint8_t)module | (uint8_t)SDL_HEADER_TYPE_ID;
  uint8_t len = SDL_MESSAGE_MIN; /* hdr + len + checksum */
  struct SdlMessage msg = {.header = hdr, .length = len};
  sdl_message_checksum_update(&msg);
  return msg;
}

struct SdlMessage
sdl_message_actuation_request_create(enum SdlHeaderModule module,
                                     const uint8_t *data) {
  uint8_t hdr = (uint8_t)module | (uint8_t)SDL_HEADER_TYPE_ACTUATE;
  uint8_t length = 1 + SDL_MESSAGE_DATA_ACTUATE_LEN + 1;
  struct SdlMessage msg = {.header = hdr, .length = length};
  memcpy(msg.data, data, SDL_MESSAGE_DATA_ACTUATE_LEN);
  sdl_message_checksum_update(&msg);
  return msg;
}

struct SdlMessage sdl_message_dtc_clear_create(enum SdlHeaderModule module) {
  uint8_t hdr = (uint8_t)module | (uint8_t)SDL_HEADER_TYPE_DTC_CLEAR;
  uint8_t length = 1 + 1 + 1;
  struct SdlMessage msg = {.header = hdr, .length = length};
  sdl_message_checksum_update(&msg);
  return msg;
}

uint8_t sdl_message_serialize(uint8_t *buf, const struct SdlMessage *msg) {
  buf[0] = msg->header;
  buf[1] = msg->length;
  memcpy(&buf[2], msg->data, msg->length - 1);
  buf[msg->length - 1] = msg->checksum;
  return msg->length;
}

enum SdlMessageStatusCode sdl_message_deserialize(const uint8_t *buf,
                                                  struct SdlMessage *msg) {
  uint8_t len = buf[1];
  if (len < 3) {
    return SDL_MESSAGE_ERR_LEN_BELOW_MIN;
  }
  msg->header = buf[0];
  msg->length = len;
  if (len > 3) {
    memcpy(msg->data, buf + 2, buf[1] - 3);
  }
  msg->checksum = buf[len - 1];
  return SDL_MESSAGE_OK;
}

uint8_t sdl_message_checksum_generate(const uint8_t *buf, uint8_t len) {
  uint8_t sum = 0;
  for (uint8_t i = 0; i < len; i++) {
    sum += buf[i];
  }
  return (uint8_t)(-sum);
}

void sdl_message_checksum_update(struct SdlMessage *msg) {
  uint8_t buf[255] = {0};
  uint8_t len = sdl_message_serialize(buf, msg);
  uint8_t checksum = sdl_message_checksum_generate(buf, len);
  msg->checksum = checksum;
}

enum SdlMessageStatusCode sdl_message_is_valid(const struct SdlMessage *msg) {
  uint8_t sum = 0;
  sum += msg->header;
  sum += msg->length;
  for (int i = 0; i < SDL_MESSAGE_DATA_LEN_MAX; i++) {
    sum += msg->data[i];
  }
  sum += msg->checksum;

  return sum == 0x00 ? SDL_MESSAGE_OK : SDL_MESSAGE_ERR_INVALID_CHECKSUM;
}

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
