#define _CRT_SECURE_NO_WARNINGS
#define FILE_NAME "machine_data.txt"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vending.h"

// ������ ��� ���� �Լ�

// ��й�ȣ Ȯ��
int checkPassword() {
	char password[5] = "1234";
	char inputPassword[5];
	printf("��й�ȣ�� �Է��ϼ��� : ");
	if (scanf_s("%s", inputPassword, sizeof(inputPassword)) != 1 || strcmp(password, inputPassword) != 0) {
		while (getchar() != '\n');
		waitEnter("\n��й�ȣ�� Ʋ�Ƚ��ϴ�.\nEnter�� �Է��Ͽ� ���ư���.\n");
		return 0;
	}
	else return 1;
}
// ������ ��� ù ȭ�� ���
int showAdminScreen() {
	int answer;
	system("cls");
	printf("--- ������ ��� ---\n\n");
	printf("�б� ��� : 1\n���� ��� : 2\n�Ǹ� ��� ���� : 3\n���� : 0\n\n");
	printf("�Է� : ");
	scanf("%d", &answer);
	getchar();
	return answer;
}
// ���� ���� ���
void printFile(FILE* fp) {
	char tmp[100]; // ���� ���� ���� ����
	system("cls");
	while (fgets(tmp, sizeof(tmp), fp) != NULL) {
		printf("%s", tmp);
	}
	waitEnter("\n\nEnterŰ�� ���� ���ư���.\n");
}
// ���� ���� ����
FILE* writeFile(FILE* fp, Drink drinks[], int drinkTypes, int totalSales, int totalRevenue) {
	FILE* temp = fopen("temp.txt", "w+t");
	checkFileOpen(temp);
	fprintf(temp, "���� ���� : %d\n", drinkTypes);
	fprintf(temp, "�� �Ǹŷ� : %d\n", totalSales);
	fprintf(temp, "�� ���� : %d\n\n", totalRevenue);
	fprintf(temp, "---���� ���---\n");
	fprintf(temp, "%-20s %-10s %-10s\n", "�̸�", "����", "���");
	fprintf(temp, "-----------------------------------------------\n");
	for (int i = 0; i < drinkTypes; i++) {
		fprintf(temp, "%-20s %-10d %-10d\n", drinks[i].name, drinks[i].price, drinks[i].stock);
	}
	fclose(fp);
	fclose(temp);
	remove(FILE_NAME);
	rename("temp.txt", FILE_NAME);

	fp = fopen(FILE_NAME, "a+t");
	checkFileOpen(fp);
	return fp;
}
// �Ǹ� �α� ���
void logSales(Drink drinks[], int choice, int totalSales) {
	FILE* fp = fopen("salesLog.txt", "a+t");
	checkFileOpen(fp);

	time_t t = time(NULL);
	struct tm* tm_info = localtime(&t);
	char buffer[100];

	fprintf(fp, "%d.\n", totalSales);
	strftime(buffer, sizeof(buffer), "%Y.%m.%d %H:%M", tm_info);
	fprintf(fp, "%s\n", buffer);
	fprintf(fp, "���� �׸� : %s\n", drinks[choice - 1].name);
	fprintf(fp, "���� : %d��\n\n", drinks[choice - 1].price);

	fclose(fp);
}
// ������ ���� ����
int selectDrinkToEdit(Drink drinks[], int drinkTypes) {
	int selectedDrinkIndex;
	// ���� ��� ���
	printf("---���� ���---\n\n");
	printf("   %-17s %-10s %-10s\n", "�̸�", "����", "���");
	printf("-----------------------------------------------\n");
	for (int i = 0; i < drinkTypes; i++) {
		printf("%d. %-17s %-10d %-10d\n", i + 1, drinks[i].name, drinks[i].price, drinks[i].stock);
	}

	// ������ ���� ����
	printf("\n������ �׸��� �����ϼ���.\n");
	printf("-1 : ���� �߰�\n-2 : ���� ����\n");
	printf("0 : �ڷ� ����\n\n");
	printf("�Է� : ");
	while (scanf("%d", &selectedDrinkIndex) != 1 || (selectedDrinkIndex < -2 || selectedDrinkIndex > drinkTypes)) {
		while (getchar() != '\n');
		printf("\n-2~%d�� ���ڸ� �Է����ּ���.\n", drinkTypes);
		printf("�Է� : ");
	}

	return selectedDrinkIndex;
}
// ������ �׸� ����
int selectFieldToEdit(Drink drinks[], int selectedDrinkIndex) {
	int editField;
	system("cls");
	printf("1. �̸� : %s\n2. ���� : %d\n3. ��� : %d\n\n", drinks[selectedDrinkIndex - 1].name, drinks[selectedDrinkIndex - 1].price, drinks[selectedDrinkIndex - 1].stock);
	printf("������ �׸��� ��ȣ�� �����ϼ���.\n");
	printf("0 : �ڷ� ����\n\n");
	printf("�Է� : ");
	while (scanf("%d", &editField) != 1 || (editField < 0 || editField > 3)) {
		waitEnter("\n0~3�� ���ڸ� �Է����ּ���.\n�Է� : ");
	}

	return editField;
}
// ���� �߰�
void addDrinkList(FILE* fp, Drink** drinks, int* drinkTypes, int totalSales, int totalRevenue) {
	char name[50];
	char yn;
	int price, stock;
	while (1) {
		system("cls");
		printf("�߰��� ������ ������ �����ּ���.\n\n");
		printf("�̸� : ");
		scanf("%s", name);
		printf("���� : ");
		while (scanf("%d", &price) != 1 || price < 0 || price % 10 != 0) {
			while (getchar() != '\n');
			if (price % 10 != 0) {
				printf("\n10�� ������ �Է����ּ���.\n\n");
			}
			else {
				printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
			}
			printf("���� : ");
		}
		printf("��� : ");
		while (scanf("%d", &stock) != 1 || stock < 0) {
			waitEnter("\n�߸��� ���� �Է��߽��ϴ�.\n��� : ");
		}
		while (getchar() != '\n');
		system("cls");
		printf("�Է��� ������ Ȯ�����ּ���\n");
		printf("�̸� : %s\n���� : %d\n��� : %d\n", name, price, stock);
		printf("\n�Է� ������ �����Ͻðڽ��ϱ�? (Q : ó������)\n");
		printf("Y/N : ");
		scanf("%c", &yn);
		while (getchar() != '\n');
		if (yn == 'y' || yn == 'Y') {
			(*drinkTypes)++;
			*drinks = realloc(*drinks, *drinkTypes * sizeof(Drink));
			strcpy((*drinks)[*drinkTypes - 1].name, name);
			(*drinks)[*drinkTypes - 1].price = price;
			(*drinks)[*drinkTypes - 1].stock = stock;

			writeFile(fp, *drinks, *drinkTypes, totalSales, totalRevenue);
			waitEnter("\n���� �߰� �Ϸ�.\nEnter�� ���� ���ư���\n");
			break;
		}
		else if (yn == 'n' || yn == 'N') {
			continue;
		}
		else if (yn == 'q' || yn == 'Q') {
			break;
		}
	}
}
// ���� ����
void deleteDrinkList(FILE* fp, Drink drinks[], int* drinkTypes, int totalSales, int totalRevenue) {
	int selectedDrinkIndex;
	// ���� ��� ���
	system("cls");
	printf("---���� ���---\n\n");
	printf("   %-17s %-10s %-10s\n", "�̸�", "����", "���");
	printf("-----------------------------------------------\n");
	for (int i = 0; i < *drinkTypes; i++) {
		printf("%d. %-17s %-10d %-10d\n", i + 1, drinks[i].name, drinks[i].price, drinks[i].stock);
	}

	// ������ ���� ����
	printf("\n������ ���Ḧ �����ϼ���.\n");
	printf("0 : �ڷ� ����\n\n");
	printf("�Է� : ");
	while (scanf("%d", &selectedDrinkIndex) != 1 || (selectedDrinkIndex < 0 || selectedDrinkIndex > *drinkTypes)) {
		while (getchar() != '\n');
		printf("\n0~%d�� ���ڸ� �Է����ּ���.\n", *drinkTypes);
		printf("�Է� : ");
	}
	while (getchar() != '\n');
	if (selectedDrinkIndex == 0) return;

	for (int i = selectedDrinkIndex; i < *drinkTypes; i++) {
		drinks[selectedDrinkIndex - 1] = drinks[selectedDrinkIndex];
	}

	(*drinkTypes)--;

	writeFile(fp, drinks, *drinkTypes, totalSales, totalRevenue);
	waitEnter("���� ���� �Ϸ�.\nEnter�� ���� ���ư���\n");
}