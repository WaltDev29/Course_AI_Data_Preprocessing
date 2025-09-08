#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char name[50];
	int price;
	int stock;
} Drink;

void printMachine(int drink, int change, int machineMoney, int userMoney);
int insertMoney(int* userMoney, int* machineMoney);
int selectMenu(int drinkTypes, Drink drinks[], int* choice, int* machineMoney);
int dispenseDrink(Drink drinks[], int choice, int machineMoney, int userMoney, int state);
int returnChange(int* machineMoney, int* userMoney);
int exitProgram();
int askYesNo(int state1, int state2);

int main(void) {
	char data[100];

	int userMoney = 5000;	// 사용자 소지 금액
	int machineMoney = 0;	// 자판기 소지 금액	
	int state = 0;		// 자판기 상태
	int mode = 0;	// 접속 모드 
	int choice = 0;		// 사용자 입력값	

	// 파일 읽기 용 변수
	char name[100];
	int price;
	int stock;
	int num;

	FILE* fp = fopen("machine_data.txt", "a+t");
	if (fp == NULL) {
		printf("데이터 불러오기 실패\n");
		printf("프로그램을 종료합니다.\n");
		return -1;
	}
		
	fseek(fp, 0, SEEK_SET);

	fscanf(fp, "%d", &num);	
	Drink* drinks = (Drink*)malloc(num * sizeof(Drink));	
	for (int i=0; i< num; i++) {
		fscanf(fp, "%s %d %d", name, &price, &stock);
		strcpy(drinks[i].name, name);
		drinks[i].price = price;
		drinks[i].stock = stock;		
	}
	for (int i = 0; i < num; i++) {
		printf("%s %d %d\n", drinks[i].name, drinks[i].price, drinks[i].stock);
	}

	/*fclose(fp);
	return 0;*/


	

	int drinkTypes = num;

	printf("### 음료 자판기 ###\n\n");
	printMachine(0, 0, machineMoney, userMoney);
	printf("\n--- 모드 입력 ---\n");
	printf("관리자 모드 : 0\n사용자 모드 : 1\n\n");
	printf("입력 : ");
	scanf("%d", &mode);
	while (getchar() != '\n');

	while (1) {
		if (mode == 0) {
			int answer;	// 관리자 입력						
			while (1) {
				system("cls");
				printf("--- 관리자 모드 ---\n\n");

				printf("읽기 모드 : 0\n쓰기 모드 : 1\n종료 : -1\n\n");
				printf("입력 : ");
				scanf("%d", &answer);
				getchar();

				fseek(fp, 0, SEEK_SET);
				if (answer == 0) {
					while (fgets(data, sizeof(data), fp) != NULL) {
						printf("%s", data);
					}
					printf("\n\nEnter를 눌러 처음으로 돌아갑니다.");
					while (getchar() != '\n');
				}

				else if (answer == 1) {
					printf("\n데이터를 입력하세요 (EXIT 입력 시 입력 종료) : \n");
					while (1) {
						fgets(data, sizeof(data), stdin);
						scanf("%s", data);
						getchar();
						if (strcmp(data, "EXIT") == 0) break;
						fprintf(fp, data);
					}
				}

				else break;
			}
			printf("프로그램을 종료합니다.\n");
			fclose(fp);
			return 0;

			// 관리자 모드
			// 파일 입출력 사용 (파일 조회, 수정)			
			// # 파일 내용(추가 제외 가능)
			//	 - 음료 이름, 가격, 재고량, 판매량, (판매수익?)
			//	 - 총 판매량, 총 수익
			//	 - (음료 구매 기록)
		}
		else if (mode == 1) {
			while (1) {
				switch (state) {
					// 현금 투입
				case 0:
					printMachine(0, 0, machineMoney, userMoney);
					state = insertMoney(&userMoney, &machineMoney);
					break;

					// 메뉴 출력
				case 1:
					printMachine(0, 0, machineMoney, userMoney);
					state = selectMenu(drinkTypes, drinks, &choice, &machineMoney);
					break;

					// 음료 제공
				case 2:
					printMachine(1, 0, machineMoney, userMoney);
					state = dispenseDrink(drinks, choice, machineMoney, userMoney, state);
					break;

					// 잔돈 반환
				case 3:
					printMachine(0, 1, machineMoney, userMoney);
					state = returnChange(&machineMoney, &userMoney);
					break;

					// 프로그램 종료
				case 4:
					printf("\n\n프로그램을 종료합니다.");
					return 0;
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


// 지금 하고 있는 것.
// 음료 리스트를 미리 작성해놓았음.
// 음료 리스트를 읽어서 동적으로 drinks 리스트를 만들도록 함.

// todo
// 0. 데이터 파일의 포맷을 어떻게 할 것인가.
//		(총 재고량 : 10 이런 식으로 작성하고 싶은데, 읽을 때는 어떻게 읽음?)
// 1. 관리자 모드 구현 (파일 입출력) (데이터를 어떻게 기록/수정할 것인가.)
// 2. UI도 음료수 갯수에 맞춰 동적으로 생성해볼까?


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

int selectMenu(int drinkTypes, Drink drinks[], int* choice, int* machineMoney) {
	int state;
	printf("\n--- 메뉴 ---\n\n");
	for (int i = 0; i < drinkTypes; i++) {
		printf("%d. %s : %d원\n", i + 1, drinks[i].name, drinks[i].price);
	}

	printf("\n번호를 입력해주세요 (0 : 처음으로) : ");
	while (scanf("%d", choice) != 1 || (*choice < 0 || *choice > drinkTypes)) {
		while (getchar() != '\n');
		printf("1~%d의 숫자를 입력해주세요.\n", drinkTypes);
		printf("\n번호를 입력해주세요 : ");
	}
	while (getchar() != '\n');
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

int dispenseDrink(Drink drinks[], int choice, int machineMoney, int userMoney, int state) {
	char answer;
	printf("\n--- 음료 제공 ---\n\n");
	printf("%s 드리겠습니다.\n\nEnter키를 눌러 받아주세요.\n", drinks[choice - 1].name);
	while (getchar() != '\n');
	printMachine(0, 0, machineMoney, userMoney);
	printf("\n맛있게 드세요!\n\n");
	printf("음료를 더 주문하실 건가요?\n");

	state = askYesNo(1, 3);
	if (state == 1 && machineMoney <= 0) state = 0;
	if (state == 3 && machineMoney <= 0) state = exitProgram();


	return state;
}

int returnChange(int* machineMoney, int* userMoney) {
	printf("\n--- 잔돈 반환 ---\n\n");
	printf("잔돈 %d원을 드리겠습니다.\n\nEnter키를 눌러 받아주세요.", *machineMoney);
	while (getchar() != '\n');
	*userMoney += *machineMoney;
	*machineMoney = 0;
	printMachine(0, 0, *machineMoney, *userMoney);
	return exitProgram();
}

int exitProgram() {
	printf("\n자판기 이용을 종료하시겠습니까?\n");
	return askYesNo(4, 0);
}

int askYesNo(int state1, int state2) {
	int state;
	char answer;
	while (1) {
		printf("Y/N : ");
		scanf("%c", &answer);
		if (answer == 'y' || answer == 'Y') {
			while (getchar() != '\n');
			state = state1;
			break;
		}
		else if (answer == 'n' || answer == 'N') {
			while (getchar() != '\n');
			state = state2;
			break;
		}
		else {
			while (getchar() != '\n');
			printf("\n잘못된 값을 입력했습니다.\n");
			continue;
		}
	}
	return state;
}