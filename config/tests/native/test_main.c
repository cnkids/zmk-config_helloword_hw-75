/*
 * Native unit test entry point.
 */

#include "test.h"

int test_failures = 0;

void test_crc16_run(void);
void test_fn_state_run(void);

int main(void)
{
	test_crc16_run();
	test_fn_state_run();

	if (test_failures > 0) {
		printf("%d test(s) FAILED\n", test_failures);
		return 1;
	}

	printf("All tests passed\n");
	return 0;
}
