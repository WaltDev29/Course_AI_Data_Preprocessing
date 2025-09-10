#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vending.h"

// 사용자 모드 관련 함수

// 자판기 출력 함수
void printMachine(int drink, int change, int machineMoney, int userMoney) {
	system("cls");
	printf("### 음료 자판기 ###\n");
	printf("소지 금액 : %5d원\n\n", userMoney);
	printf("-----------------------------------------------------------------------------\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t-------------\t-------------\t-------------\t-------------\t    -\n");
	printf("-\t-    콜라   -\t-   사이다  -\t-     물    -\t- 파워에이드-\t    -\n");
	printf("-\t-           -\t-           -\t-           -\t-           -\t    -\n");
	printf("-\t-   1000원  -\t-   1200원  -\t-   800원   -\t-   1500원  -\t    -\n");
	printf("-\t-           -\t-           -\t-           -\t-           -\t    -\n");
	printf("-\t-  +++++++  -\t-  +++++++  -\t-  +++++++  -\t-  +++++++  -\t    -\n");
	printf("-\t-  +  1  +  -\t-  +  2  +  -\t-  +  3  +  -\t-  +  4  +  -\t    -\n");
	printf("-\t-  +++++++  -\t-  +++++++  -\t-  +++++++  -\t-  +++++++  -\t    -\n");
	printf("-\t-------------\t-------------\t-------------\t-------------\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                  \t 투입 금액 : %5d원\t    -\n", machineMoney);
	printf("-\t                                                 \t\t    -\n");
	printf("-\t           \t           \t           \t                    -\n");
	printf("-\t           \t           \t           \t      -------       -\n");
	if (change == 1) printf("-\t           \t           \t           \t      -  @  -       -\n");
	else printf("-\t           \t           \t           \t      -     -       -\n");
	printf("-\t           \t           \t           \t      -------       -\n");
	printf("-\t           \t           \t           \t                    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t               -------------------------------  \t\t    -\n");
	printf("-\t               -                             -  \t\t    -\n");
	if (drink == 1) printf("-\t               -           ()@@@)            -  \t\t    -\n");
	else printf("-\t               -                             -  \t\t    -\n");
	printf("-\t               -------------------------------  \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-----------------------------------------------------------------------------\n");
}
// 모드 선택
int selectMode() {
	int mode;
	printf("\n--- 모드 입력 ---\n");
	printf("관리자 모드 : 1\n사용자 모드 : 2\n종료 : 0\n\n");
	printf("입력 : ");
	while (scanf("%d", &mode) != 1) {
		waitEnter("\n숫자를 입력해주세요.\n다시 입력 : ");
	}
	while (getchar() != '\n');
	return mode;
}

// 현금 투입
int insertMoney(int* userMoney, int* machineMoney) {
	int state;
	int cash;
	printf("\n--- 현금 투입 ---\n\n");
	printf("투입 금액을 입력해주세요 (0 : 프로그램 종료) : ");
	while (scanf("%d", &cash) != 1 || (cash > *userMoney || cash < 0) || cash % 10 != 0) {
		while (getchar() != '\n');
		if (cash % 10 != 0) {
			printf("\n10원 단위로 입력해주세요.\n\n");
		}
		else {
			printf("\n잘못된 값을 입력했습니다.\n");
			printf("현재 소지한 금액에 맞게 입력해주세요.\n\n");
		}
		printf("투입 금액을 입력해주세요 : ");
	}
	while (getchar() != '\n');
	if (cash == 0) state = exitProgram();
	else {
		*userMoney -= cash;
		*machineMoney += cash;
		state = 1;
	}
	return state;
}
// 메뉴 선택
int selectMenu(int drinkTypes, Drink drinks[], int* choice, int* machineMoney) {
	int state;
	printf("\n--- 메뉴 ---\n\n");
	for (int i = 0; i < drinkTypes; i++) {
		if (drinks[i].stock <= 0) printf("%d. %s : %d원 (매진)\n", i + 1, drinks[i].name, drinks[i].price);
		else printf("%d. %s : %d원\n", i + 1, drinks[i].name, drinks[i].price);
	}

	printf("\n번호를 입력해주세요 (0 : 처음으로) : ");
	while (scanf("%d", choice) != 1 || (*choice < 0 || *choice > drinkTypes)) {
		while (getchar() != '\n');
		printf("1~%d의 숫자를 입력해주세요.\n", drinkTypes);
		printf("\n번호를 입력해주세요 : ");
	}
	while (getchar() != '\n');
	if (*choice != 0 && drinks[*choice - 1].stock <= 0) {
		waitEnter("\n해당 음료는 현재 재고가 없습니다.\nEnter를 입력하여 메뉴로 돌아갑니다.\n");
		return state = 1;
	}
	if (*choice == 0) {
		if (*machineMoney <= 0) state = 0;
		else state = 3;
		return state;
	}

	if (drinks[*choice - 1].price > *machineMoney) {
		char answer;
		printf("\n잔돈이 부족합니다.\n");
		printf("현금을 더 넣으시겠습니까?\n\n");
		state = askYesNo(0, 1);
	}
	else {
		*machineMoney -= drinks[*choice - 1].price;
		state = 2;
	}
	return state;
}
// 음료 제공
int dispenseDrink(Drink drinks[], int choice, int machineMoney, int userMoney, int state) {
	char answer;
	printf("\n--- 음료 제공 ---\n\n");
	printf("%s 드리겠습니다.\n\n", drinks[choice - 1].name);
	waitEnter("Enter키를 눌러 받아주세요.\n");

	printMachine(0, 0, machineMoney, userMoney);
	printf("\n맛있게 드세요!\n\n");
	printf("음료를 더 주문하실 건가요?\n");

	state = askYesNo(1, 3);
	if (state == 1 && machineMoney <= 0) state = 0;
	if (state == 3 && machineMoney <= 0) state = exitProgram();

	return state;
}
// 잔돈 반환
int returnChange(int* machineMoney, int* userMoney) {
	printf("\n--- 잔돈 반환 ---\n\n");
	printf("잔돈 %d원을 드리겠습니다.\n\n", *machineMoney);
	waitEnter("Enter키를 눌러 받아주세요.\n");
	*userMoney += *machineMoney;
	*machineMoney = 0;
	printMachine(0, 0, *machineMoney, *userMoney);
	return exitProgram();
}
// 자판기 종료
int exitProgram() {
	printf("\n자판기 이용을 종료하시겠습니까?\n");
	return askYesNo(4, 0);
}
// Yes or No 입력 받기
int askYesNo(int state1, int state2) {
	int state;
	char answer;
	while (1) {
		printf("Y/N : ");
		scanf("%c", &answer);
		while (getchar() != '\n');
		if (answer == 'y' || answer == 'Y') {
			state = state1;
			break;
		}
		else if (answer == 'n' || answer == 'N') {
			state = state2;
			break;
		}
		else {
			printf("\n잘못된 값을 입력했습니다.\n");
			continue;
		}
	}
	return state;
}