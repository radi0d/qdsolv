#pragma once

/**
 * @brief Number of the roots of an equation
 */
enum root_number {
	NO_ROOTS, ///< No roots
	ONE_ROOT, ///< One root
	TWO_ROOTS, ///< Two roots
	INF_ROOTS, ///< Any number
};

const char *root_number_to_str(enum root_number n);

/**
 * @brief Roots of an equation
 *
 * The result of quad_solve()
 */
struct roots {
	double x1; // the 1st root
	double x2; // the 2nd root
	enum root_number num; // the number of the roots
};

void print_roots(struct roots r);

/**
 * @brief Coefficients of an equation
 *
 * The argument of quad_solve()
 */
struct coeffs {
	double a; ///< the 1st coefficient
	double b; ///< the 2nd coefficient
	double c; ///< the 3rd coefficient
};
