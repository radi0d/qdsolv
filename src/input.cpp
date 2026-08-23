/// @file

#include "auxf.h"
#include "input.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define HANDLE(x)                                                              \
	do {                                                                   \
		if ((x) < 0) {                                                 \
			fprintf(stderr, "[ERR] Invalid coefficient format\n"); \
			return -1;                                             \
		}                                                              \
	} while(0)

/**
 * @brief Gets a double from the stdin line
 *
 * @param[in] dst Destination
 *
 * @return Error code: 1, if successful; -1, if any errors were encoutered
 *
 * Gets a double from the stdin line. Expects only the double in the line (ignoring all whitespace characters).
 */
int
get_double(double *dst)
{
	assert(dst != NULL);

	double res = 0;
	int num = scanf("%lf", &res);
	if (num != 1 || ISNAN(res) || ISINF(res)) {
		return -1;
	}

	// sanitise the remaining chars
	int c = 0;
	while ((c = getchar()) != '\n' && c != EOF) {
		if (!isspace(c)) {
			return -1;
		}
	}

	*dst = res;
	return 1;
}

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

	printf("Enter coefficient *a*: ");
	HANDLE(get_double(&dst->a));

	printf("Enter coefficient *b*: ");
	HANDLE(get_double(&dst->b));

	printf("Enter coefficient *c*: ");
	HANDLE(get_double(&dst->c));

	return 1;
}
