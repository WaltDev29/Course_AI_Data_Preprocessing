#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
	char str_tmp[1024];	// 문자열 저장할 임시 버퍼
	int** scores;	// 점수들 넣을 배열	
	char* token = NULL;	// strtok 반환값 저장할 임시 버퍼
	int iSize = 0;	// 배열 행 개수
	int* jSize;	// 배열 행별 열 개수

	// 파일 열기
	FILE* fp = fopen("simple.csv", "rt");
	if (fp == NULL) {
		printf("파일을 읽어오지 못했습니다.\n");
		return -1;
	}


	// 배열 행/열 사이즈 구하기
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


	// 배열 행/열 사이즈 출력
	printf("iSize : %d\n", iSize);
	for (int i = 0; i < 2; i++) {
		printf("jSize[%d] : %d\n", i, jSize[i]);
	}
	printf("\n");

	// 배열 동적 할당
	scores = (int**)malloc(sizeof(int*) * iSize);
	for (int i = 0; i < iSize; i++) {
		scores[i] = (int*)malloc(sizeof(int) * jSize[i]);
	}

	// 파일 읽어서 배열에 할당
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

	// 점수 출력
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

// 1. fgets로 읽어올 때마다 인덱스 증가 (몇 행인지 확인) 
// 2. seek처음으로 돌리고 malloc으로 배열 만들기
// 3. str_tmp 복사해서 str_cpy만들고 이거 token만들 때마다 인덱스 증가 (컬럼 수 확인)
// 4. 각 행별로 malloc으로 배열 만들어서 삽입
// 5. 각 행별로 str_tmp strtok해서 배열에 넣기
// DONE.