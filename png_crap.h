#ifndef _PNG_CRAP_H_
#define _PNG_CRAP_H_

#include <png.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

int *read_png(char *filename, int *width, int *height);
int write_png(char *filename, int width, int height, int *data);

#endif /* !_PNG_CRAP_H_ */
