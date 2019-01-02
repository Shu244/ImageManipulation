// ppm_io.h
// 601.220, Fall 2018
// Starter code for midterm project - feel free to edit/add to this file


#ifndef PPM_IO_H
#define PPM_IO_H

#include <stdio.h>

/* A struct to store a single RGB pixel, one byte per color channel.
 */
typedef struct{
  unsigned char r;  //red   channel value
  unsigned char g;  //green channel value
  unsigned char b;  //blue  channel value
} Pixel;


/* A struct to bundle together a pixel array with the other
 * image data we'll frequently want to pass around with it.
 * (This saves us from having to pass the same three 
 * variables to every function.) Note that no Pixels are
 * stored within this struct; the data field is a pointer.
 */
typedef struct{
  Pixel *data;  //pointer to array of Pixels
  int rows;     //number of rows of Pixels
  int cols;     //number of columns of Pixels
} Image;




/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp);


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im);


#endif
