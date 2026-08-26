/// @file

#include "auxf.h"
#include "input.h"
#include "parser.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HANDLE(x)                                                              \
	do {                                                                   \
		if ((x)) {                                                     \
			fprintf(stderr, "[ERR] Invalid coefficient format\n"); \
			return -1;                                             \
		}                                                              \
	} while(0)

/**
 * @brief Gets a double from the stdin line
 *
 * @param[in] dst Destination
 *
 * @return Error code: 0, if successful; -1, if any errors were encoutered
 *
 * Gets a double from the stdin line. Expects only the double in the line (ignoring all whitespace characters).
 */
int
get_double(double *dst)
{
	assert(NULL != dst);

	double res = 0;
	int num = scanf("%lf", &res);
	if (1 != num || ISNAN(res) || ISINF(res)) {
		return -1;
	}

	// sanitise the remaining chars
	int c = 0;
	while ('\n' != (c = getchar()) && EOF != c) {
		if (!isspace(c)) {
			return -1;
		}
	}

	*dst = res;
	return 0;
}

static int read_equation(struct coeffs *dst);
static int read_coeffs(struct coeffs *dst);

/**
 * @brief Asks user to enter a double
 *
 * @param[in] arg Name of the double
 * @param[out] dst Destination of the double
 *
 * @return The result of get_double() called internally
 */
int
prompt_user(struct coeffs *dst)
{
	assert(dst != NULL);

	int ans = 0;
	while (true) {
		ans = 0;
		printf("Equation or coefficients? (1,2): ");
		scanf("%d", &ans);

		// sanitise the remaining chars
		int c = 0;
		while ('\n' != (c = getchar()) && EOF != c) {
			if (!isspace(c)) {
				fprintf(stderr,
				        "[ERR] Invalid option format\n");
				return -1;
			}
		}

		if (1 == ans || 2 == ans) {
			break;
		}
	}

	switch (ans) {
	case 1: // equation
		return read_equation(dst);
	case 2: // coefficients
		return read_coeffs(dst);
	default:
		assert("Unreachable" && 0);
	}
}

static int
read_equation(struct coeffs *dst)
{
	assert(NULL != dst);

	printf("Enter the equation (ax^2 + bx + c = 0): ");

	char *line = NULL;
	size_t cap = 0;
	if (getline(&line, &cap, stdin) < 0) {
		perror("getline() failed");
		free(line);
		return -1;
	}
	if (line == NULL) {
		return -1;
	}
	(void) cap;

	if (parse_equation(line, strlen(line), dst)) {
		fprintf(stderr, "[ERR] Invalid equation format\n");
		free(line);
		return -1;
	}

	free(line);
	return 0;
}

static int
read_coeffs(struct coeffs *dst)
{
	assert(NULL != dst);

	printf("Enter coefficient *a*: ");
	HANDLE(get_double(&dst->a));

	printf("Enter coefficient *b*: ");
	HANDLE(get_double(&dst->b));

	printf("Enter coefficient *c*: ");
	HANDLE(get_double(&dst->c));

	return 0;
}
