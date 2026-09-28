/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Omar Elgedawy
**
**
** DATE:        2026-09-28
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color *color = malloc(sizeof(Color));
	if(color == NULL)return NULL;
	uint8_t b = image->image[row*image->cols+col]->B & 1;
	color->R = b ? 255 : 0;
	color->G = b ? 255 : 0;
	color->B = b ? 255 : 0;
	return color;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *newImage = malloc(sizeof(Image));
	if(newImage == NULL)return NULL;
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	int total = newImage->rows*newImage->cols;
	newImage->image = malloc(total*sizeof(Color*));
	if(newImage->image == NULL){
		free(newImage);
		return NULL;
	}
	for(int i=0;i<newImage->rows;i++){
		for(int j=0;j<newImage->cols;j++){
			Color *p = evaluateOnePixel(image,i,j);
			if(p==NULL){
				for (int k = 0; k < i * newImage->cols + j; k++) free(newImage->image[k]);
       			free(newImage->image);
				free(newImage);
				return NULL;
			}
			newImage->image[i*newImage->cols+j] = p;
		}
	}
	return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if(argc != 2){
		printf("usage: %s filename\n", argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		exit(-1);
	}
	Image *image = readData(argv[1]);
	fprintf(stderr, "rows=%u cols=%u\n", image->rows, image->cols);
	if(image == NULL){
	   fprintf(stderr, "Error reading image data from file %s\n", argv[1]);
		exit(-1);
	}
	Image *secret = steganography(image);
	if(secret == NULL){
		printf("Error creating secret image\n");
		freeImage(image);
		exit(-1);
	}
	writeData(secret);
	freeImage(image);
	freeImage(secret);
	return 0;
}
