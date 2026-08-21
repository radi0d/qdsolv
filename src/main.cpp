#include "input.h"
#include "root.h"
#include "solve.h"

#include <stdio.h>

#define HANDLE(x)							\
	do {								\
		if ((x) < 0) {						\
			fprintf(stderr, "[ERR] Invalid coefficient format\n"); \
			return 1;					\
		}							\
	} while(0)							\

int
main(void)
{
	printf("Quadratic equation solver\n"
	       "Copyright (c) radi0d 2026\n\n"
	       );

	double a = 0, b = 0, c = 0;

	HANDLE(prompt_user("a", &a));
	HANDLE(prompt_user("b", &b));
	HANDLE(prompt_user("c", &c));

	struct roots result = quad_solve(a, b, c);

	printf("\n");
	print_roots(result);
	return 0;
}
