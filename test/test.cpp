#include "auxf.h"
#include "comp.h"
#include "types.h"
#include "solve.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

struct test_case {
	struct coeffs arg;
	struct roots ref;
};

#define TEST(a_, b_, c_, num_ref, x1_ref, x2_ref)       \
	(struct test_case) {                            \
		.arg = (struct coeffs) {                \
			.a = a_,                        \
			.b = b_,                        \
			.c = c_,                        \
		},                                      \
		.ref = (struct roots) {                 \
			.x1 = x1_ref,                   \
			.x2 = x2_ref,                   \
			.num = num_ref,                 \
		},                                      \
	}

void run_test(size_t i, struct test_case c);

static bool roots_isequal(roots a, roots b);

int
main(void)
{
	struct test_case cases[] = {
		TEST(1, 2, 1, ONE_ROOT, -1, NAN),
		TEST(1, -12, 35, TWO_ROOTS, 5, 7),
		TEST(0, 0, 0, INF_ROOTS, NAN, NAN),
		TEST(0, 0, 1, NO_ROOTS, NAN, NAN),
		TEST(0, 2, 1, ONE_ROOT, -0.5, NAN),
	};

	for (size_t i = 0; i < sizeof(cases) / sizeof(struct test_case); i++) {
		run_test(i, cases[i]);
	}
	return 0;
}

static bool
roots_isequal(roots a, roots b)
{
	if (a.num != b.num) {
		return false;
	}

	switch (a.num) {
	case NO_ROOTS:
		return true;
	case ONE_ROOT:
		return isequal(a.x1, b.x1) || (ISNAN(a.x1) && ISNAN(b.x1));
	case TWO_ROOTS:
		return (isequal(a.x1, b.x1) && isequal(a.x2, b.x2)) ||
			((ISNAN(a.x1) && ISNAN(b.x1)) ||
			 (ISNAN(a.x2) && ISNAN(b.x2)));
	case INF_ROOTS:
		return true;
	default:
		assert("Unreachable" && 0);
	}
}

void
run_test(size_t i, struct test_case c)
{
	printf("Running test#%zu... ", i);
	struct roots r = quad_solve(c.arg);
	if (!roots_isequal(r, c.ref)) {
		printf("ERR\n");
		printf("Test#%zu failed:\n"
		       "Expected: %s root(s); x1 = %lg; x2 = %lg\n"
		       "Got: %s root(s); x1 = %lg; x2 = %lg\n",
		       i,
		       root_number_to_str(c.ref.num), c.ref.x1, c.ref.x2,
		       root_number_to_str(r.num), r.x1, r.x2
		       );
		exit(1);
	}
	printf("OK\n");
}
