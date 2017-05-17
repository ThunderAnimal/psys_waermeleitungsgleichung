/**
 * Test for functions in core.c
 *
 * We don't need to test anything here really.
 */
#include <assert.h>

/* Include the source here */
#include "core.c"

/******************************************************/

/* This test the hello_world() function */
void test__hello_world(void)
{
	hello_world();
}

/**
 * Main entry for the test.
 */
int main(int argc, char **argv)
{
	test__hello_world();
	return 0;
}
