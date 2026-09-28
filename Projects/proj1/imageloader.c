/************************************************************************
**
** NAME:        imageloader.c
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
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *fp = fopen(filename,"r");
	if(fp == NULL)
		return NULL;
	char format[3];
	int cols,rows,maxval;
	fscanf(fp,"%s %d %d %d",format,&cols,&rows,&maxval);
	Image *image = malloc(sizeof(Image));
	image -> rows = rows;
	image -> cols = cols;
	int total = rows*cols;
	image -> image = malloc(total*sizeof(Color*));
	if(image->image == NULL){
		free(image);
		fclose(fp);
		return NULL;
	}
	for(int i=0;i<total;i++){
		Color *color = malloc(sizeof(Color));
		fscanf(fp,"%3hhu %3hhu %3hhu",&color->R,&color->G,&color->B);
		image -> image[i] = color;
	}
	fclose(fp);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n%u %u\n255\n", image->cols, image->rows);
	for(int i=0;i<image->rows;i++){
		for(int j=0;j<image->cols;j++){
			Color *p = image->image[i*image->cols+j];
			if(j>0)printf("   ");
			printf("%3u %3u %3u",p->R,p->G,p->B);
		}
		printf("\n");
	}
}

//Frees an image
void freeImage(Image *image)
{
	int total = image->rows * image->cols;
	for(int i=0;i<total;i++)free(image->image[i]);
	free(image->image);
	free(image);
}