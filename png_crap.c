#include "png_crap.h"

int *read_png(char *filename, int *width, int *height)
{
	int i, j;
	int *data;
	FILE *file;
	png_struct *png;
	png_info *info;
	png_byte **row_pointers;

	if ((file = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Cannot open png file for reading.\n");
		exit(EXIT_FAILURE);
	}
	if ((png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
					NULL, NULL)) == NULL) {
		fprintf(stderr, "Failed to create png read struct.\n");
		exit(EXIT_FAILURE);
	}
	if ((info = png_create_info_struct(png)) == NULL) {
		fprintf(stderr, "Failed to create png info struct.\n");
		exit(EXIT_FAILURE);
	}
	if (setjmp(png_jmpbuf(png))) {
		fprintf(stderr, "Something bad happened with reading "
				"png image.\n");
		exit(EXIT_FAILURE);
	}
	png_init_io(png, file);
	png_read_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);
	*width = png_get_image_width(png, info);
	*height = png_get_image_height(png, info);
	row_pointers = png_get_rows(png, info);

	if ((data = malloc(*width * *height * sizeof(*data))) == NULL) {
		fprintf(stderr, "%s\n", strerror(ENOMEM));
		fflush(stderr);
		raise(SIGSEGV);
	}
	for (i = 0; i < *height; i++) {
		for (j = 0; j < *width; j++) {
			data[*width*i + j] = row_pointers[i][j];
		}
	}

	png_destroy_read_struct(&png, &info, NULL);
	fclose(file);
	return data;
}

int write_png(char *filename, int width, int height, int *data)
{
	int i, j;
	FILE *file;
	png_structp png;
	png_infop info;
	png_bytep row;

	if ((file = fopen(filename, "w")) == NULL) {
		fprintf(stderr, "Cannot open png file for writing.\n");
		exit(EXIT_FAILURE);
	}
	if ((png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
			NULL)) == NULL) {
		fprintf(stderr, "Failed to create png write struct.\n");
		exit(EXIT_FAILURE);
	}
	if ((info = png_create_info_struct(png)) == NULL) {
		fprintf(stderr, "Failed to create png info struct.\n");
		exit(EXIT_FAILURE);
	}
	if (setjmp(png_jmpbuf(png))) {
		fprintf(stderr, "Something bad happened with writing "
				"png image.\n");
		exit(EXIT_FAILURE);
	}
	png_init_io(png, file);
	png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_GRAY,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, info);

	if ((row = malloc(width * sizeof(*row))) == NULL) {
		fprintf(stderr, "%s\n", strerror(ENOMEM));
		fflush(stderr);
		raise(SIGSEGV);
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			row[j] = data[width*i + j];
		}
		png_write_row(png, row);
	}
	png_write_end(png, NULL);

	free(row);
	png_destroy_write_struct(&png, &info);
	fclose(file);
	return 0;
}
