#define _CRT_SECURE_NO_WARNINGS

#define MIN(x,y) x < y ? x: y
#define MAX(x,y) x > y ? x: y


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


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

void equalize(Image im) {
	int minVal[3] = { 255,255,255 };
	int maxVal[3] = { 0,0,0 };
	float normCoef[3];

	if (im->format == GREY) {
		for (int i = 0; i < im->total; i++) {
			minVal[0] = MIN(im->content[i], minVal[0]);
			maxVal[0] = MAX(im->content[i], maxVal[0]);
		}
	}
	else if (im->format == RGB) {
		for (int i = 0; i < im->total; i += 3) {
			minVal[0] = MIN(im->content[i], minVal[0]);
			maxVal[0] = MAX(im->content[i], maxVal[0]);
			minVal[1] = MIN(im->content[i + 1], minVal[1]);
			maxVal[1] = MAX(im->content[i + 1], maxVal[1]);
			minVal[2] = MIN(im->content[i + 2], minVal[2]);
			maxVal[2] = MAX(im->content[i + 2], maxVal[2]);
		}
	}

	if (im->format == GREY) {
		normCoef[0] = (float)im->levels / (maxVal[0] - minVal[0]);
		for (int i = 0; i < im->total; i++)
			im->content[i] = (im->content[i] - minVal[0]) * normCoef[0];
	}
	else if (im->format == RGB) {
		normCoef[0] = (float)im->levels / (maxVal[0] - minVal[0]);
		normCoef[1] = (float)im->levels / (maxVal[1] - minVal[1]);
		normCoef[2] = (float)im->levels / (maxVal[2] - minVal[2]);
		for (int i = 0; i < im->total; i += 3) {
			im->content[i] = (im->content[i] - minVal[0]) * normCoef[0];
			im->content[i + 1] = (im->content[i + 1] - minVal[1]) * normCoef[1];
			im->content[i + 2] = (im->content[i + 2] - minVal[2]) * normCoef[2];
		}
	}
}

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

void writePBMImage(const char* filename, const Image im) {
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
			fputc((unsigned char)im->content[k], pgmFile);
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

void GetHistiogram(Image im, short* horizontal, short* vertical) {
	for (int i = 0; i < im->cols; i++) horizontal[i] = 0;	
	for (int i = 0; i < im->cols; i++) vertical[i] = 0;
	for (int y = 0; y < im->rows; ++y) {
		for (int x = 0; x < im->cols; ++x) {
			if (im->content[y * im->cols + x] == 0) horizontal[x]++;
		}
	}
	for (int x = 0; x < im->cols; ++x) {
		for (int y = 0; y < im->rows; y++) {
			if (im->content[y * im->cols + x] == 0) vertical[y]++;
		}
	}
}

void GetAverageOfPosition(Image im, short* horizontal, short* vertical, int* v_avg, int* h_avg) {
	int tmpSum = 0;
	int totalCnt = 0;
	for (int i = 0; i < im->cols; i++) {
		tmpSum += horizontal[i] * i;
		totalCnt += horizontal[i];
	}
	*h_avg = tmpSum / totalCnt;

	tmpSum = 0;
	totalCnt = 0;
	for (int i = 0; i < im->rows; i++) {
		tmpSum += vertical[i] * i;
		totalCnt += vertical[i];
	}
	*v_avg = tmpSum / totalCnt;
}

void GetVariance(Image im, short* horizontal, short* vertical, int v_avg, int h_avg, int* var_v, int* var_h) {
	int tmpSum = 0;
	int totalCnt = 0;
	for (int i = 0; i < im->cols; i++) {
		for (int j = 0; j < horizontal[i]; j++) {
			tmpSum += (h_avg - i) * (h_avg - i);
		}
		totalCnt += horizontal[i];
	}
	*var_h = tmpSum / totalCnt;

	tmpSum = 0;
	totalCnt = 0;
	for (int i = 0; i < im->rows; i++) {
		for (int j = 0; j < vertical[i]; j++) {
			tmpSum += (v_avg - i) * (v_avg - i);
		}
		totalCnt += vertical[i];
	}
	*var_v = tmpSum / totalCnt;
}

int main(void) {
	Image img = 0;
	
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 3; j++) {			
			char imgPath[] = "./pgm/no0-1.pgm";
			imgPath[8] = '0' + i;
			imgPath[10] = '1' + j;
			img = readPBMImage(imgPath);	
			short* histo_v = (short*)malloc(sizeof(short) * img->cols);
			short* histo_h = (short*)malloc(sizeof(short) * img->rows);
			int v_avg = 0, h_avg = 0, v_var = 0, h_var = 0;

			GetHistiogram(img, histo_h, histo_v);
			GetAverageOfPosition(img, histo_h, histo_v, &v_avg, &h_avg);
			GetVariance(img, histo_h, histo_v, v_avg, h_avg, &v_var, &h_var);

			printf("%d 이미지 - 폰트타입 : %d\n", i, j);
			printf("가로평균 위치 : %d 세로평균 위치 : %d 가로분산 : %d 세로분산 : %d\n\n", v_avg, h_avg, v_var, h_var);
			
			free(histo_v);
			free(histo_h);
			
		}
	}

	Image im = readPBMImage("frog.pbm");
	equalize(im);
	writePBMImage("frog2.pbm", im);
	return 0;
}