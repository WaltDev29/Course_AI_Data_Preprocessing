#include <stdio.h>

int main(void) {
	int ch;
	FILE* fp = fopen("data.txt", "rt");
	if (fp == NULL) {
		printf("���� ���� ����");
		return -1;
	}

	while (1) {
		ch = fgetc(fp);
		if (ch == EOF) break;
		printf("%c", ch);
	}

	fclose(fp);

	return 0;
}