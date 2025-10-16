#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void) {
	char name[10];
	char sex;
	int age;
	int ret;

	FILE* fp = fopen("friends.txt", "rt");

	if (fp == NULL) {
		printf("파일 불러오기 실패");
		return -1;
	}

	printf("%-7s %-7s %-7s\n", "이름", "성별", "나이");
	while ((ret = fscanf(fp, "%s %c %d", name, &sex, &age)) != EOF) {
		printf("%-7s %-7c %-7d\n", name, sex, age);				
	}

	fclose(fp);

	return 0;
}