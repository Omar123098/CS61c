/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color *newColor = malloc(sizeof(Color));
	if(newColor == NULL)
	{
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	newColor->R = 0;
	newColor->G = 0;
	newColor->B = 0;

	Color *me = image->image[row*image->cols + col];

	for(int bit = 0; bit < 8; bit++)
	{
		int cntR = 0;
		int cntG = 0;
		int cntB = 0;
		for(int dx = -1; dx <= 1; dx++)
		{
			for(int dy = -1; dy <= 1; dy++)
			{
				if(dx == 0 && dy == 0)
					continue;
				int neighborRow = (row + dx + image->rows) % image->rows;
				int neighborCol = (col + dy + image->cols) % image->cols;
				Color *neighborColor = image->image[neighborRow*image->cols + neighborCol];
				cntR += (neighborColor->R >> bit) & 1;
				cntG += (neighborColor->G >> bit) & 1;
				cntB += (neighborColor->B >> bit) & 1;
			}
		}
		int idxR = ((me->R>>bit) & 1) ? cntR+9: cntR;
		int idxG = ((me->G>>bit) & 1) ? cntG+9: cntG;
		int idxB = ((me->B>>bit) & 1) ? cntB+9: cntB;

		newColor->R |= ((rule >> idxR) & 1) << bit;
		newColor->G |= ((rule >> idxG) & 1) << bit;
		newColor->B |= ((rule >> idxB) & 1) << bit;
	}
	return newColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *newImage = malloc(sizeof(Image));
	if(newImage == NULL)
	{
		fprintf(stderr, "malloc failed\n");
		return NULL;
	}

	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = malloc(sizeof(Color*) * newImage->rows * newImage->cols);
	if(newImage->image == NULL)
	{
		fprintf(stderr, "malloc failed\n");
		free(newImage);
		return NULL;
	}
	for(int row = 0; row < newImage->rows; row++)
	{
		for(int col = 0; col < newImage->cols; col++)
		{
			Color *newColor = evaluateOneCell(image, row, col, rule);
			if(newColor == NULL){
				for(int i=0;i<newImage->rows*newImage->cols;i++){
					free(newImage->image[i]);
				}
				free(newImage->image);
				free(newImage);
				return NULL;
			}
			newImage->image[row*newImage->cols + col] = newColor;
		}
	}
	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("usage: ./gameOfLife filename rule\n");
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		return -1;
	}

	uint32_t rule = strtol(argv[2], NULL, 16);

	Image *image = readData(argv[1]);
	if (image == NULL) {
		return -1;
	}

	Image *next = life(image, rule);
	if (next == NULL) {
		freeImage(image);
		return -1;
	}

	writeData(next);

	freeImage(image);
	freeImage(next);
	return 0;
}
