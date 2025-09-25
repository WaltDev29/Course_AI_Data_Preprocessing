#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
	char str_tmp[1024];	// ���ڿ� ������ �ӽ� ����
	int** scores;	// ������ ���� �迭	
	char* token = NULL;	// strtok ��ȯ�� ������ �ӽ� ����
	int iSize = 0;	// �迭 �� ����
	int* jSize;	// �迭 �ະ �� ����

	// ���� ����
	FILE* fp = fopen("simple.csv", "rt");
	if (fp == NULL) {
		printf("������ �о���� ���߽��ϴ�.\n");
		return -1;
	}


	// �迭 ��/�� ������ ���ϱ�
	jSize = (int*)malloc(sizeof(int) * iSize);
	while (fgets(str_tmp, sizeof(str_tmp), fp) != NULL) {
		iSize++;
		jSize = (int*)realloc(jSize, sizeof(int) * iSize);
		jSize[iSize - 1] = 0;
		token = strtok(str_tmp, ",");
		while (token != NULL) {
			jSize[iSize - 1]++;
			token = strtok(NULL, ",");
		}
	}


	// �迭 ��/�� ������ ���
	printf("iSize : %d\n", iSize);
	for (int i = 0; i < 2; i++) {
		printf("jSize[%d] : %d\n", i, jSize[i]);
	}
	printf("\n");

	// �迭 ���� �Ҵ�
	scores = (int**)malloc(sizeof(int*) * iSize);
	for (int i = 0; i < iSize; i++) {
		scores[i] = (int*)malloc(sizeof(int) * jSize[i]);
	}

	// ���� �о �迭�� �Ҵ�
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

	// ���� ���
	for (int i = 0; i < iSize; i++) {
		for (int j = 0; j < jSize[i]; j++) {
			printf("scores[%d][%d] = %d\n", i, j, scores[i][j]);
		}
		printf("\n");
	}

	fclose(fp);
	free(jSize);
	free(scores);
	return 0;
}

// 1. fgets�� �о�� ������ �ε��� ���� (�� ������ Ȯ��) 
// 2. seekó������ ������ malloc���� �迭 �����
// 3. str_tmp �����ؼ� str_cpy����� �̰� token���� ������ �ε��� ���� (�÷� �� Ȯ��)
// 4. �� �ະ�� malloc���� �迭 ���� ����
// 5. �� �ະ�� str_tmp strtok�ؼ� �迭�� �ֱ�
// DONE.