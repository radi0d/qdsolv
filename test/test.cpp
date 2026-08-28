#include "auxf.h"
#include "comp.h"
#include "types.h"
#include "solve.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

struct test_case {
	struct coeffs arg;
	struct roots ref;
};

#define GREEN "\033[0;32;49m"
#define YELLOW "\033[0;33;49m"
#define RED "\033[0;31;49m"
#define RESET "\033[0;39;49m"

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

int run_hard_coded();
int run_from_file(const char *file);

void print_usage();

int
main(int argc, const char *argv[])
{
	if (argc < 2) {
		goto fail;
	}

	if (0 == strcmp(argv[1], "hard") && 2 == argc) {
		if (run_hard_coded()) {
			return 1;
		}
	} else if (0 == strcmp(argv[1], "file") && 3 == argc) {
		if (run_from_file(argv[2])) {
			return 1;
		}
	} else {
		goto fail;
	}
	return 0;
 fail:
	print_usage();
	return 1;
}

int run_test(size_t i, struct test_case c);

int
run_hard_coded()
{
	struct test_case cases[] = {
		TEST(1, 2, 1, ONE_ROOT, -1, NAN),
		TEST(1, -12, 35, TWO_ROOTS, 5, 7),
		TEST(0, 0, 0, INF_ROOTS, NAN, NAN),
		TEST(0, 0, 1, NO_ROOTS, NAN, NAN),
		TEST(0, 2, 1, ONE_ROOT, -0.5, NAN),
	};

	printf(YELLOW "Running hard coded tests:\n" RESET );
	for (size_t i = 0; i < sizeof(cases) / sizeof(struct test_case); i++) {
		if (run_test(i, cases[i])) {
			return -1;
		}
	}
	return 0;
}

int read_test_case(FILE *f, struct test_case *c);

int
run_from_file(const char *file)
{
	FILE *test_file = fopen(file, "r");
	if (NULL == test_file) {
		perror("Testing failed");
		return -1;
	}

	printf(YELLOW "Running tests from the file:\n" RESET);
	size_t current = 0;
	while (true) {
		struct test_case c = {};

		int res = read_test_case(test_file, &c);
		// EOF
		if (1 == res) {
			break;
		}
		// format error
		if (-1 == res) {
			printf(RED "[ERR] " RESET "Invalid test format\n");
			fclose(test_file);
			return -1;
		}

		if (run_test(current++, c)) {
			fclose(test_file);
			return -1;
		}
	}
	fclose(test_file);
	return 0;
}

void
print_usage()
{
	printf("Usage: test hard\n"
	       "       test file <file>\n"
	       );
}

static bool roots_isequal(roots a, roots b);

int
run_test(size_t i, struct test_case c)
{
	printf("| Running test#%zu... ", i);
	struct roots r = quad_solve(c.arg);
	if (!roots_isequal(r, c.ref)) {
		printf(RED "ERR\n" RESET);
		printf(YELLOW "Test#%zu failed:\n" RESET
		       "| " GREEN "Expected" RESET ": %s root(s); x1 = %lg; x2 = %lg\n"
		       "| " RED "Got" RESET ": %s root(s); x1 = %lg; x2 = %lg\n",
		       i,
		       root_number_to_str(c.ref.num), c.ref.x1, c.ref.x2,
		       root_number_to_str(r.num), r.x1, r.x2
		       );
		return -1;
	}
	printf(GREEN "OK\n" RESET);
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

int
read_test_case(FILE *f, struct test_case *c)
{
	assert(NULL != f);
	assert(NULL != c);

	struct test_case res = {};
	int num_roots = 0;
	int fields_read = fscanf(f, "%lf %lf %lf %d %lf %lf",
	                         &res.arg.a, &res.arg.b, &res.arg.c,
	                         &num_roots,
	                         &res.ref.x1, &res.ref.x2);
	if (EOF == fields_read) {
		return 1;
	}
	if (6 != fields_read) {
		return -1;
	}

	// sanitise the remaining chars
	int ch = 0;
	while ('\n' != (ch = fgetc(f)) && EOF != ch) {
		if (!isspace(ch)) {
			return -1;
		}
	}

	switch(num_roots) {
	case 0:
		res.ref.num = NO_ROOTS;
		break;
	case 1:
		res.ref.num = ONE_ROOT;
		break;
	case 2:
		res.ref.num = TWO_ROOTS;
		break;
	case 3:
		res.ref.num = INF_ROOTS;
		break;
	default:
		return -1;
	}

	*c = res;
	return 0;
}
