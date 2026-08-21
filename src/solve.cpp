/// @file

#include "comp.h"
#include "root.h"
#include "solve.h"

#include <assert.h>
#include <math.h>

static struct roots lin_solve(double a, double b);
static double get_discr(double a, double b, double c);

/**
 * @brief Solves a quadratic equation (ax^2 + bx + c = 0)
 *
 * @param[in] a Coefficient a
 * @param[in] b Coefficient b
 * @param[in] c Coefficient c
 */
struct roots
quad_solve(double a, double b, double c) {
	assert(!isnan(a));
	assert(!isinf(a));

	assert(!isnan(b));
	assert(!isinf(b));

	assert(!isnan(c));
	assert(!isinf(c));

	// linear equation
	if (isequal(a, 0)) {
		struct roots result = lin_solve(b, c);
		assert(result.num != TWO_ROOTS);
		return (struct roots) {
			.x1 = result.x1,
			.x2 = 0,
			.num = result.num,
		};
	}

	double discr = get_discr(a, b, c);
	if (discr < 0) {
		return (struct roots) {
			.x1 = 0,
			.x2 = 0,
			.num = NO_ROOTS,
		};
	}
	if (isequal(discr, 0)) {
		return (struct roots) {
			.x1 = -b / (2 * a),
			.x2 = 0,
			.num = ONE_ROOT,
		};
	}
	// discr > 0
	return (struct roots) {
		.x1 = (-b - sqrt(discr)) / (2 * a),
		.x2 = (-b + sqrt(discr)) / (2 * a),
		.num = TWO_ROOTS,
	};
}

// solves a linear equation (ax + b = 0)
static struct roots
lin_solve(double a, double b)
{
	assert(!isnan(a));
	assert(!isinf(a));

	assert(!isnan(b));
	assert(!isinf(b));

	if (isequal(a, 0)) {
		// equation 0 = 0
		if (isequal(b, 0)) {
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
		.x1 = -b / a,
		.x2 = -b / a,
		.num = ONE_ROOT,
	};
}

// computes the dicriminant (D = b^2 - 4ac)
static double
get_discr(double a, double b, double c)
{
	assert(!isnan(a));
	assert(!isinf(a));

	assert(!isnan(b));
	assert(!isinf(b));

	assert(!isnan(c));
	assert(!isinf(c));

	return b * b - 4 * a * c;
}
