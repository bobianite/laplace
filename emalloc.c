#include "emalloc.h"

void *emalloc(size_t bytes)
{
	void *p;

	if ((p = malloc(bytes)) == NULL) {
		fprintf(stderr, "%s\n", strerror(ENOMEM));
		exit(EXIT_FAILURE);
	}

	return p;
}
