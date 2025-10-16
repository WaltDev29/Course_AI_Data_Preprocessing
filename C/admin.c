#define _CRT_SECURE_NO_WARNINGS
#define FILE_NAME "machine_data.txt"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vending.h"

// 관리자 모드 관련 함수

// 비밀번호 확인
int checkPassword() {
	char password[5] = "1234";
	char inputPassword[5];
	printf("비밀번호를 입력하세요 : ");
	if (scanf_s("%s", inputPassword, sizeof(inputPassword)) != 1 || strcmp(password, inputPassword) != 0) {
		while (getchar() != '\n');
		waitEnter("\n비밀번호가 틀렸습니다.\nEnter를 입력하여 돌아가기.\n");
		return 0;
	}
	else return 1;
}
// 관리자 모드 첫 화면 출력
int showAdminScreen() {
	int answer;
	system("cls");
	printf("--- 관리자 모드 ---\n\n");
	printf("읽기 모드 : 1\n쓰기 모드 : 2\n판매 기록 열람 : 3\n종료 : 0\n\n");
	printf("입력 : ");
	scanf("%d", &answer);
	getchar();
	return answer;
}
// 파일 내용 출력
void printFile(FILE* fp) {
	char tmp[100]; // 파일 내용 담을 버퍼
	system("cls");
	while (fgets(tmp, sizeof(tmp), fp) != NULL) {
		printf("%s", tmp);
	}
	waitEnter("\n\nEnter키를 눌러 돌아가기.\n");
}
// 파일 새로 쓰기
FILE* writeFile(FILE* fp, Drink drinks[], int drinkTypes, int totalSales, int totalRevenue) {
	FILE* temp = fopen("temp.txt", "w+t");
	checkFileOpen(temp);
	fprintf(temp, "음료 종류 : %d\n", drinkTypes);
	fprintf(temp, "총 판매량 : %d\n", totalSales);
	fprintf(temp, "총 수익 : %d\n\n", totalRevenue);
	fprintf(temp, "---음료 목록---\n");
	fprintf(temp, "%-20s %-10s %-10s\n", "이름", "가격", "재고량");
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
// 판매 로그 기록
void logSales(Drink drinks[], int choice, int totalSales) {
	FILE* fp = fopen("salesLog.txt", "a+t");
	checkFileOpen(fp);

	time_t t = time(NULL);
	struct tm* tm_info = localtime(&t);
	char buffer[100];

	fprintf(fp, "%d.\n", totalSales);
	strftime(buffer, sizeof(buffer), "%Y.%m.%d %H:%M", tm_info);
	fprintf(fp, "%s\n", buffer);
	fprintf(fp, "구매 항목 : %s\n", drinks[choice - 1].name);
	fprintf(fp, "수익 : %d원\n\n", drinks[choice - 1].price);

	fclose(fp);
}
// 수정할 음료 선택
int selectDrinkToEdit(Drink drinks[], int drinkTypes) {
	int selectedDrinkIndex;
	// 음료 목록 출력
	printf("---음료 목록---\n\n");
	printf("   %-17s %-10s %-10s\n", "이름", "가격", "재고량");
	printf("-----------------------------------------------\n");
	for (int i = 0; i < drinkTypes; i++) {
		printf("%d. %-17s %-10d %-10d\n", i + 1, drinks[i].name, drinks[i].price, drinks[i].stock);
	}

	// 수정할 음료 선택
	printf("\n수정할 항목을 선택하세요.\n");
	printf("-1 : 음료 추가\n-2 : 음료 삭제\n");
	printf("0 : 뒤로 가기\n\n");
	printf("입력 : ");
	while (scanf("%d", &selectedDrinkIndex) != 1 || (selectedDrinkIndex < -2 || selectedDrinkIndex > drinkTypes)) {
		while (getchar() != '\n');
		printf("\n-2~%d의 숫자를 입력해주세요.\n", drinkTypes);
		printf("입력 : ");
	}

	return selectedDrinkIndex;
}
// 수정할 항목 선택
int selectFieldToEdit(Drink drinks[], int selectedDrinkIndex) {
	int editField;
	system("cls");
	printf("1. 이름 : %s\n2. 가격 : %d\n3. 재고량 : %d\n\n", drinks[selectedDrinkIndex - 1].name, drinks[selectedDrinkIndex - 1].price, drinks[selectedDrinkIndex - 1].stock);
	printf("수정할 항목의 번호를 선택하세요.\n");
	printf("0 : 뒤로 가기\n\n");
	printf("입력 : ");
	while (scanf("%d", &editField) != 1 || (editField < 0 || editField > 3)) {
		waitEnter("\n0~3의 숫자를 입력해주세요.\n입력 : ");
	}

	return editField;
}
// 음료 추가
void addDrinkList(FILE* fp, Drink** drinks, int* drinkTypes, int totalSales, int totalRevenue) {
	char name[50];
	char yn;
	int price, stock;
	while (1) {
		system("cls");
		printf("추가할 음료의 정보를 적어주세요.\n\n");
		printf("이름 : ");
		scanf("%s", name);
		printf("가격 : ");
		while (scanf("%d", &price) != 1 || price < 0 || price % 10 != 0) {
			while (getchar() != '\n');
			if (price % 10 != 0) {
				printf("\n10원 단위로 입력해주세요.\n\n");
			}
			else {
				printf("\n잘못된 값을 입력했습니다.\n");
			}
			printf("가격 : ");
		}
		printf("재고량 : ");
		while (scanf("%d", &stock) != 1 || stock < 0) {
			waitEnter("\n잘못된 값을 입력했습니다.\n재고량 : ");
		}
		while (getchar() != '\n');
		system("cls");
		printf("입력한 내용을 확인해주세요\n");
		printf("이름 : %s\n가격 : %d\n재고량 : %d\n", name, price, stock);
		printf("\n입력 내용을 저장하시겠습니까? (Q : 처음으로)\n");
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
			waitEnter("\n음료 추가 완료.\nEnter를 눌러 돌아가기\n");
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
// 음료 삭제
void deleteDrinkList(FILE* fp, Drink drinks[], int* drinkTypes, int totalSales, int totalRevenue) {
	int selectedDrinkIndex;
	// 음료 목록 출력
	system("cls");
	printf("---음료 목록---\n\n");
	printf("   %-17s %-10s %-10s\n", "이름", "가격", "재고량");
	printf("-----------------------------------------------\n");
	for (int i = 0; i < *drinkTypes; i++) {
		printf("%d. %-17s %-10d %-10d\n", i + 1, drinks[i].name, drinks[i].price, drinks[i].stock);
	}

	// 삭제할 음료 선택
	printf("\n삭제할 음료를 선택하세요.\n");
	printf("0 : 뒤로 가기\n\n");
	printf("입력 : ");
	while (scanf("%d", &selectedDrinkIndex) != 1 || (selectedDrinkIndex < 0 || selectedDrinkIndex > *drinkTypes)) {
		while (getchar() != '\n');
		printf("\n0~%d의 숫자를 입력해주세요.\n", *drinkTypes);
		printf("입력 : ");
	}
	while (getchar() != '\n');
	if (selectedDrinkIndex == 0) return;

	for (int i = selectedDrinkIndex; i < *drinkTypes; i++) {
		drinks[selectedDrinkIndex - 1] = drinks[selectedDrinkIndex];
	}

	(*drinkTypes)--;

	writeFile(fp, drinks, *drinkTypes, totalSales, totalRevenue);
	waitEnter("음료 삭제 완료.\nEnter를 눌러 돌아가기\n");
}