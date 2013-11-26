#ifndef _LAPLACE_H_
#define _LAPLACE_H_

#include <float.h>
#include "emalloc.h"
#include "boundary.h"
#include "png_crap.h"
#include "constants.h"

struct point {
	float v; /* value of function at point */
	int fixed; /* 1 means fixed, any other value means not fixed,
		    * edge of lattice is fixed regardless of this value
		    */
};

void iteration();
void boundary_setup(int (*boundary)(int, float*));
void setup();
void output();

#endif /* !_LAPLACE_H_ */
