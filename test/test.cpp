#include "auxf.h"
#include "comp.h"
#include "root.h"
#include "solve.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#define TEST_INIT size_t test_current = 1

static bool roots_isequal(roots a, roots b);
static int run_test(size_t i, double a, double b, double c, double x1_ref,
                    double x2_ref, enum root_number num_ref);

#define TEST(a, b, c, x1_ref, x2_ref, num_ref)                  \
	do {                                                    \
		printf("Running test#%zu... ", test_current);   \
		if (!run_test(test_current, a, b, c,            \
		              x1_ref, x2_ref, num_ref)) {       \
			return 1;                               \
		}                                               \
		printf("OK\n");                                 \
		test_current++;                                 \
	} while(0)


int
main(void)
{
	TEST_INIT;
	TEST(1, 2, 1, -1, NAN, ONE_ROOT);
	TEST(1, -12, 35, 5, 7, TWO_ROOTS);
	TEST(0, 0, 0, NAN, NAN, INF_ROOTS);
	TEST(0, 0, 1, NAN, NAN, NO_ROOTS);
	TEST(0, 2, 1, -0.5, NAN, ONE_ROOT);
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

static int
run_test(size_t i, double a, double b, double c, double x1_ref,
         double x2_ref, enum root_number num_ref)
{
	roots t = {
		.x1 = x1_ref,
		.x2 = x2_ref,
		.num = num_ref,
	};
	roots r = quad_solve(a, b, c);
	if (!roots_isequal(t, r)) {
		printf("\nTest#%zu failed:\n"
		       "Expected: %s root(s); x1 = %lg; x2 = %lg\n"
		       "Got: %s root(s); x1 = %lg; x2 = %lg\n",
		       i,
		       root_number_to_str(num_ref), x1_ref, x2_ref,
		       root_number_to_str(r.num), r.x1, r.x2
		       );
		return 0;
	}
	return 1;
}
