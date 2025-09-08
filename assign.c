#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* name;
	int price;
	int stock;
} Drink;

void printMachine(int drink, int change, int machineMoney, int userMoney);
int insertMoney(int* userMoney, int* machineMoney);
int selectMenu(int drinkTypes, Drink drinks[], int* choice, int* machineMoney);
int dispenseDrink(Drink drinks[], int choice, int machineMoney, int state);
int returnChange(int* machineMoney, int* userMoney);
int exitProgram();
int askYesNo(int state1, int state2);

int main(void) {
	int userMoney = 5000;	// 사용자 소지 금액
	int machineMoney = 0;	// 자판기 소지 금액
	int floatingMoney = 0;
	int state = 0;		// 자판기 상태
	int mode = 0;	// 접속 모드 
	int choice = 0;		// 사용자 입력값	

	Drink coke = { "콜라", 1000, 10 };
	Drink cider = { "사이다", 1200, 10 };
	Drink water = { "물", 800, 10 };
	Drink powerAde = { "파워에이드", 1500, 10 };
	Drink drinks[] = { coke, cider, water, powerAde };

	int drinkTypes = sizeof(drinks) / sizeof(Drink);

	printf("### 음료 자판기 ###\n\n");
	printMachine(0, 0, machineMoney, userMoney);
	printf("\n--- 모드 입력 ---\n");
	printf("관리자 모드 : 0\n사용자 모드 : 1\n\n");
	printf("입력 : ");
	scanf("%d", &mode);
	while (getchar() != '\n');

	while (1) {
		if (mode == 0) {
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
					state = dispenseDrink(drinks, choice, machineMoney, state);
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



// todo
// 1. 관리자 모드 구현 (파일 입출력) (데이터를 어떻게 기록할 것인가.)
// 2. 프로그램 실행 시 파일로부터 데이터를 읽어서 음료
// 3. 함수에 주소전달하는 거 줄일 수 있는지 확인


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
	int floatingMoney;
	printf("\n--- 현금 투입 ---\n\n");
	printf("투입 금액을 입력해주세요 (0 : 프로그램 종료) : ");
	while (scanf("%d", &floatingMoney) != 1 || (floatingMoney > *userMoney || floatingMoney < 0) || floatingMoney % 10 != 0) {
		while (getchar() != '\n');
		if (floatingMoney % 10 != 0) {
			printf("\n10원 단위로 입력해주세요.\n\n");
		}
		else {
			printf("\n잘못된 값을 입력했습니다.\n");
			printf("현재 소지한 금액에 맞게 입력해주세요.\n\n");
		}
		printf("투입 금액을 입력해주세요 : ");
	}
	while (getchar() != '\n');
	if (floatingMoney == 0) state = exitProgram();
	else {
		*userMoney -= floatingMoney;
		*machineMoney += floatingMoney;
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

int dispenseDrink(Drink drinks[], int choice, int machineMoney, int state) {	
	char answer;
	printf("\n--- 음료 제공 ---\n\n");
	printf("%s 드리겠습니다.\n\nEnter키를 눌러 받아주세요.\n", drinks[choice - 1].name);
	while (getchar() != '\n');
	printf("맛있게 드세요!\n\n");
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