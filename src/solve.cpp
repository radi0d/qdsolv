/// @file

#include "auxf.h"
#include "comp.h"
#include "types.h"
#include "solve.h"

#include <assert.h>
#include <math.h>

static struct roots lin_solve(struct coeffs c);
static double get_discr(struct coeffs c);

/**
 * @brief Solves a quadratic equation
 *
 * @param[in] c Coefficients
 */
struct roots
quad_solve(struct coeffs c) {
	assert(!ISNAN(c.a));
	assert(!ISINF(c.a));

	assert(!ISNAN(c.b));
	assert(!ISINF(c.b));

	assert(!ISNAN(c.c));
	assert(!ISINF(c.c));

	// linear equation
	if (isequal(c.a, 0)) {
		struct coeffs l = (struct coeffs) {
			.a = c.b,
			.b = c.c,
			.c = 0,
		};
		struct roots result = lin_solve(l);
		assert(TWO_ROOTS != result.num);
		return result;
	}

	double discr = get_discr(c);
	if (discr < 0) {
		return (struct roots) {
			.x1 = 0,
			.x2 = 0,
			.num = NO_ROOTS,
		};
	}
	if (isequal(discr, 0)) {
		return (struct roots) {
			.x1 = -c.b / (2 * c.a),
			.x2 = 0,
			.num = ONE_ROOT,
		};
	}
	// discr > 0
	return (struct roots) {
		.x1 = (-c.b - sqrt(discr)) / (2 * c.a),
		.x2 = (-c.b + sqrt(discr)) / (2 * c.a),
		.num = TWO_ROOTS,
	};
}

// solves a linear equation (ax + b = 0)
static struct roots
lin_solve(struct coeffs c)
{
	assert(!ISNAN(c.a));
	assert(!ISINF(c.a));

	assert(!ISNAN(c.b));
	assert(!ISINF(c.b));

	if (isequal(c.a, 0)) {
		// equation 0 = 0
		if (isequal(c.b, 0)) {
			return (struct roots) {
				.x1 = 0,
				.x2 = 0,
				.num = INF_ROOTS,
			};
		}
		// equation b = 0
		return (struct roots) {
			.x1 = 0,
			.x2 = 0,
			.num = NO_ROOTS,
		};
	}

	// ax + b = 0
	return (struct roots) {
		.x1 = -c.b / c.a,
		.x2 = -c.b / c.a,
		.num = ONE_ROOT,
	};
}

// computes the dicriminant (D = b^2 - 4ac)
static double
get_discr(struct coeffs c)
{
	assert(!ISNAN(c.a));
	assert(!ISINF(c.a));

	assert(!ISNAN(c.b));
	assert(!ISINF(c.b));

	assert(!ISNAN(c.c));
	assert(!ISINF(c.c));

	return c.b * c.b - 4 * c.a * c.c;
}
