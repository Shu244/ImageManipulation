#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageManip.h"
#include "ppm_io.h"
#include <ctype.h>

int check_int(char *argument)
{
	int count = 1;
	int len = strlen(argument);
	for (int i = 0; i < len && count != 0; i++)
		if (!isdigit(*(argument + i)))
			count = 0;
	return count;
}

int main(int argc, char *argv[])
{

	if (argc < 3) //check if the user inputs filename for read and write
	{
		printf("Failed to supply input filename or output filename, or both\n");
		return 1;
	}
	Image *a = NULL; //initialize image type pointer for read_ppm
	FILE *fp_read = fopen(argv[1], "rb");
	if (fp_read == NULL)
	{
		printf("Specified input file could not be opened\n");
		return 2;
	}
	a = read_ppm(fp_read);
	if (a == NULL)
	{
		printf("Specified input file is not a properly-formatted PPM file, or reading input somehow fails\n");
		return 3;
	}
	int row = a->rows;
	int col = a->cols;
	fclose(fp_read);
	FILE *fp_write = fopen(argv[2], "wb");
	if (fp_write == NULL)
	{
		printf("Specified output file could not be opened for writing, or writing output somehow fails\n");
		return 7;
	}
	if (argc == 4) //check if the argument number is right for function swap, grayscale, zoom_in, and zoom_out.
	{
		if ((strcmp(argv[3], "zoom_in") == 0))
			zoom_in(a);
		else if ((strcmp(argv[3], "zoom_out") == 0))
		{
			if (row ==1 || col==1)
			{
				printf("PPM connot be zoomed out");
				return 8;
			}
			else
				zoom_out(a);
		}
		else if ((strcmp(argv[3], "swap") == 0))
			swap(a);
		else if ((strcmp(argv[3], "grayscale") == 0))
			grayscale(a);
		else
		{
			printf("Incorrect number of arguments or kind of arguments specified for the specified operation");
			return 5;
		}
	}
	else if (argc == 5) //check if the argument number is right for function blur and contrast
	{
		if ((strcmp(argv[3], "contrast") == 0))
		{
			double index = atof(argv[4]);
			if (index <= 0)
			{
				printf("Incorrect contrast value specified for the operation");
				return 5;
			}
			contrast(a, index);
		}
		else if ((strcmp(argv[3], "blur") == 0))
		{
			double sigma = atof(argv[4]);
			if (sigma <= 0)
			{
				printf("Incorrect sigma value specified for the operation");
				return 5;
			}
			blur(a, sigma);
		}
		else
		{
			printf("Incorrect number of arguments or kind of arguments specified for the specified operation");
			return 5;
		}
	}
	else if (argc == 8) //make sure we account for when r up and r down are the same( occluding one pixel)
	{
		if ((strcmp(argv[3], "occlude") == 0))
		{
			if (check_int(argv[4]) &&
				check_int(argv[5]) &&
				check_int(argv[6]) &&
				check_int(argv[7])) //check if the user inputs integer value of occlude area
			{
				int r_up = atoi(argv[4]);
				int c_up = atoi(argv[5]);
				int r_down = atoi(argv[6]);
				int c_down = atoi(argv[7]);
				if ((r_up <= r_down) && (c_up <= c_down) && (r_up >= 0) && (r_down >= 0) && (r_down < row) && (c_down < col))
				{
					occlude(a, r_up, c_up, r_down, c_down);
				}
				else
				{
					printf("Arguments for occlude operation were out of range for the given input image");
					return 6;
				}
			}
			else
			{
				printf("Arguments for occlude operation were not integer type");
				return 6;
			}
		}
		else
		{
			printf("Incorrect number of arguments or kind of arguments specified for the specified operation");
			return 5;
		}
	}
	else
	{
		printf("No operation name was specified, or operation name specified was invalid");
		return 4;
	}
	int num_pixels_written = write_ppm(fp_write, a);
	fclose(fp_write);
	printf("Image created with %d pixels.\n", num_pixels_written);
	free(a->data);
	free(a);
	return 0;
}
