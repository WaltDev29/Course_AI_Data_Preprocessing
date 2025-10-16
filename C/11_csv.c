#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void) {
	FILE* fp = fopen("simple.csv", "rt");
	char str_tmp[1024];

	if (fp == NULL) {
		printf("파일을 읽어오지 못했습니다.\n");
		return -1;
	}

	while (fgets(str_tmp, sizeof(str_tmp), fp) != NULL) {				
		//printf("%s", str_tmp);		
		char* p = strtok(str_tmp, ",");		
		while (p != NULL) {
			//printf("%s\n", p);
			int number = atoi(p);
			printf("%d\n", number);
			p = strtok(NULL, ",");
		}
	}

	fclose(fp);
	return 0;
}