#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void) {
	char name[10];
	char sex;
	int age;
	int ret;

	FILE* fp = fopen("friends.txt", "rt");

	if (fp == NULL) {
		printf("���� �ҷ����� ����");
		return -1;
	}

	printf("%-7s %-7s %-7s\n", "�̸�", "����", "����");
	while ((ret = fscanf(fp, "%s %c %d", name, &sex, &age)) != EOF) {
		printf("%-7s %-7c %-7d\n", name, sex, age);				
	}

	fclose(fp);

	return 0;
}