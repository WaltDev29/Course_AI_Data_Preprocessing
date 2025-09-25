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
int** allotArray(FILE* fp, int iSize, int* jSize) {
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

int main(void) {	
	int** scores;	// ������ ���� �迭		
	int iSize = 0;	// �迭 �� ����
	int* jSize;	// �迭 �ະ �� ����

	// ���� ����
	FILE* fp = fopen(FILE_NAME, "rt");
	if (fp == NULL) {
		printf("������ �о���� ���߽��ϴ�.\n");
		return -1;
	}

	// �迭 ��/�� ������ ���ϱ�
	jSize = getSizes(fp, &iSize);

	// �迭 ��/�� ������ ���
	//printf("iSize : %d\n", iSize);
	//for (int i = 0; i < 2; i++) {
	//	printf("jSize[%d] : %d\n", i, jSize[i]);
	//}
	//printf("\n");

	// �迭 ���� �Ҵ� �� ���� ���� ����
	scores = allotArray(fp, iSize, jSize);

	// �迭 ���
	//for (int i = 0; i < iSize; i++) {
	//	for (int j = 0; j < jSize[i]; j++) {
	//		printf("scores[%d][%d] = %d\n", i, j, scores[i][j]);
	//	}
	//	printf("\n");
	//}



	fclose(fp);
	free(jSize);
	free(scores);
	return 0;
}

// to-do
// 1. �ֵ� ��� ����ϱ�
// 2. ���� ��� ����ϱ�