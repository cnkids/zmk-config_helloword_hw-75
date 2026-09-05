#!/bin/sh
# Build and run native unit tests for the hw75 UART hardening logic.
set -eu

cd "$(dirname "$0")"

CC="${CC:-cc}"
CFLAGS="-std=c99 -Wall -Wextra -Werror"

$CC $CFLAGS -o test_bin \
    test_main.c \
    test_crc16.c \
    test_fn_state.c \
    ../../proto/uart_comm_crc.c \
    ../../boards/arm/hw75_dynamic/app/uart_comm/handler/fn_state.c

./test_bin
