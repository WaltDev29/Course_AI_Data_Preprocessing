#define _CRT_SECURE_NO_WARNINGS

#define MIN(x,y) x < y ? x: y
#define MAX(x,y) x > y ? x: y
#define NUM_DIGITS 10
#define NUM_FONTS 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <math.h>


HWND hwnd;	// handle window (������ �ٷ��)
HDC hdc;

// pbm ���� �о�ͼ� �����ϴ� ���α׷�

enum FORMAT { EMPTY, GREY, RGB, YCBCR, YCBCR420, BLOCK };

typedef struct {		// �� 21 byte
	unsigned int rows;
	unsigned int cols;
	char format;
	unsigned long total;	// Bytes per image
	unsigned int levels;	// Number of gray/each color levels
	short* content;
} ImageType;
typedef ImageType* Image;

void DrawImage(Image im);	// ���� �̹����� ǥ���ϴ� �Լ�
void DrawHistogram(Image im, short* horizontal, short* vertical);	// ������׷��� ǥ���ϴ� �Լ�

void cal_histogram(Image im, short* horizontal, short* vertical);	// ������׷��� ����ϴ� �Լ�
void cal_variance_bias(Image im, float variance_arr[], float bias_arr[], short* horizontal, short* vertical);	// ������׷� ���/�л��� ���ϴ� �Լ�
void classification(Image im, float minMaxRange[][8], char results[], float variance_arr[], float bias_arr[]);	// �̹����� �з��ϴ� �Լ�
void equalize(Image im);	// ������׷� ��Ȱȭ�� �����ϴ� �Լ�

Image imageAllocate(unsigned int rows, unsigned int cols, char format, unsigned int levels);
Image readPBMImage(const char* filename);
void writePBMImage(const char* filename, const Image im);


int main(void) {
	Image img = 0;

	float var_bias_arr[NUM_DIGITS][NUM_FONTS][4];	// �� ����, ��Ʈ�� ��հ� �л��� �����ϴ� �迭
	float variance_arr[2];	// ����� �����ϴ� �ӽ� �迭
	float bias_arr[2];	// �л��� �����ϴ� �ӽ� �迭

	// �� ����,��Ʈ�� ���/�л��� ���ϴ� �κ�
	for (int i = 0; i < NUM_DIGITS; i++) {
		for (int j = 0; j < NUM_FONTS; j++) {
			char imgPath[] = "./pgm/no0-1.pgm";
			imgPath[8] = '0' + i;
			imgPath[10] = '1' + j;
			img = readPBMImage(imgPath);
			short* horizontal = malloc(sizeof(short) * img->cols);
			short* vertical = malloc(sizeof(short) * img->rows);
			cal_histogram(img, horizontal, vertical);
			cal_variance_bias(img, variance_arr, bias_arr, horizontal, vertical);
			var_bias_arr[i][j][0] = variance_arr[0];
			var_bias_arr[i][j][1] = variance_arr[1];
			var_bias_arr[i][j][2] = bias_arr[0];
			var_bias_arr[i][j][3] = bias_arr[1];
			free(horizontal);
			free(vertical);
		}
	}

	// ���ں� ���/�л��� ������ ���ϴ� �κ�
	float minMaxRange[NUM_DIGITS][8];
	for (int i = 0; i < NUM_DIGITS; i++) {
		for (int j = 0; j < 4; j++) {
			float v0 = var_bias_arr[i][0][j];
			float v1 = var_bias_arr[i][1][j];
			float v2 = var_bias_arr[i][2][j];

			float mn = fminf(v0, fminf(v1, v2));
			float mx = fmaxf(v0, fmaxf(v1, v2));

			minMaxRange[i][j * 2] = mn;
			minMaxRange[i][j * 2 + 1] = mx;
		}
		// �� ���ں� ���/�л��� ����/�ְ�ġ ���
		//printf("--- %d ---\n", i);
		//printf("var_row_min : %.2f\n", minMaxRange[i][0]);
		//printf("var_row_max : %.2f\n", minMaxRange[i][1]);
		//printf("var_col_min : %.2f\n", minMaxRange[i][2]);
		//printf("var_col_max : %.2f\n", minMaxRange[i][3]);
		//printf("bias_row_min : %.2f\n", minMaxRange[i][4]);
		//printf("bias_row_max : %.2f\n", minMaxRange[i][5]);
		//printf("bias_col_min : %.2f\n", minMaxRange[i][6]);
		//printf("bias_col_max : %.2f\n\n", minMaxRange[i][7]);
	}

	// ���� �̹����� �о� �з��ϴ� �κ�
	while (1) {
		for (int i = 0; i < NUM_DIGITS; i++) {
			for (int j = 0; j < NUM_FONTS; j++) {
				system("cls");
				char imgPath[] = "./pgm/no0-1.pgm";
				imgPath[8] = '0' + i;
				imgPath[10] = '1' + j;
				img = readPBMImage(imgPath);
				DrawImage(img);
				short* horizontal = (short*)malloc(sizeof(short) * img->cols);
				short* vertical = (short*)malloc(sizeof(short) * img->rows);
				char results[10] = { 0 };
				cal_histogram(img, horizontal, vertical);
				cal_variance_bias(img, variance_arr, bias_arr, horizontal, vertical);
				classification(img, minMaxRange, results, variance_arr, bias_arr);
				printf("results : ");
				for (int i = 0; i < 10; i++) {
					if (results[i] >= 1) printf("%d ", i);
				}
				printf("\n");
				free(horizontal);
				free(vertical);

				Sleep(1000);
			}
		}
	}


	// ������׷��� ���/�л��� ����ϴ� �κ�
	/*while (1) {
		for (int i = 0; i < NUM_DIGITS; i++) {
			for (int j = 0; j < NUM_FONTS; j++) {
				system("cls");
				char imgPath[] = "./pgm/no0-1.pgm";
				imgPath[8] = '0' + i;
				imgPath[10] = '1' + j;
				img = readPBMImage(imgPath);
				DrawImage(img);

				short* horizontal = malloc(sizeof(short) * img->cols);
				short* vertical = malloc(sizeof(short) * img->rows);
				cal_histogram(img, horizontal, vertical);

				printf("horizontal variance : %.2f\n", var_bias_arr[i][j][0]);
				printf("vertical variance : %.2f\n", var_bias_arr[i][j][1]);
				printf("horizontal bias : %.2f\n", var_bias_arr[i][j][2]);
				printf("vertical bias : %.2f\n", var_bias_arr[i][j][3]);
				DrawHistogram(img, horizontal, vertical);
				free(horizontal);
				free(vertical);
				Sleep(1000);
			}
		}
	}*/
	return 0;
}

// ���� �̹����� ǥ���ϴ� �Լ�
void DrawImage(Image im) {
	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd);
	for (int i = 0; i < im->rows; ++i) {
		for (int j = 0; j < im->cols; j++) {
			SetPixel(hdc, 250 + i, 250 + j, RGB(
				im->content[j * im->cols + i],
				im->content[j * im->cols + i],
				im->content[j * im->cols + i]
			));
		}
	}
}

// ������׷��� ����ϴ� �Լ�
void cal_histogram(Image im, short* horizontal, short* vertical) {
	// �迭 �ʱ�ȭ
	for (int i = 0; i < im->cols; i++) horizontal[i] = 0;
	for (int i = 0; i < im->rows; i++) vertical[i] = 0;

	// ����/���θ� ��ȸ�ϸ� �� ��ǥ�� �ȼ� ���� ���
	for (int y = 0; y < im->rows; ++y)
		for (int x = 0; x < im->cols; x++) {
			if (im->content[y * im->cols + x] < 10) horizontal[x]++;
		}
	for (int x = 0; x < im->cols; ++x) {
		for (int y = 0; y < im->rows; y++) {
			if (im->content[y * im->cols + x] < 10) vertical[y]++;
		}
	}
}

// ������׷��� ǥ���ϴ� �Լ�
void DrawHistogram(Image im, short* horizontal, short* vertical) {

	hwnd = GetForegroundWindow();
	hdc = GetWindowDC(hwnd);

	// ����/���� ������׷��� ǥ��
	for (int i = 0; i < im->cols; i++) {
		for (int j = 0; j < horizontal[i] * 5; j++) {
			SetPixel(hdc, 250 + i, 250 - j, RGB(0, 255, 0));
		}
	}
	for (int i = 0; i < im->rows; i++) {
		for (int j = 0; j < vertical[i] * 5; j++) {
			SetPixel(hdc, 250 - j, 250 + i, RGB(0, 255, 0));
		}
	}
}

// ������׷� ���/�л��� ���ϴ� �Լ�
void cal_variance_bias(Image im, float variance_arr[], float bias_arr[], short* horizontal, short* vertical) {
	// �� (��ǥ*�ȼ�)���� ����
	short horizontal_sum = 0;
	short vertical_sum = 0;

	// ����/���κ� �� �ȼ� ����
	short horizontal_count = 0;
	short vertical_count = 0;

	// �� �ȼ� ������ (��ǥ*�ȼ�)���� ������ ���ϴ� �ݺ���
	for (int i = 0; i < im->cols; i++) {
		horizontal_count += horizontal[i];
		horizontal_sum += (i + 1) * horizontal[i];
	}
	for (int i = 0; i < im->rows; i++) {
		vertical_count += vertical[i];
		vertical_sum += (i + 1) * vertical[i];
	}

	// ��� ���ϱ�
	float horizontal_variance = horizontal_sum / (float)horizontal_count;
	float vertical_variance = vertical_sum / (float)vertical_count;

	// ��� ��ȯ
	variance_arr[0] = horizontal_variance;
	variance_arr[1] = vertical_variance;

	short horizontal_bias_sum = 0;
	short vertical_bias_sum = 0;

	// �ȼ� ���� * (��ǥ - ���)�� ���ϴ� �ݺ���
	for (int i = 0; i < im->cols; i++) {
		horizontal_bias_sum += horizontal[i] * pow((i + 1) - horizontal_variance, 2);
	}
	for (int i = 0; i < im->rows; i++) {
		vertical_bias_sum += vertical[i] * pow((i + 1) - vertical_variance, 2);
	}

	// �л� ��ȯ
	bias_arr[0] = horizontal_bias_sum / (float)horizontal_count;
	bias_arr[1] = vertical_bias_sum / (float)vertical_count;
}

// �̹����� �з��ϴ� �Լ�
void classification(Image im, float minMaxRange[][8], char results[], float variance_arr[], float bias_arr[]) {
	int num;
	for (int i = 0; i < NUM_DIGITS; i++) {
		for (int j = 0; j < 2; j++) {
			if ((minMaxRange[i][0] <= variance_arr[0] && variance_arr[0] <= minMaxRange[i][1]) && (minMaxRange[i][2] <= variance_arr[1] && variance_arr[1] <= minMaxRange[i][3]))
				if ((minMaxRange[i][4] <= bias_arr[0] && bias_arr[0] <= minMaxRange[i][5]) && (minMaxRange[i][6] <= bias_arr[1] && bias_arr[1] <= minMaxRange[i][7]))
					results[i] = 1;
		}
	}
}

// ������׷� ��Ȱȭ�� �����ϴ� �Լ�
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