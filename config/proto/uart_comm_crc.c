/*
 * Copyright (c) 2022-2023 XiNGRZ
 * SPDX-License-Identifier: MIT
 */

#include <stddef.h>
#include <stdint.h>

#include "uart_comm_crc.h"

#define CRC16_POLY 0x1021
#define CRC16_INIT 0xFFFF

uint16_t uart_comm_crc16(const uint8_t *data, size_t len)
{
	uint16_t crc = CRC16_INIT;

	for (size_t i = 0; i < len; i++) {
		crc ^= (uint16_t)data[i] << 8;
		for (int bit = 0; bit < 8; bit++) {
			crc = (crc & 0x8000) ? (crc << 1) ^ CRC16_POLY : (crc << 1);
		}
	}

	return crc;
}
