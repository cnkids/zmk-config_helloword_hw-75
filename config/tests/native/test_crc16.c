/*
 * Native unit tests for uart_comm_crc16
 *
 * Run with: gcc test_main.c test_crc16.c ../../proto/uart_comm_crc.c
 */

#include <string.h>

#include "test.h"
#include "../../proto/uart_comm_crc.h"

static void test_crc16_standard_vector(void)
{
	const uint8_t data[] = "123456789";

	CHECK_EQ_U16(uart_comm_crc16(data, strlen((const char *)data)), 0x29B1,
		     "CRC-16/CCITT-FALSE standard vector");
}

static void test_crc16_empty(void)
{
	CHECK_EQ_U16(uart_comm_crc16(NULL, 0), 0xFFFF, "empty input keeps init value");
}

static void test_crc16_deterministic(void)
{
	const uint8_t data[] = { 0x01, 0x02, 0x03, 0xFF };

	CHECK_EQ_U16(uart_comm_crc16(data, sizeof(data)), uart_comm_crc16(data, sizeof(data)),
		     "same input yields same crc");
}

static void test_crc16_differs_by_content(void)
{
	const uint8_t a[] = { 0x01, 0x02 };
	const uint8_t b[] = { 0x01, 0x03 };

	CHECK_NE_U16(uart_comm_crc16(a, sizeof(a)), uart_comm_crc16(b, sizeof(b)),
		     "different content yields different crc");
}

static void test_crc16_differs_by_append(void)
{
	const uint8_t a[] = { 0x01 };
	const uint8_t b[] = { 0x01, 0x02 };

	CHECK_NE_U16(uart_comm_crc16(a, sizeof(a)), uart_comm_crc16(b, sizeof(b)),
		     "appending bytes changes crc");
}

void test_crc16_run(void)
{
	test_crc16_standard_vector();
	test_crc16_empty();
	test_crc16_deterministic();
	test_crc16_differs_by_content();
	test_crc16_differs_by_append();
}
