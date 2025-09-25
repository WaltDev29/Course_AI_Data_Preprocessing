#define _CRT_SECURE_NO_WARNINGS

#define FILE_NAME "simple.csv"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 배열 행/열 사이즈 구하는 함수
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

// 배열 동적 할당 및 파일 내용 대입하는 함수
int** allotArray(FILE* fp, int iSize, int* jSize) {
	char str_tmp[1024];
	char* token = NULL;

	// 배열 동적 생성
	int** scores = (int**)malloc(sizeof(int*) * iSize);	
	for (int i = 0; i < iSize; i++) {
		scores[i] = (int*)malloc(sizeof(int) * jSize[i]);
	}

	// 배열에 파일 내용 대입
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
	int** scores;	// 점수들 넣을 배열		
	int iSize = 0;	// 배열 행 개수
	int* jSize;	// 배열 행별 열 개수

	// 파일 열기
	FILE* fp = fopen(FILE_NAME, "rt");
	if (fp == NULL) {
		printf("파일을 읽어오지 못했습니다.\n");
		return -1;
	}

	// 배열 행/열 사이즈 구하기
	jSize = getSizes(fp, &iSize);

	// 배열 행/열 사이즈 출력
	//printf("iSize : %d\n", iSize);
	//for (int i = 0; i < 2; i++) {
	//	printf("jSize[%d] : %d\n", i, jSize[i]);
	//}
	//printf("\n");

	// 배열 동적 할당 및 파일 내용 대입
	scores = allotArray(fp, iSize, jSize);

	// 배열 출력
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
// 1. 애들 평균 계산하기
// 2. 과목별 평균 계산하기