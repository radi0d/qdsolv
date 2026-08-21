/// @file

#include "input.h"

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	char *line = NULL;
	size_t cap = 0;
	ssize_t read = getline(&line, &cap, stdin);
	if (read < 0) {
		return -1;
	}
	if (NULL == line) {
		return -1;
	}

	double t = 0;
	char mark = ' ';
	ssize_t num = sscanf(line, "%lf %c", &t, &mark);

	if (num != 1 || isnan(t) || isinf(t) || !isspace(mark)) {
		free(line);
		return -1;
	}

	*dst = t;
	free(line);
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
prompt_user(const char *arg, double *dst)
{
	assert(arg != NULL);
	assert(dst != NULL);

	printf("Enter coefficient *%s*: ", arg);
	return get_double(dst);
}
