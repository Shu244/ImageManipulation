/*
 *imageManip.c
 *Created on: Oct 13, 2018
 *Author: Shuhao Lai and Steven Tan
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#define PI 3.14159265358979323846
#include "imageManip.h"

/*
 * Swaps the rgb values in image such that r = g, g = b, and b = r.
 */
void swap(Image *image)
{
	int cols = image->cols, rows = image->rows;
	for (int r = 0; r < rows; r++)
		for (int c = 0; c < cols; c++)
		{
			int tempr = (image->data + (r)*cols + c)->r, tempg = (image->data + (r)*cols + c)->g, tempb = (image->data + (r)*cols + c)->b;
			(image->data + (r)*cols + c)->r = tempg;
			(image->data + (r)*cols + c)->g = tempb;
			(image->data + (r)*cols + c)->b = tempr;
		}
}

/*
 * Grayscales the image by setting the rgb values for a pixel to 0.3*r + 0.59*g + 0.11*b.
 */
void grayscale(Image *image)
{
	int cols = image->cols, rows = image->rows;
	for (int r = 0; r < rows; r++)
		for (int c = 0; c < cols; c++)
		{
			char intensity = (char)((image->data + (r)*cols + c)->r * 0.30 + (image->data + (r)*cols + c)->g * 0.59 + (image->data + (r)*cols + c)->b * 0.11);
			(image->data + (r)*cols + c)->r = intensity;
			(image->data + (r)*cols + c)->g = intensity;
			(image->data + (r)*cols + c)->b = intensity;
		}
}

/*
 * Contrasts the image by making the rgb values evenly span zero, multiplying the span by factor, and converting it back
 * to rgb value using saturated math.
 */
void contrast(Image *image, double factor)
{
	int cols = image->cols, rows = image->rows;
	for (int r = 0; r < rows; r++)
		for (int c = 0; c < cols; c++)
		{
			//Making the rgb values evenly span zero.
			double split_r = (double)(image->data + (r)*cols + c)->r * 2 - 255;
			double split_g = (double)(image->data + (r)*cols + c)->g * 2 - 255;
			double split_b = (double)(image->data + (r)*cols + c)->b * 2 - 255;

			//Multiply each adjusted rgb value by factor.
			split_r *= factor;
			split_g *= factor;
			split_b *= factor;

			//reverts adjusted rgb values back to proper rgb values.
			double revert_r = (split_r+255)/2, revert_g = (split_g+255)/2, revert_b = (split_b+255)/2;
			if (revert_r > 255)
				revert_r = 255;
			else if (revert_r < 0)
				revert_r = 0;
			if (revert_g > 255)
				revert_g = 255;
			else if (revert_g < 0)
				revert_g = 0;
			if (revert_b > 255)
				revert_b = 255;
			else if (revert_b < 0)
				revert_b = 0;

			(image->data + (r)*cols + c)->r = (char)revert_r;
			(image->data + (r)*cols + c)->g = (char)revert_g;
			(image->data + (r)*cols + c)->b = (char)revert_b;
		}
}

/*
 * Creates a new image where each pixel is replicated into a 2 by 2 pixel panel with the rgb values the same
 * as the original pixel.
 */
void zoom_in(Image *image)
{
	int cols = image->cols, rows = image->rows;
	int new_rows = 2 * image->rows, new_cols = 2 * cols;
	image->rows = new_rows;
	image->cols = new_cols;
	Pixel *zoomed = (Pixel *)malloc(sizeof(Pixel) * new_rows * new_cols);
	for (int r = 0, count_r = 0; count_r < rows; r += 2, count_r++)
		for (int c = 0, count_c = 0; count_c < cols; c += 2, count_c++)
		{
			unsigned char red = (image->data + (count_r)*cols + count_c)->r, green = (image->data + (count_r)*cols + count_c)->g, blue = (image->data + (count_r)*cols + count_c)->b;
			//Copying red on the same row.
			(zoomed + r * new_cols + c)->r = red;
			(zoomed + r * new_cols + c + 1)->r = red;
			//Copying red onto the next row.
			(zoomed + (r + 1) * new_cols + c)->r = red;
			(zoomed + (r + 1) * new_cols + c + 1)->r = red;

			//Copying green on the same row.
			(zoomed + r * new_cols + c)->g = green;
			(zoomed + r * new_cols + c + 1)->g = green;
			//Copying green onto the next row.
			(zoomed + (r + 1) * new_cols + c)->g = green;
			(zoomed + (r + 1) * new_cols + c + 1)->g = green;

			//Copying blue on the same row.
			(zoomed + r * new_cols + c)->b = blue;
			(zoomed + r * new_cols + c + 1)->b = blue;
			//Copying blue onto the next row.
			(zoomed + (r + 1) * new_cols + c)->b = blue;
			(zoomed + (r + 1) * new_cols + c + 1)->b = blue;
		}
	free(image->data);
	image->data = zoomed;
}

/*
 * Creates a new image where a panel of 2 by 2 pixel is compressed into one pixel.
 * The rgb values of the new pixel is the average values from the original 4 pixels.
 */
void zoom_out(Image *image) //Use (image->data + (r) * cols + c)->r for zoom_out
{
	int cols = image->cols;
	int new_rows = (image->rows) / 2, new_cols = (image->cols) / 2;
	image->cols = new_cols;
	image->rows = new_rows;
	Pixel *zoomed = (Pixel *)malloc(sizeof(Pixel) * new_rows * new_cols);
	for (int r = 0, count_r = 0; r < new_rows; r++, count_r += 2)
		for (int c = 0, count_c = 0; c < new_cols; c++, count_c += 2)
		{
			//Obtains all the rgb values from a 2 by 2 panel of pixels.
			int red1 = (image->data + (count_r)*cols + count_c)->r,
				green1 = (image->data + (count_r)*cols + count_c)->g,
				blue1 = (image->data + (count_r)*cols + count_c)->b;
			int red2 = (image->data + (count_r)*cols + count_c + 1)->r,
				green2 = (image->data + (count_r)*cols + count_c + 1)->g,
				blue2 = (image->data + (count_r)*cols + count_c + 1)->b;
			int red3 = (image->data + (count_r + 1) * (cols) + count_c)->r,
				green3 = (image->data + (count_r + 1) * (cols) + count_c)->g,
				blue3 = (image->data + (count_r + 1) * (cols) + count_c)->b;
			int red4 = (image->data + (count_r + 1) * (cols) + count_c + 1)->r,
				green4 = (image->data + (count_r + 1) * (cols) + count_c + 1)->g,
				blue4 = (image->data + (count_r + 1) * (cols) + count_c + 1)->b;
			int red = (red1 + red2 + red3 + red4) / 4,
				green = (green1 + green2 + green3 + green4) / 4,
				blue = (blue1 + blue2 + blue3 + blue4) / 4;

			(zoomed + r * new_cols + c)->r = red;
			(zoomed + r * new_cols + c)->g = green;
			(zoomed + r * new_cols + c)->b = blue;
		}
	free(image->data);
	image->data = zoomed;
}

/*
 * Sets all the pixels enclosed by the rectangle specified by parameters to black. row1 and col1
 * specify the top left corner of the rectangle while row2 and col2 specify the bottom right corner.
 */
void occlude(Image *image, int row1, int col1, int row2, int col2)
{
	int cols = image->cols;
	for (int r = row1; r <= row2; r++)
		for (int c = col1; c <= col2; c++)
		{
			(image->data + (r)*cols + c)->r = 0; 
			(image->data + (r)*cols + c)->g = 0;
			(image->data + (r)*cols + c)->b = 0;
		}
}

/*
 * Blurs an image by applying a Gaussian filter matrix to each pixel to obtained a weighted average.
 * Each pixel's value is set to the weighted average.
 */
void blur(Image *image, double sigma)
{
	//determine size of filter array.
	int size = sigma * 10;
	if (!(size % 2))
		++size;
	//Only one quadrant of the filter is needed as the other three are replicas.
	size = size / 2 + 1;

	//generating weights
	double *weights = gaus_array(size, sigma);

	int rows = image->rows, cols = image->cols;
	Pixel * blurred = (Pixel *)malloc(sizeof(Pixel) * rows * cols);
	//applying weights to pixels
	for (int r = 0; r < rows; r++)
		for (int c = 0; c < cols; c++)
			apply_weights(image, weights, r, c, size, blurred);

	free(image->data);
	image->data = blurred;
	free(weights);
}

/*
 * Applying the Gaussian filter to pixels surrounding the centered pixel to obtain a weighted average.
 * The weighted average becomes the new values for the pixel.
 */
void apply_weights(Image *image, double *weights, int centered_r,
				   int centered_c, int size, Pixel * blurred)
{
	//these are the max rows and cols in the image.
	int max_row = image->rows, max_col = image->cols;
	//the number of rows and cols needed for the filter to not go out of bound.
	int half = size - 1;
	//These tell the for loop how many pixels to span.
	int start_r, end_r, start_c, end_c;

	//limiting the col range
	start_c = centered_c - half < 0 ? 0 : centered_c - half;
	end_c = centered_c + half >= max_col ? max_col - 1 : centered_c + half;

	//limiting the row range
	start_r = centered_r - half < 0 ? 0 : centered_r - half;
	end_r = centered_r + half >= max_row ? max_row - 1 : centered_r + half;

	double weighted_red = 0.0, weighted_green = 0.0, weighted_blue = 0.0, weight_sum = 0;
	for (int r = start_r; r <= end_r; r++)
		for (int c = start_c; c <= end_c; c++)
		{
			//Obtaining the rows and column of the weight for the corresponding pixel.
			int r_weights = abs(r - centered_r), c_weights = abs(
													 c - centered_c);
			double weight = *(weights + r_weights * size + c_weights);
			weighted_red += weight * (image->data + (r)*max_col + c)->r;
			weighted_green += weight * (image->data + (r)*max_col + c)->g;
			weighted_blue += weight * (image->data + (r)*max_col + c)->b;
			weight_sum += weight;
		}

	weighted_red /= weight_sum;
	weighted_green /= weight_sum;
	weighted_blue /= weight_sum;

	(blurred + (centered_r)*max_col + centered_c)->r = weighted_red;
	(blurred + (centered_r)*max_col + centered_c)->g = weighted_green;
	(blurred + (centered_r)*max_col + centered_c)->b = weighted_blue;
}
/*
 * Fills filter array using the Gaussian Distribution formula. Only populates one quadrant as all others are the same.
 * Size is the size of one quadrant.
 */
double *gaus_array(int size, double sigma)
{
	double *weights = malloc(sizeof(double) * size * size);
	for (int r = 0; r < size; r++)
		for (int c = 0; c < size; c++)
		{
			double weight = (1.0 / (2.0 * PI * sigma * sigma)) * exp(-(r * r + c * c) / (2 * (sigma * sigma)));
			*(weights + r * size + c) = weight;
		}
	return weights;
}
