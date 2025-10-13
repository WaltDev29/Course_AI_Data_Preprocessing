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
int** allocateArray(FILE* fp, int iSize, int* jSize) {
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

// 전체 점수 평균 구하는 함수
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

// 전체 점수 분산 구하는 함수
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

// 학생별 평균 구하는 함수
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

// 과목별 점수 평균 구하는 함수
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
	int** scores;	// 점수들 넣을 배열		
	int iSize = 0;	// 배열 행 개수 (학생 수)
	int* jSize;	// 배열 행별 열 개수 (과목 수)

	float totalAverage;		// 전체 성적 평균
	float totalVariance;	// 전체 성적 분산
	float* student_averages;	// 학생별 성적 평균
	float* subject_averages;	// 과목별 성적 평균

	char* students[] = { "홍길동", "성춘향" };
	char* subjects[] = { "국어","영어","수학","과학" };


	// 파일 열기
	FILE* fp = fopen(FILE_NAME, "rt");
	if (fp == NULL) {
		printf("파일을 읽어오지 못했습니다.\n");
		return -1;
	}

	// 배열 행/열 사이즈 구하기
	jSize = getSizes(fp, &iSize);

	// 배열 동적 할당 및 파일 내용 대입
	scores = allocateArray(fp, iSize, jSize);

	// 전체 점수 평균/분산 구하기
	totalAverage = getTotalAverage(scores, iSize, jSize);			
	totalVariance = getTotalVariance(scores, iSize, jSize, totalAverage);

	// 학생별 평균 구하기
	student_averages = getStudentAverages(scores, iSize, jSize);

	// 최대 과목 개수 구하기
	int maxlen = 0;	
	for (int i = 0; i < iSize; i++) {
		if (jSize[i] > maxlen) maxlen = jSize[i];
	}

	// 과목별 평균 구하기
	subject_averages = getSubjectAverages(scores, iSize, jSize, maxlen);


	// 전체 점수 평균/분산 출력
	printf("전체 점수 평균 : %.2f\n", totalAverage);
	printf("전체 점수 분산 : %.2f\n\n", totalVariance);

	// 학생 평균 출력
	printf("===학생 점수 평균===\n");
	for (int i = 0; i < iSize; i++) {
		printf("%s의 점수 평균 : %.2f\n", students[i], student_averages[i]);
	}
	printf("\n");

	// 과목 평균 점수 출력
	printf("===과목 평균 점수===\n");
	for (int i = 0; i < maxlen; i++) {
		printf("%s 과목의 평균 점수 : %.2f\n", subjects[i], subject_averages[i]);
	}

	// 메모리 해제
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