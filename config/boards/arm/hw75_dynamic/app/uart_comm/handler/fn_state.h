/*
 * Copyright (c) 2022-2023 XiNGRZ
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

enum fn_state_action_type {
	FN_STATE_NONE,
	FN_STATE_ENTER,
	FN_STATE_LEAVE,
};

struct fn_state_action {
	enum fn_state_action_type type;
	uint8_t layer;
};

struct fn_state_machine {
	uint8_t fn_layer;
	uint8_t current_layer;
	bool fn_active;
	bool target;
	bool pending;
};

void fn_state_init(struct fn_state_machine *sm, uint8_t fn_layer);

void fn_state_on_message(struct fn_state_machine *sm, bool pressed);

struct fn_state_action fn_state_apply(struct fn_state_machine *sm, uint8_t active_layer);
