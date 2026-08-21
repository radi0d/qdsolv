/// @file

#include "comp.h"

#include <math.h>

/**
 * @brief Checks if two doubles are equal
 *
 * @param[in] a 1st double
 * @param[in] b 2nd double
 *
 * @return true, if a == b; false otherwise
 *
 * Checks if two doubles are equal with precision down to EPS
 */
bool
isequal(double a, double b)
{
	return fabs(a - b) < EPS;
}
