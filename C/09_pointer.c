#include <stdio.h>

void test1(int* a) {
	a[0] = 1;
}

void test2(int a[]) {
	*(a + 1) = 1;
}

int main(void) {
	int abc = 10;	// 4byte ��ȣ�� �ִ� ������
	short def = 10;	// 2byte ��ȣ�� �ִ� ������

	int num[] = { 0,129,2,3 };

	char* pnum = 0;	// char�� ������
	pnum = num;	// int�� �迭�� �ּҸ� ����Ű��
	pnum++;		// short���̹Ƿ� �ּҰ��� ++�ص� 1byte�� ������.
				// �׷��ϱ� �迭ó�� ���ӵǴ� �ڷ����� ��� ������ �ڷ����� �� �������.
	pnum++;
	pnum++;
	pnum++;

	printf("%d\n", *pnum);	// char���� -127~128�̶� 129�� ����� ǥ������ ����. �����÷ο� �ż� -127����

	printf("num[0] : %d\n", num[0]);
	test1(num);
	printf("num[0] : %d\n", num[0]);

	printf("num[1] : %d\n", num[1]);
	test2(num);
	printf("num[1] : %d\n", num[1]);

	return 0;
}