/*
 * Copyright (c) 2022-2023 XiNGRZ
 * SPDX-License-Identifier: MIT
 */

#include "handler.h"

#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include <zmk/keymap.h>

#include "fn_state.h"

#define FN_LAYER 1

static struct fn_state_machine fn_sm;

static struct k_work_delayable fn_state_work;

static void fn_state_work_handler(struct k_work *work)
{
	ARG_UNUSED(work);

	struct fn_state_action action = fn_state_apply(&fn_sm, zmk_keymap_highest_layer_active());

	switch (action.type) {
	case FN_STATE_ENTER:
		zmk_keymap_layer_to(fn_sm.fn_layer);
		break;
	case FN_STATE_LEAVE:
		zmk_keymap_layer_to(action.layer);
		break;
	default:
		break;
	}
}

bool handle_fn_state(const uart_comm_MessageK2D *k2d)
{
	const uart_comm_FnState *report = &k2d->payload.fn_state;

	if (report->pressed == fn_sm.fn_active) {
		return true;
	}

	fn_state_on_message(&fn_sm, report->pressed);
	k_work_reschedule(&fn_state_work, K_MSEC(CONFIG_HW75_UART_COMM_FN_DEBOUNCE_MS));

	return true;
}

static int handler_fn_state_init(const struct device *dev)
{
	ARG_UNUSED(dev);

	fn_state_init(&fn_sm, FN_LAYER);
	k_work_init_delayable(&fn_state_work, fn_state_work_handler);

	return 0;
}

SYS_INIT(handler_fn_state_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
