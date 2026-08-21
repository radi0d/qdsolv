#include "input.h"
#include "root.h"
#include "solve.h"
#include "input.h"

#include <assert.h>
#include <stdio.h>

int
main(void)
{
	printf("Quadratic equation solver\n"
	       "Copyright (c) radi0d 2026\n\n"
	       );
	double a = 0, b = 0, c = 0;

	handle_prompt_error(prompt_user("a", &a));
	handle_prompt_error(prompt_user("b", &b));
	handle_prompt_error(prompt_user("c", &c));

	struct quad_roots result = quad_solve(a, b, c);

	printf("\n");
	switch (result.num) {
	case NO_ROOTS:
		printf("No roots\n");
		break;
	case ONE_ROOT:
		printf("One root:\n"
		       "x = %lg\n", result.x1);
		break;
	case TWO_ROOTS:
		printf("Two roots:\n"
		       "x1 = %lg\n"
		       "x2 = %lg\n", result.x1, result.x2);
		break;
	case INF_ROOTS:
		printf("Any number\n");
		break;
	default:
		assert("Unreachable" && 0);
	}

	return 0;
}
