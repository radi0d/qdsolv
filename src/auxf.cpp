#include "auxf.h"

#include <stdint.h>

// `-Wfloat-equal` violation is required
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"

bool
custom_isnan_comp(double x)
{
	// see IEEE 754
	return !(x == x);
}

#pragma GCC diagnostic pop

bool
custom_isnan_bin(double x)
{
	uint64_t t = *(uint64_t *) &x;
	// see IEEE 754
	return 0 != (t & 0xfffffffffffff) && 0x7ff == (t >> 52 & 0x7ff);
}

bool
custom_isinf(double x)
{
	uint64_t t = *(uint64_t *) &x;
	// see IEEE 754
	return 0 == (t & 0xfffffffffffff) && 0x7ff == (t >> 52 & 0x7ff);
}
