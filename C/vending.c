#include <stdio.h>
#include <stdlib.h>
#include "vending.h"

// Enter �Է� �ޱ�
void waitEnter(char* str) {
	printf("%s", str);
	while (getchar() != '\n');
}
// ���� ��ȿ�� �˻�
void checkFileOpen(FILE* fp) {
	if (fp == NULL) {
		printf("���� �ҷ����� ����\n");
		printf("���α׷��� �����մϴ�.\n");
		exit(-1);
	}
}