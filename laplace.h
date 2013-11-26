#ifndef _LAPLACE_H_
#define _LAPLACE_H_

#include <float.h>
#include "emalloc.h"
#include "boundary.h"
#include "png_crap.h"
#include "constants.h"

void iteration();
void boundary_setup(int (*boundary)(int, float*));
void setup();
void output();

#endif /* !_LAPLACE_H_ */
