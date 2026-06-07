/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Shehriyar Qureshi (thatdevsherry)
 */

#ifndef LIB_H
#define LIB_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SDL_MESSAGE_MIN 3
#define SDL_MESSAGE_MAX 255
#define SDL_MESSAGE_DATA_LEN_MAX 252
#define SDL_MESSAGE_DATA_ACTUATE_LEN 8

enum SdlMessageStatusCode {
  SDL_MESSAGE_OK = 0,
  SDL_MESSAGE_ERR_LEN_BELOW_MIN = -1,
  SDL_MESSAGE_ERR_INVALID_CHECKSUM = -2,
};

/*
 * OBD address representation.
 */
struct SdlObdAddress {
  uint8_t map[SDL_MESSAGE_DATA_LEN_MAX];
  uint8_t len;
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

#endif
