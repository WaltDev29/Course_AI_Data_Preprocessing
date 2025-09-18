#define _CRT_SECURE_NO_WARNINGS
// 매크로
#define MIN(x,y) x < y ? x: y
#define MAX(x,y) x > y ? x: y
// 리턴타입과 매개변수의 타입을 지정하지 않아도 됨
// 복잡한 함수에는 할 수 없음.
// 매크로에 대해서는 디버깅이 안됨

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// pbm 파일 읽어와서 복사하는 프로그램

enum FORMAT { EMPTY, GREY, RGB, YCBCR, YCBCR420, BLOCK };

typedef struct {		// 총 21 byte
	unsigned int rows;
	unsigned int cols;
	char format;
	unsigned long total;	// Bytes per image
	unsigned int levels;	// Number of gray/each color levels
	short* content;
} ImageType;
typedef ImageType* Image;

Image imageAllocate(unsigned int rows, unsigned int cols, char format, unsigned int levels) {
	Image im = (Image)malloc(sizeof(ImageType));
	im->rows = rows;
	im->cols = cols;
	im->format = format;
	im->levels = levels;
	im->content = NULL;
	if (im->format == EMPTY) return im;

	switch (im->format) {
	case GREY:
		im->total = im->cols * im->rows;
		break;
	case RGB:
		im->total = 3 * im->cols * im->rows;
		break;
	default:
		break;
	}

	im->content = (short*)malloc(im->total * sizeof(short));
	return im;
}

Image readPBMImage(const char* filename) {
	FILE* pgmFile;
	int k;

	char signature[3];
	unsigned int cols, rows, levels;

	Image im = imageAllocate(0, 0, EMPTY, 0);

	pgmFile = fopen(filename, "rb");
	if (pgmFile == NULL) {
		perror("Cannot open file to read!");
		fclose(pgmFile);
		return imageAllocate(0, 0, EMPTY, 0);
	}

	fgets(signature, sizeof(signature), pgmFile);
	if (strcmp(signature, "P5") != 0 && strcmp(signature, "P6") != 0) {
		perror("Wrong file type!");
		fclose(pgmFile);
		return im;
	}

	// read header
	fscanf(pgmFile, "%d %d %d", &cols, &rows, &levels);
	fgetc(pgmFile);

	if (strcmp(signature, "P5") == 0) {
		im = imageAllocate(rows, cols, GREY, levels);
		for (k = 0; k < im->total; ++k) {
			im->content[k] = (unsigned char)fgetc(pgmFile);
		}
	}
	else if (strcmp(signature, "P6") == 0) {
		im = imageAllocate(rows, cols, RGB, levels);
		unsigned long gOffset = im->cols * im->rows;
		unsigned long bOffset = 2 * im->cols * im->rows;
		for (k = 0; k < im->total / 3; ++k) {
			im->content[k] = (unsigned char)fgetc(pgmFile);
			im->content[k + gOffset] = (unsigned char)fgetc(pgmFile);
			im->content[k + bOffset] = (unsigned char)fgetc(pgmFile);
		}
	}

	fclose(pgmFile);
	return im;
}

void writePBMImage(const char* filename, const Image im, unsigned char minVal, float normCoef) {
	FILE* pgmFile;
	int k;

	pgmFile = fopen(filename, "wb");
	if (pgmFile == NULL) {
		perror("Cannot open file to write");
		exit(EXIT_FAILURE);
	}

	if (im->format == GREY) fprintf(pgmFile, "%s ", "P5");
	else if (im->format == RGB) fprintf(pgmFile, "%s", "P6");

	fprintf(pgmFile, "%d %d ", im->cols, im->rows);
	fprintf(pgmFile, "%d ", im->levels);

	if (im->format == GREY) {
		for (k = 0; k < im->total; ++k) {
			fputc((unsigned char)((im->content[k] - minVal) * normCoef), pgmFile);
		}
	}
	else if (im->format == RGB) {
		unsigned long gOffset = im->cols * im->rows;
		unsigned long bOffset = 2 * im->cols * im->rows;
		for (k = 0; k < im->total / 3; ++k) {
			fputc((unsigned char)im->content[k], pgmFile);
			fputc((unsigned char)im->content[k + gOffset], pgmFile);
			fputc((unsigned char)im->content[k + bOffset], pgmFile);
		}
	}

	fclose(pgmFile);
}

int main(void) {
	unsigned char minVal = 255;
	float normCoef = 0;
	Image im = readPBMImage("frog.pbm");
	writePBMImage("frog2.pbm", im, minVal, normCoef);
	return 0;
}
