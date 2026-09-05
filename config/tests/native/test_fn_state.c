/*
 * Native unit tests for fn_state machine (FN layer switch debounce logic)
 *
 * Run with: gcc test_main.c test_fn_state.c \
 *           ../../boards/arm/hw75_dynamic/app/uart_comm/handler/fn_state.c
 */

#include "test.h"
#include "../../boards/arm/hw75_dynamic/app/uart_comm/handler/fn_state.h"

#define FN_LAYER 1

static void test_enter_records_current_layer(void)
{
	struct fn_state_machine sm;
	fn_state_init(&sm, FN_LAYER);

	fn_state_on_message(&sm, true);
	struct fn_state_action act = fn_state_apply(&sm, 0);

	CHECK_EQ_INT(act.type, FN_STATE_ENTER, "pressed enters FN layer");
	CHECK_EQ_U8(sm.current_layer, 0, "records layer before entering FN");
	CHECK_EQ_INT(sm.fn_active, 1, "fn marked active");
}

static void test_duplicate_pressed_is_ignored(void)
{
	struct fn_state_machine sm;
	fn_state_init(&sm, FN_LAYER);

	fn_state_on_message(&sm, true);
	fn_state_apply(&sm, 0);

	fn_state_on_message(&sm, true);
	struct fn_state_action act = fn_state_apply(&sm, FN_LAYER);

	CHECK_EQ_INT(act.type, FN_STATE_NONE, "duplicate pressed produces no action");
	CHECK_EQ_U8(sm.current_layer, 0, "duplicate pressed does not overwrite recorded layer");
}

static void test_pressed_on_fn_layer_keeps_recorded_layer(void)
{
	struct fn_state_machine sm;
	fn_state_init(&sm, FN_LAYER);

	fn_state_on_message(&sm, true);
	fn_state_apply(&sm, 0);
	fn_state_apply(&sm, FN_LAYER);

	/* Re-entering while already on FN layer (e.g. second message after
	 * user switched away and back) must not clobber recorded layer. */
	fn_state_on_message(&sm, true);
	struct fn_state_action act = fn_state_apply(&sm, FN_LAYER);

	CHECK_EQ_INT(act.type, FN_STATE_NONE, "already active FN produces no action");
	CHECK_EQ_U8(sm.current_layer, 0, "recorded layer kept while already on FN");
}

static void test_release_returns_to_recorded_layer(void)
{
	struct fn_state_machine sm;
	fn_state_init(&sm, FN_LAYER);

	fn_state_on_message(&sm, true);
	fn_state_apply(&sm, 0);

	fn_state_on_message(&sm, false);
	struct fn_state_action act = fn_state_apply(&sm, FN_LAYER);

	CHECK_EQ_INT(act.type, FN_STATE_LEAVE, "released leaves FN layer");
	CHECK_EQ_U8(act.layer, 0, "leaves to recorded layer");
	CHECK_EQ_INT(sm.fn_active, 0, "fn marked inactive");
}

static void test_release_without_press_is_ignored(void)
{
	struct fn_state_machine sm;
	fn_state_init(&sm, FN_LAYER);

	fn_state_on_message(&sm, false);
	struct fn_state_action act = fn_state_apply(&sm, 0);

	CHECK_EQ_INT(act.type, FN_STATE_NONE, "stray release produces no action");
}

static void test_debounce_reverts_before_apply(void)
{
	struct fn_state_machine sm;
	fn_state_init(&sm, FN_LAYER);

	/* Bounce: pressed then released before debounce window elapsed. */
	fn_state_on_message(&sm, true);
	fn_state_on_message(&sm, false);
	struct fn_state_action act = fn_state_apply(&sm, 0);

	CHECK_EQ_INT(act.type, FN_STATE_NONE, "bounce filtered before apply");
	CHECK_EQ_INT(sm.fn_active, 0, "fn never became active");
}

static void test_cycle_can_repeat(void)
{
	struct fn_state_machine sm;
	fn_state_init(&sm, FN_LAYER);

	fn_state_on_message(&sm, true);
	fn_state_apply(&sm, 2);
	fn_state_on_message(&sm, false);
	struct fn_state_action leave = fn_state_apply(&sm, FN_LAYER);

	CHECK_EQ_INT(leave.type, FN_STATE_LEAVE, "first cycle leaves");
	CHECK_EQ_U8(leave.layer, 2, "first cycle returns to layer 2");

	fn_state_on_message(&sm, true);
	struct fn_state_action enter = fn_state_apply(&sm, 2);

	CHECK_EQ_INT(enter.type, FN_STATE_ENTER, "second cycle can enter again");
	CHECK_EQ_U8(sm.current_layer, 2, "second cycle records current layer");
}

void test_fn_state_run(void)
{
	test_enter_records_current_layer();
	test_duplicate_pressed_is_ignored();
	test_pressed_on_fn_layer_keeps_recorded_layer();
	test_release_returns_to_recorded_layer();
	test_release_without_press_is_ignored();
	test_debounce_reverts_before_apply();
	test_cycle_can_repeat();
}
