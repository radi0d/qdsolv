#include "input.h"
#include "types.h"
#include "solve.h"

#include <stdio.h>

int
main(void)
{
	printf("Quadratic equation solver\n"
	       "Copyright (c) radi0d 2026\n\n"
	       );

	struct coeffs input = {};

	if (prompt_user(&input) < 0) {
		return 1;
	}

	struct roots result = quad_solve(input);

	printf("\n");
	print_roots(result);
	return 0;
}
