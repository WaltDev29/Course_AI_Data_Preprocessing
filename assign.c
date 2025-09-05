#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* name;
	int price;
} Drink;

void PrintMachine(int drink, int change, int machineMoney, int userMoney);

int main(void) {
	int userMoney = 5000;	// 사용자 소지 금액
	int machineMoney = 0;	// 자판기 소지 금액
	int floatingMoney = 0;
	int state = 0;		// 자판기 상태
	int mode = 0;	// 접속 모드 
	int choice = 0;		// 사용자 입력값
	char answer;	// 사용자 응답		

	Drink coke = { "콜라", 1000 };
	Drink cider = { "사이다", 1200 };
	Drink water = { "물", 800 };
	Drink powerAde = { "파워에이드", 1500 };
	Drink drinks[] = { coke, cider, water, powerAde };

	int drinkTypes = sizeof(drinks) / sizeof(Drink);

	printf("### 음료 자판기 ###\n\n");
	PrintMachine(0, 0, machineMoney, userMoney);
	printf("\n--- 모드 입력 ---\n");
	printf("관리자 모드 : 0\n사용자 모드 : 1\n\n");
	printf("입력 : ");
	scanf("%d", &mode);
	while (getchar() != '\n');

	while (1) {
		if (mode == 0) {

		}
		else if (mode == 1) {
			while (1) {
				switch (state) {
					// 현금 투입
				case 0:
					PrintMachine(0, 0, machineMoney, userMoney);
					printf("\n--- 현금 투입 ---\n\n");
					printf("투입 금액을 입력해주세요 : ");
					while (scanf("%d", &floatingMoney) != 1 || (floatingMoney > userMoney || floatingMoney <= 0)) {
						while (getchar() != '\n');
						if (floatingMoney % 10 != 0) {
							printf("10원 단위로 입력해주세요.\n\n");
						}
						else {
							printf("\n잘못된 값을 입력했습니다.\n");
							printf("현재 소지한 금액에 맞게 입력해주세요.\n\n");
						}
						printf("투입 금액을 입력해주세요 : ");
					}
					while (getchar() != '\n');
					userMoney -= floatingMoney;
					machineMoney += floatingMoney;
					state = 1;
					break;

					// 메뉴 출력
				case 1:
					PrintMachine(0, 0, machineMoney, userMoney);
					printf("\n--- 메뉴 ---\n\n");
					for (int i = 0; i < drinkTypes; i++) {
						printf("%d. %s : %d원\n", i + 1, drinks[i].name, drinks[i].price);
					}

					printf("\n번호를 입력해주세요 : ");
					while (scanf("%d", &choice) != 1 || (choice <= 0 || choice > drinkTypes)) {
						while (getchar() != '\n');
						printf("1~%d의 숫자를 입력해주세요.\n", drinkTypes);
						printf("\n번호를 입력해주세요 : ");
					}
					while (getchar() != '\n');

					if (drinks[choice - 1].price > machineMoney) {
						printf("\n잔돈이 부족합니다.\n");
						printf("현금을 더 넣으시겠습니까?\n\n");
						printf("Y/N : ");
						while (1) {
							scanf("%c", &answer);
							if (answer == 'y' || answer == 'Y') {
								while (getchar() != '\n');
								state = 0;
								break;
							}
							else if (answer == 'n' || answer == 'N') {
								while (getchar() != '\n');
								state = 1;
								break;
							}
							else {
								while (getchar() != '\n');
								printf("\n잘못된 값을 입력했습니다.\n");
								printf("Y/N : ");
								continue;
							}
						}
						break;
					}
					else machineMoney -= drinks[choice - 1].price;

					state = 2;
					break;

					// 음료 제공
				case 2:
					PrintMachine(1, 0, machineMoney, userMoney);
					printf("\n--- 음료 제공 ---\n\n");
					printf("%s 드리겠습니다.\n\nEnter키를 눌러 받아주세요.\n", drinks[choice - 1].name);
					while (getchar() != '\n');					
					printf("맛있게 드세요!\n\n");
					printf("음료를 더 주문하실 건가요?\n");
					printf("Y/N : ");
					while (1) {
						scanf("%c", &answer);
						if (answer == 'y' || answer == 'Y') {
							while (getchar() != '\n');
							state = 1;
							break;
						}
						else if (answer == 'n' || answer == 'N') {
							while (getchar() != '\n');
							if (machineMoney > 0) state = 3;
							else state = 0;
							break;
						}
						else {
							while (getchar() != '\n');
							printf("\n잘못된 값을 입력했습니다.\n");
							printf("Y/N : ");
							continue;
						}
					}
					break;

					// 잔돈 반환
				case 3:
					PrintMachine(0, 1, machineMoney, userMoney);
					printf("\n--- 잔돈 반환 ---\n\n");
					printf("잔돈 %d원을 드리겠습니다.\n\nEnter키를 눌러 받아주세요.", machineMoney);
					while (getchar() != '\n');
					userMoney += machineMoney;
					machineMoney = 0;
					state = 0;
					break;

				default:
					printf("프로그램 오류. 프로그램을 종료합니다.");
					return -1;
				}
			}
		}

		else {
			printf("잘못된 값입니다. 프로그램을 종료합니다.");
			return -1;
		}
	}	
	return 0;
}



// todo
// 3. 이전 단계로 돌아갈 수 있도록
// 4. 각 단계 함수화


// 자판기 출력 함수
void PrintMachine(int drink, int change, int machineMoney, int userMoney) {
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