/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Shehriyar Qureshi (thatdevsherry)
 */

#include "test_lib.h"
#include "suzuki_sdl/suzuki_sdl.h"
#include "suzuki_sdl/vehicles.h"
#include "unity.h"
#include <string.h>

void setUp() {}
void tearDown() {}

void test_deserialize_get_command() {
  struct SdlMessage msg = {};
  enum SdlMessageStatusCode ret =
      sdl_message_deserialize(sdl_message_get_command_bytes, &msg);
  TEST_ASSERT_EQUAL(SDL_MESSAGE_OK, ret);
  TEST_ASSERT_EQUAL_HEX(0x10, msg.header);
  TEST_ASSERT_EQUAL_HEX(0x03, msg.length);
  TEST_ASSERT_EQUAL_HEX(0xED, msg.checksum);
  uint8_t data[251] = {0};
  TEST_ASSERT_EQUAL_UINT8_ARRAY(data, msg.data, 251);
}

void test_deserialize_obd_query_command() {
  struct SdlMessage msg = {};
  enum SdlMessageStatusCode ret =
      sdl_message_deserialize(sdl_message_obd_query_command_bytes, &msg);
  TEST_ASSERT_EQUAL(SDL_MESSAGE_OK, ret);
  TEST_ASSERT_EQUAL_HEX(0x13, msg.header);
  TEST_ASSERT_EQUAL_HEX(0x05, msg.length);
  TEST_ASSERT_EQUAL_HEX(0xEF, msg.checksum);
  uint8_t data[251] = {
      0x0A,
      0x0B,
  };
  TEST_ASSERT_EQUAL_HEX8_ARRAY(data, msg.data, 251);
}

void test_serialize_get_command() {
  uint8_t buf[255] = {0};
  uint8_t len = sdl_message_serialize(buf, &sdl_message_get_command);
  TEST_ASSERT_EQUAL_UINT8(3, len);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(sdl_message_get_command_bytes, buf, 3);
}

void test_serialize_obd_query_command() {
  uint8_t buf[255] = {0};
  uint8_t len = sdl_message_serialize(buf, &sdl_message_obd_query_command);
  TEST_ASSERT_EQUAL_UINT8(5, len);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(sdl_message_obd_query_command_bytes, buf, 5);
}

void test_sdl_message_data_request_create_single() {
  enum SdlHeaderModule module = SDL_HEADER_MODULE_ECU;
  uint8_t map[] = {SDL_OBD_ADDRESS_BALENO_33920_65GP_VEHICLE_SPEED_SENSOR};
  uint8_t len = 1;
  struct SdlObdAddress obd_addr = {.len = len};
  memcpy(obd_addr.map, map, len);
  struct SdlMessage msg = sdl_message_data_request_create(module, &obd_addr);
  uint8_t expected_data[] = {
      SDL_OBD_ADDRESS_BALENO_33920_65GP_VEHICLE_SPEED_SENSOR};
  TEST_ASSERT_EQUAL_HEX8(0x13, msg.header);
  TEST_ASSERT_EQUAL_HEX8(0x04, msg.length);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_data, msg.data, 1);
  TEST_ASSERT_EQUAL_HEX8(0xE2, msg.checksum);
}

void test_sdl_message_data_request_create_exceed_len() {
  enum SdlHeaderModule module = SDL_HEADER_MODULE_ECU;

  uint8_t len = 252;
  uint8_t map[len];
  for (int i = 0; i < len; i++) {
    map[i] = SDL_OBD_ADDRESS_BALENO_33920_65GP_VEHICLE_SPEED_SENSOR;
  }
  map[251] = SDL_OBD_ADDRESS_BALENO_33920_65GP_RADIATOR_FAN;
  struct SdlObdAddress obd_addr = {.len = len};
  memcpy(obd_addr.map, map, len);
  struct SdlMessage msg = sdl_message_data_request_create(module, &obd_addr);
  uint8_t expected_data[252];
  for (int i = 0; i < len; i++) {
    expected_data[i] = SDL_OBD_ADDRESS_BALENO_33920_65GP_VEHICLE_SPEED_SENSOR;
  }
  expected_data[251] = SDL_OBD_ADDRESS_BALENO_33920_65GP_RADIATOR_FAN;
  TEST_ASSERT_EQUAL_HEX8(0x13, msg.header);
  TEST_ASSERT_EQUAL_HEX8(0xFF, msg.length);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_data, msg.data, 252);
  TEST_ASSERT_EQUAL_HEX8(0xF8, msg.checksum);
}
void test_sdl_message_id_request_create() {
  struct SdlMessage msg = sdl_message_id_request_create(SDL_HEADER_MODULE_ECU);
  TEST_ASSERT_EQUAL_HEX8(0x10, msg.header);
  TEST_ASSERT_EQUAL_HEX8(0x03, msg.length);
  TEST_ASSERT_EQUAL_HEX8(0xED, msg.checksum);
}

void test_sdl_message_actuation_request_create() {
  uint8_t expected_data[] = {0xC0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  enum SdlHeaderModule module = SDL_HEADER_MODULE_ECU;
  struct SdlMessage msg =
      sdl_message_actuation_request_create(module, expected_data);
  TEST_ASSERT_EQUAL_HEX8(0x15, msg.header);
  TEST_ASSERT_EQUAL_HEX8(0x0A, msg.length);
  TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_data, msg.data,
                               SDL_MESSAGE_DATA_ACTUATE_LEN);
  TEST_ASSERT_EQUAL_HEX8(0x22, msg.checksum);
}

void test_sdl_message_checksum_verify_valid() {
  struct SdlMessage msg = sdl_message_id_request_create(SDL_HEADER_MODULE_ECU);
  enum SdlMessageStatusCode ret = sdl_message_is_valid(&msg);
  TEST_ASSERT_EQUAL(SDL_MESSAGE_OK, ret);
}

void test_sdl_message_checksum_verify_invalid() {
  uint8_t invalid_cmd[3] = {0x10, 0x03, 0xFF};
  struct SdlMessage msg;
  enum SdlMessageStatusCode ret = sdl_message_deserialize(invalid_cmd, &msg);
  TEST_ASSERT_EQUAL(SDL_MESSAGE_OK, ret);
  ret = sdl_message_is_valid(&msg);
  TEST_ASSERT_EQUAL(SDL_MESSAGE_ERR_INVALID_CHECKSUM, ret);
}

void test_sdl_message_dtc_clear_create() {
  enum SdlHeaderModule module = SDL_HEADER_MODULE_ECU;
  struct SdlMessage msg = sdl_message_dtc_clear_create(module);
  TEST_ASSERT_EQUAL_HEX8(0x14, msg.header);
  TEST_ASSERT_EQUAL_HEX8(0x03, msg.length);
  TEST_ASSERT_EQUAL_HEX8(0xE9, msg.checksum);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_deserialize_get_command);
  RUN_TEST(test_deserialize_obd_query_command);
  RUN_TEST(test_serialize_get_command);
  RUN_TEST(test_serialize_obd_query_command);
  RUN_TEST(test_sdl_message_data_request_create_single);
  RUN_TEST(test_sdl_message_data_request_create_exceed_len);
  RUN_TEST(test_sdl_message_id_request_create);
  RUN_TEST(test_sdl_message_actuation_request_create);
  RUN_TEST(test_sdl_message_checksum_verify_valid);
  RUN_TEST(test_sdl_message_checksum_verify_invalid);
  RUN_TEST(test_sdl_message_dtc_clear_create);
  return UNITY_END();
}
