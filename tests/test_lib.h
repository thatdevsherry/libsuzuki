/*
 * SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 Shehriyar Qureshi (thatdevsherry)
 */

#include "suzuki_sdl/lib.h"

struct SdlMessage sdl_message_get_command = {
    .header = 0x10, .length = 0x03, .data = {0}, .checksum = 0xED};
struct SdlMessage sdl_message_obd_query_command = {
    .header = 0x13, .length = 0x05, .data = {0x0A, 0x0B}, .checksum = 0xEF};
uint8_t sdl_message_get_command_bytes[] = {0x10, 0x03, 0xED};
uint8_t sdl_message_obd_query_command_bytes[] = {0x13, 0x05, 0x0A, 0x0B, 0xEF};
