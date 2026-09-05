/*
 * Copyright (c) 2022-2023 XiNGRZ
 * SPDX-License-Identifier: MIT
 */

#include <stdbool.h>
#include <stdint.h>

#include "fn_state.h"

void fn_state_init(struct fn_state_machine *sm, uint8_t fn_layer)
{
	sm->fn_layer = fn_layer;
	sm->current_layer = 0;
	sm->fn_active = false;
	sm->target = false;
	sm->pending = false;
}

void fn_state_on_message(struct fn_state_machine *sm, bool pressed)
{
	sm->target = pressed;
	sm->pending = (pressed != sm->fn_active);
}

struct fn_state_action fn_state_apply(struct fn_state_machine *sm, uint8_t active_layer)
{
	struct fn_state_action action = { .type = FN_STATE_NONE, .layer = sm->fn_layer };

	if (!sm->pending) {
		return action;
	}

	if (sm->target) {
		if (active_layer != sm->fn_layer) {
			sm->current_layer = active_layer;
		}
		action.type = FN_STATE_ENTER;
	} else {
		action.type = FN_STATE_LEAVE;
		action.layer = sm->current_layer;
	}

	sm->fn_active = sm->target;
	sm->pending = false;

	return action;
}
