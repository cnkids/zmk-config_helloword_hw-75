/*
 * Shared assertions for native unit tests.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

extern int test_failures;

#define CHECK_FAIL(cond_desc)                                                                      \
	do {                                                                                       \
		printf("FAIL: %s (%s:%d)\n", cond_desc, __FILE__, __LINE__);                       \
		test_failures++;                                                                   \
	} while (0)

#define CHECK_EQ_U16(actual, expected, desc)                                                       \
	do {                                                                                       \
		unsigned int _a = (actual);                                                        \
		unsigned int _e = (expected);                                                      \
		if (_a != _e) {                                                                    \
			printf("FAIL: %s: got 0x%04X, want 0x%04X (%s:%d)\n", desc, _a, _e,       \
			       __FILE__, __LINE__);                                                \
			test_failures++;                                                           \
		} else {                                                                           \
			printf("PASS: %s\n", desc);                                                \
		}                                                                                  \
	} while (0)

#define CHECK_NE_U16(actual, other, desc)                                                          \
	do {                                                                                       \
		unsigned int _a = (actual);                                                        \
		unsigned int _o = (other);                                                         \
		if (_a == _o) {                                                                    \
			printf("FAIL: %s: both 0x%04X (%s:%d)\n", desc, _a, __FILE__, __LINE__);   \
			test_failures++;                                                           \
		} else {                                                                           \
			printf("PASS: %s\n", desc);                                                \
		}                                                                                  \
	} while (0)

#define CHECK_EQ_U8(actual, expected, desc)                                                        \
	CHECK_EQ_U16((actual), (expected), desc)

#define CHECK_EQ_INT(actual, expected, desc)                                                       \
	CHECK_EQ_U16((actual), (expected), desc)
