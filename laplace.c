#include "laplace.h"

struct point *lattice;
struct point *lattice2;

void average_neighbors(int location)
{
	struct point *p, *top, *bottom, *left, *right;

	p = lattice2 + location;
	if ((lattice + location)->fixed == 1) {
		p->v = (lattice + location)->v;
		p->fixed = 1;
		return;
	}

	top = lattice + location - WIDTH;
	bottom = lattice + location + WIDTH;
	left = lattice + location - 1;
	right = lattice + location + 1;

	p->v = (top->v + bottom->v + left->v + right->v)/4;
	p->fixed = 0;
}

void iteration()
{
	int i, j;
	struct point *temp;

	for (i = 1; i < HEIGHT-1; i++) {
		for (j = 1; j < WIDTH-1; j++) {
			average_neighbors(i*WIDTH + j);
		}
	}

	temp = lattice;
	lattice = lattice2;
	lattice2 = temp;
}

void boundary_setup(int (*boundary)(int, float*))
{
	int i, j, location;
	float v;
	struct point *p;

	for (i = 1; i < HEIGHT-1; i++) {
		for (j = 1; j < WIDTH-1; j++) {
			location = i*HEIGHT + j;
			if ((*boundary)(location, &v) == 1) {
				p = lattice + location;
				p->v = v;
				p->fixed = 1;
			}
		}
	}
}

void setup()
{
	int i, j, location;
	struct point *p;

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			location = i*HEIGHT + j;
			p = lattice + location;
			p->v = 0;
			p->fixed = 0;
		}
	}
}

void output()
{
	int i, j;
	int location;
	int *data;
	float min, max;
	struct point *p;

	min = FLT_MAX;
	max = FLT_MIN;
	data = emalloc(HEIGHT * WIDTH * sizeof(*data));

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			location = i*WIDTH + j;
			p = lattice + location;
			if (max < p->v)
				max = p->v;
			if (min > p->v)
				min = p->v;
		}
	}

	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			location = i*WIDTH + j;
			p = lattice + location;
			data[location] = (float)255 * (p->v - min)
				/(max - min) + 0.5;
		}
	}

	write_png("output.png", WIDTH, HEIGHT, data);

	free(data);
}

int main(int argc, char *argv[])
{
	int i;

	lattice = emalloc(WIDTH * HEIGHT * sizeof(*lattice));
	lattice2 = emalloc(WIDTH * HEIGHT * sizeof(*lattice2));
	setup();
	
	boundary_setup(&b1);
	boundary_setup(&b2);

	for (i = 0; i < MAX_ITER; i++)
		iteration();

	output();

	free(lattice);
	free(lattice2);
	return 0;
}
