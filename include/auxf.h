#pragma once

#include <math.h>
#include <stddef.h>

#ifdef CUSTOM_ISNAN_COMP
#define ISNAN(x) custom_isnan_comp((x))
#elif defined(CUSTOM_ISNAN_BIN)
#define ISNAN(x) custom_isnan_bin((x))
#else
#define ISNAN(x) isnan((x))
#endif

#ifdef CUSTOM_ISINF
#define ISINF(x) custom_isinf(x)
#else
#define ISINF(x) isinf(x)
#endif

bool custom_isnan_comp(double x);
bool custom_isnan_bin(double x);

bool custom_isinf(double x);
