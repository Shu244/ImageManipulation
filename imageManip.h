/*
 *imageManip.h
 * Created on: Oct 13, 2018
 * Author: Shuhao Lai & Steven Tan
 */

#ifndef IMAGEMANIP_H_
#define IMAGEMANIP_H_
#include "ppm_io.h"

void swap(Image * image);

void grayscale(Image * image);

void contrast(Image * image, double factor);

void zoom_in(Image * image);

void zoom_out(Image * image);

void occlude(Image * image,int row1, int col1, int row2, int col2);

void blur(Image * image, double sigma);

void apply_weights(Image * image, double * weights, int centered_r,
		int centered_c, int size, Pixel * blurred);

double * gaus_array(int size, double sigma);

#endif /* IMAGEMANIP_H_ */
