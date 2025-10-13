#define _CRT_SECURE_NO_WARNINGS

#define FILE_NAME "simple.csv"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// �迭 ��/�� ������ ���ϴ� �Լ�
int* getSizes(FILE* fp, int* iSize) {
	char str_tmp[1024];
	char* token;
	int* jSize = (int*)malloc(sizeof(int) * (*iSize));

	while (fgets(str_tmp, sizeof(str_tmp), fp) != NULL) {
		(*iSize)++;
		jSize = (int*)realloc(jSize, sizeof(int) * *iSize);
		jSize[*iSize - 1] = 0;
		token = strtok(str_tmp, ",");
		while (token != NULL) {
			jSize[*iSize - 1]++;
			token = strtok(NULL, ",");
		}
	}
	return jSize;
}

// �迭 ���� �Ҵ� �� ���� ���� �����ϴ� �Լ�
int** allocateArray(FILE* fp, int iSize, int* jSize) {
	char str_tmp[1024];
	char* token = NULL;

	// �迭 ���� ����
	int** scores = (int**)malloc(sizeof(int*) * iSize);
	for (int i = 0; i < iSize; i++) {
		scores[i] = (int*)malloc(sizeof(int) * jSize[i]);
	}

	// �迭�� ���� ���� ����
	fseek(fp, SEEK_SET, 0);
	for (int i = 0; i < iSize; i++) {
		fgets(str_tmp, sizeof(str_tmp), fp);

		token = strtok(str_tmp, ",");
		int num = atoi(token);
		scores[i][0] = num;

		for (int j = 1; j < jSize[i]; j++) {
			token = strtok(NULL, ",");
			num = atoi(token);
			scores[i][j] = num;
		}
	}

	return scores;
}

// ��ü ���� ��� ���ϴ� �Լ�
float getTotalAverage(int** scores, int iSize, int* jSize) {
	int sum = 0;
	int cnt = 0;
	float avg;

	for (int i = 0; i < iSize; i++) {
		for (int j = 0; j < jSize[i]; j++) {
			cnt++;
			sum += scores[i][j];
		}
	}
	avg = (float)sum / cnt;
	return avg;
}

// ��ü ���� �л� ���ϴ� �Լ�
float getTotalVariance(int** scores, int iSize, int* jSize, float avg) {	
	float sum = 0;
	int cnt = 0;
	
	for (int i = 0; i < iSize; i++) {
		for (int j = 0; j < jSize[i]; j++) {
			cnt++;
			sum += (scores[i][j] - avg)* (scores[i][j] - avg);
		}
	}	
	return sum / cnt;
}

// �л��� ��� ���ϴ� �Լ�
float* getStudentAverages(int** scores, int iSize, int* jSize) {
	int sum;
	float* avg = (float*)malloc(sizeof(float) * iSize);

	for (int i = 0; i < iSize; i++) {
		sum = 0;
		for (int j = 0; j < jSize[i]; j++) {
			sum += scores[i][j];
		}
		avg[i] = (float)sum / jSize[i];
	}
	return avg;
}

// ���� ���� ��� ���ϴ� �Լ�
float* getSubjectAverages(int** scores, int iSize, int* jSize, int maxlen) {
	int sum;	
	int count = 0;

	float* avg = (float*)malloc(sizeof(float) * maxlen);

	for (int j = 0; j < maxlen; j++) {
		sum = 0;
		count = 0;
		for (int i = 0; i < iSize; i++) {
			if (j >= jSize[i]) continue;
			sum += scores[i][j];
			count++;
		}
		avg[j] = (float)sum / count;
	}
	return avg;
}

int main(void) {
	int** scores;	// ������ ���� �迭		
	int iSize = 0;	// �迭 �� ���� (�л� ��)
	int* jSize;	// �迭 �ະ �� ���� (���� ��)

	float totalAverage;		// ��ü ���� ���
	float totalVariance;	// ��ü ���� �л�
	float* student_averages;	// �л��� ���� ���
	float* subject_averages;	// ���� ���� ���

	char* students[] = { "ȫ�浿", "������" };
	char* subjects[] = { "����","����","����","����" };


	// ���� ����
	FILE* fp = fopen(FILE_NAME, "rt");
	if (fp == NULL) {
		printf("������ �о���� ���߽��ϴ�.\n");
		return -1;
	}

	// �迭 ��/�� ������ ���ϱ�
	jSize = getSizes(fp, &iSize);

	// �迭 ���� �Ҵ� �� ���� ���� ����
	scores = allocateArray(fp, iSize, jSize);

	// ��ü ���� ���/�л� ���ϱ�
	totalAverage = getTotalAverage(scores, iSize, jSize);			
	totalVariance = getTotalVariance(scores, iSize, jSize, totalAverage);

	// �л��� ��� ���ϱ�
	student_averages = getStudentAverages(scores, iSize, jSize);

	// �ִ� ���� ���� ���ϱ�
	int maxlen = 0;	
	for (int i = 0; i < iSize; i++) {
		if (jSize[i] > maxlen) maxlen = jSize[i];
	}

	// ���� ��� ���ϱ�
	subject_averages = getSubjectAverages(scores, iSize, jSize, maxlen);


	// ��ü ���� ���/�л� ���
	printf("��ü ���� ��� : %.2f\n", totalAverage);
	printf("��ü ���� �л� : %.2f\n\n", totalVariance);

	// �л� ��� ���
	printf("===�л� ���� ���===\n");
	for (int i = 0; i < iSize; i++) {
		printf("%s�� ���� ��� : %.2f\n", students[i], student_averages[i]);
	}
	printf("\n");

	// ���� ��� ���� ���
	printf("===���� ��� ����===\n");
	for (int i = 0; i < maxlen; i++) {
		printf("%s ������ ��� ���� : %.2f\n", subjects[i], subject_averages[i]);
	}

	// �޸� ����
	fclose(fp);
	free(jSize);
	for (int i = 0; i < iSize; i++) {
		free(scores[i]);
	}
	free(scores);
	free(student_averages);
	free(subject_averages);
	return 0;
}