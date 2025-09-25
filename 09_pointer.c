#include <stdio.h>

void test1(int* a) {
	a[0] = 1;
}

void test2(int a[]) {
	*(a + 1) = 1;
}

int main(void) {
	int abc = 10;	// 4byte 부호가 있는 정수형
	short def = 10;	// 2byte 부호가 있는 정수형

	int num[] = { 0,129,2,3 };

	char* pnum = 0;	// char형 포인터
	pnum = num;	// int형 배열의 주소를 가리키면
	pnum++;		// short형이므로 주소값을 ++해도 1byte씩 움직임.
				// 그러니까 배열처럼 연속되는 자료형의 경우 포인터 자료형을 잘 맞춰야함.
	pnum++;
	pnum++;
	pnum++;

	printf("%d\n", *pnum);	// char형은 -127~128이라 129를 제대로 표시하지 못함. 오버플로우 돼서 -127나옴

	printf("num[0] : %d\n", num[0]);
	test1(num);
	printf("num[0] : %d\n", num[0]);

	printf("num[1] : %d\n", num[1]);
	test2(num);
	printf("num[1] : %d\n", num[1]);

	return 0;
}