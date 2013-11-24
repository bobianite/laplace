#ifndef _BOUNDARY_H_
#define _BOUNDARY_H_

#include <math.h>
#include "constants.h"

int distance(int loc1, int loc2)
{
	int x1, y1, x2, y2;

	x1 = loc1 % WIDTH;
	y1 = loc1 / WIDTH;
	x2 = loc2 % WIDTH;
	y2 = loc2 / WIDTH;

	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

int b1(int location, float *v)
{
	int center;
	int radius;

	radius = (float)HEIGHT/16;
	center = (float)1*HEIGHT/2*WIDTH + (float)WIDTH/2;

	if (distance(location, center) == radius) {
		*v = 5;
		return 1;
	}

	return 0;
}

int b2(int location, float *v)
{
	int center;
	int radius;

	radius = (float)HEIGHT/8;
	center = (float)1*HEIGHT/4*WIDTH + (float)WIDTH/2;

	if (distance(location, center) == radius) {
		*v = -5;
		return 1;
	}

	return 0;
}

#endif /* !_BOUNDARY_H_ */
