/// @file

#include "root.h"

#include <assert.h>
#include <stdio.h>

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
