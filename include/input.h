#pragma once

#include "types.h"

int get_double(double *dst);
int prompt_user(struct coeffs *dst);
void handle_prompt_error(int res);
