#include "input.h"
#include "root.h"
#include "solve.h"

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

	struct roots result = quad_solve(a, b, c);

	printf("\n");
	print_roots(result);
	return 0;
}
