#include "laplace.h"

float *lattice_v;
float *lattice2_v;
int *lattice_fixed;
int *lattice2_fixed;

/* value at a point in new lattice is average of surrounding values */
void average_neighbors(int location)
{
	float top, bottom, left, right;

	top = lattice_v[location - WIDTH];
	bottom = lattice_v[location + WIDTH];
	left = lattice_v[location - 1];
	right = lattice_v[location + 1];

	lattice2_v[location] = (top + bottom + left + right)/4;
	lattice2_fixed[location] = 0;
}

/* each iteration creates a new lattice using averaging */
void iteration()
{
	int i, j, location;
	float *v_temp;
	int *fixed_temp;

	for (i = 1; i < HEIGHT-1; i++) {
		for (j = 1; j < WIDTH-1; j++) {
			location = i*WIDTH + j;
			average_neighbors(location);
		}
	}

	for (i = 1; i < HEIGHT-1; i++) {
		for (j = 1; j < WIDTH-1; j++) {
			location = i*WIDTH + j;
			if (lattice_fixed[location] == 1) {
				lattice2_v[location] =
					lattice_v[location];
				lattice2_fixed[location] = 1;
			}
		}
	}

	v_temp = lattice_v;
	lattice_v = lattice2_v;
	lattice2_v = v_temp;

	fixed_temp = lattice_fixed;
	lattice_fixed = lattice2_fixed;
	lattice2_fixed = fixed_temp;
}

/* set up the boundary conditions */
void boundary_setup(int (*boundary)(int, float*))
{
	int i, j, location;
	float v;

	for (i = 1; i < HEIGHT-1; i++) {
		for (j = 1; j < WIDTH-1; j++) {
			location = i*HEIGHT + j;
			if ((*boundary)(location, &v) == 1) {
				lattice_v[location] = v;
				lattice_fixed[location] = 1;
			}
		}
	}
}

/* initialize lattice with 0 everywhere */
void setup()
{
	int i, j, location;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			location = i*HEIGHT + j;
			lattice_v[location] = 0;
			lattice_fixed[location] = 0;
		}
	}
}

/* output solution to png image */
void output()
{
	int i, j;
	int location;
	int *data;
	float min, max;

	min = FLT_MAX;
	max = FLT_MIN;
	data = emalloc(HEIGHT * WIDTH * sizeof(*data));

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			location = i*WIDTH + j;
			if (max < lattice_v[location])
				max = lattice_v[location];
			if (min > lattice_v[location])
				min = lattice_v[location];
		}
	}

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			location = i*WIDTH + j;
			data[location] = (float)255 *
				(lattice_v[location] - min)
				/(max - min) + 0.5;
		}
	}

	write_png("output.png", WIDTH, HEIGHT, data);

	free(data);
}

/* the yos */
int main(int argc, char *argv[])
{
	int i;

	lattice_v = emalloc(WIDTH * HEIGHT * sizeof(*lattice_v));
	lattice2_v = emalloc(WIDTH * HEIGHT * sizeof(*lattice2_v));
	lattice_fixed = emalloc(WIDTH * HEIGHT *
			sizeof(*lattice_fixed));
	lattice2_fixed = emalloc(WIDTH * HEIGHT *
			sizeof(*lattice2_fixed));
	setup();
	
	boundary_setup(&b1);
	boundary_setup(&b2);

	for (i = 0; i < MAX_ITER; i++)
		iteration();

	output();

	free(lattice_v);
	free(lattice2_v);
	free(lattice_fixed);
	free(lattice2_fixed);

	return 0;
}
