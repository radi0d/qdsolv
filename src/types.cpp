/// @file

#include "types.h"

#include <assert.h>
#include <stdio.h>

const char *
root_number_to_str(enum root_number n)
{
	switch (n) {
	case NO_ROOTS:
		return "0";
	case ONE_ROOT:
		return "1";
	case TWO_ROOTS:
		return "2";
	case INF_ROOTS:
		return "inf";
	default:
		assert("Unreachable" && 0);
	}
}

/**
 * @brief Prints the roots of an equation
 *
 * @param[in] r Roots
 */
void
print_roots(struct roots r)
{
	switch (r.num) {
	case NO_ROOTS:
		printf("No roots\n");
		break;
	case ONE_ROOT:
		printf("One root:\n"
		       "x = %lg\n", r.x1);
		break;
	case TWO_ROOTS:
		printf("Two roots:\n"
		       "x1 = %lg\n"
		       "x2 = %lg\n", r.x1, r.x2);
		break;
	case INF_ROOTS:
		printf("Any number\n");
		break;
	default:
		assert("Unreachable" && 0);
	}
}
