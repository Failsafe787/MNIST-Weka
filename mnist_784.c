#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <byteswap.h>

/* 
 * Authors: subwave07
 * Version: 1.0.0
 * Last Revision: September 17th, 2018
 * Comment: a simple conversion tool for converting the MNIST IDX format 
 * into WEKA ARFF. Based on the Java converter found at
 * https://github.com/aywi/hdr-mnist-weka/tree/master/src/hdr/mnist/weka
 */

// Endianess detection (for fread function)
const int i = 1;
#define is_bigendian() ( (*(char*)&i) == 0 )

// Function prototypes
size_t fread_endian(void *ptr, size_t size, size_t nmemb, FILE *stream);
int check_files(FILE *labels, FILE *images);
long currentTimeMillis();
FILE* open(char *filename, char *mode);
void close(FILE* filepointer);
void help();

int main(int argc, char *argv[]){
	// Check arguments number
	if(argc!=5){
		help(argv[0]);
		return 0;
	}
	long start = currentTimeMillis();
	FILE *labels = open(argv[2],"r");
	FILE *images = open(argv[3],"r");
	if(!check_files(labels, images)){
		return -1;
	}
	int numLabels = 0, numImages = 0;
	fread_endian(&numLabels, sizeof(int), 1, labels);
	fread_endian(&numImages, sizeof(int), 1, images);
	if (numLabels != numImages) {
		printf("The label file and the image file do not contain the same number of items.\n");
		printf("The label file contains %d items\n", numLabels);
		printf("The image file contains %d items\n", numImages);
		return -1;
	}
	int numRows = 0, numCols = 0;
	fread_endian(&numRows, sizeof(int), 1, images);
	fread_endian(&numCols, sizeof(int), 1, images);
	printf("\n\nConverting %s and %s to %s\n\n",argv[2],argv[3],argv[4]);
	FILE *arff = open(argv[4],"w");
	fprintf(arff, "@relation %s\n\n",argv[1]);
	int pixel = 0;
	for (pixel = 1; pixel <= numRows * numCols; pixel++) {
		fprintf(arff, "@attribute pixel%d numeric\n", pixel);
	}
	fputs("@attribute label {0,1,2,3,4,5,6,7,8,9}\n\n@data\n", arff);
	int numLabelsRead = 0, numImagesRead = 0;
	unsigned int buffer = 0;
	while (!feof(labels) && numLabelsRead < numLabels) {
		fread(&buffer, 1, 1, labels);
		unsigned int label = (unsigned int)buffer;
		numLabelsRead++;
		int image[numRows][numCols];
		int row = 0;
		int col = 0;
		for (row = 0; row < numRows; row++) {
			for (col = 0; col < numCols; col++) {
				fread(&buffer, 1, 1, images);
				image[row][col] = buffer;
				fprintf(arff, "%d,", image[row][col]);
			}
		}
		numImagesRead++;
		if (numLabelsRead == numImagesRead){
			fprintf(arff, "%x\n", label);
		}
		if (numLabelsRead % 20 == 0) {
			printf(">");
		}
		if (numLabelsRead % 1000 == 0) {
			printf(" %d/%d", numLabelsRead, numLabels);
			long end = currentTimeMillis();
			long elapsed = end - start;
			long minutes = elapsed / (1000 * 60);
			long seconds = elapsed / 1000 - minutes * 60;
			printf(" %ld'%ld''\n", minutes, seconds);
		}
	}
	close(arff);
	close(labels);
	close(images);
	long end = currentTimeMillis();
	long elapsed = end - start;
	long minutes = elapsed / (1000 * 60);
	long seconds = elapsed / 1000 - minutes * 60;
	printf("\n\n%d items converted in %ld'%ld''.\n\n", numLabelsRead, minutes, seconds);
	return 0;
}

size_t fread_endian(void *ptr, size_t size, size_t nmemb, FILE *stream){
	size_t buffer = 0;
	size_t result = fread(&buffer, size, nmemb, stream);
	if(!is_bigendian()){
		buffer = (size_t)__bswap_32 (buffer);
	}
	*(size_t*)ptr = buffer;
	return result;
}

int check_files(FILE *labels, FILE *images){
	int magic_number = 0; 
	fread_endian(&magic_number, sizeof(int), 1, labels);
	if(magic_number != 2049){
		printf("The label file has wrong magic number: %d (should be 2049). Conversion aborted!\n", magic_number);
		return 0;
	}
	fread_endian(&magic_number, sizeof(int), 1, images);
	if(magic_number != 2051){
		printf("The label file has wrong magic number: %d (should be 2051). Conversion aborted!\n", magic_number);
		return 0;
	}
	return 1;
}

long currentTimeMillis(){
    struct timeval te; 
    gettimeofday(&te, NULL); // get current time
    long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
    return milliseconds;
}

FILE* open(char *filename, char *mode){
	FILE* opened = fopen(filename,mode);
	if(!opened){
		printf("An error occoured while opening %s! Conversion aborted!\n", filename);
		exit(0);
	}
	return opened;
}

void close(FILE* filepointer){
	int result = fclose(filepointer);
	if(result==EOF){
		printf("An error occoured while closing a file! Conversion aborted!\n");
	}
	return;
}

void help(char* progname){
	printf("MNIST Raw File Format to Weka ARFF Converter\n");
	printf("Usage: %s <dataset name> <dataset labels> <dataset images> <output-file-name>\n",progname);
	/*if(is_bigendian()){
		printf("Running on a Big-Endian architecture");
	}
	else{
		printf("Running on a Little-Endian architecture");
	}*/
	return;
}