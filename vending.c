#include <stdio.h>
#include <stdlib.h>
#include "vending.h"

// Enter 입력 받기
void waitEnter(char* str) {
	printf("%s", str);
	while (getchar() != '\n');
}
// 파일 유효성 검사
void checkFileOpen(FILE* fp) {
	if (fp == NULL) {
		printf("파일 불러오기 실패\n");
		printf("프로그램을 종료합니다.\n");
		exit(-1);
	}
}