//Name: Shuhao Lai & Steven Tan
//ppm_io.c
// 601.220, Fall 2018

#include <assert.h>
#include <stdlib.h>
#include "ppm_io.h"

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image *read_ppm(FILE *fp)
{
	int c, r, color;
	char format1, format2, letter, new_line;
	fscanf(fp, "%c%c", &format1, &format2);
	if (format1 != 'P' || format2 != '6')
		return NULL;
	//Skips over comment.
	if (fscanf(fp, " #%c", &letter))
		while (fscanf(fp, "%c", &letter))
			if (letter == '\n')
				break;
	//not properly formatted
	if (!fscanf(fp, "%d", &c))
		return NULL;
	if (!fscanf(fp, "%d", &r))
		return NULL;
	if (!fscanf(fp, "%d", &color))
		return NULL;
	if (color != 255)
		return NULL;
	if (!fscanf(fp, "%c", &new_line))
		return NULL;

	Pixel *data = (Pixel *)malloc(sizeof(Pixel) * (r * c));
	Image *im = (Image *)malloc(sizeof(data) + sizeof(int) * 2); //stores pointer and 2 ints.
	im->rows = r;
	im->cols = c;
	for (int i = 0; i < r * c; i++)
	{
		unsigned char pixel[3];
		fread(pixel, 3, sizeof(char), fp);
		(data + i)->r = pixel[0];
		(data + i)->g = pixel[1];
		(data + i)->b = pixel[2];
	}
	im->data = data;
	return im;
}

/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im)
{

	// check that fp is not NULL
	assert(fp);

	// write PPM file header, in the following format
	// P6
	// cols rows
	// 255
	fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

	// now write the pixel array
	int num_pixels_written = fwrite(im->data, sizeof(Pixel),
									im->rows * im->cols, fp);

	if (num_pixels_written != im->rows * im->cols)
	{
		fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
	}

	return num_pixels_written;
}
