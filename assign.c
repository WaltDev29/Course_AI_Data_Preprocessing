#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NAME "machine_data.txt"


// 구조체 선언
typedef struct {
	char name[50];
	int price;
	int stock;
} Drink;


// 함수 선언
void checkFileOpen(FILE* fp);
int selectMode();
void waitEnter(char* str);
// 사용자 모드 관련
void printMachine(int drink, int change, int machineMoney, int userMoney);
int insertMoney(int* userMoney, int* machineMoney);
int selectMenu(int drinkTypes, Drink drinks[], int* choice, int* machineMoney);
int dispenseDrink(Drink drinks[], int choice, int machineMoney, int userMoney, int state, int* totalSales, int* totalRevenue);
int returnChange(int* machineMoney, int* userMoney);
int exitProgram();
int askYesNo(int state1, int state2);
// 관리자 모드 관련
int checkPassword();
int showAdminScreen();
void printFile(FILE* fp);
FILE* writeFile(FILE* fp, Drink drinks[], int drinkTypes, int totalSales, int totalRevenue);
void logSales(Drink drinks[], int choice, int totalSales);
int selectDrinkToEdit(Drink drinks[], int drinkTypes);
int selectFieldToEdit(Drink drinks[], int selectedDrinkIndex);
void addDrinkList(FILE* fp, Drink** drinks, int* drinkTypes, int totalSales, int totalRevenue);
void deleteDrinkList(FILE* fp, Drink drinks[], int* drinkTypes, int totalSales, int totalRevenue);

int main(void) {
	// 상호작용 관련 변수
	int userMoney = 5000;	// 사용자 소지 금액
	int machineMoney = 0;	// 자판기 소지 금액	
	int mode = 0;	// 접속 모드 
	int state = 0;		// 자판기 상태
	int choice = 0;		// 사용자 입력값	

	// 파일 읽기용 변수
	int drinkTypes;	// 음료 종류 가지 수
	int totalSales;	// 총 판매량
	int totalRevenue;	// 총 수익	

	// 파일 열기
	FILE* fp = fopen("test.txt", "a+t");
	checkFileOpen(fp);
	// 커서 위치 처음으로
	fseek(fp, 0, SEEK_SET);

	if (fgetc(fp) == EOF) {
		printf("파일이 비어있습니다.");
		fprintf(fp, "음료 종류 : 4\n총 판매량 : 0\n총 수익 : 0\n\n");
		fprintf(fp, "---음료 목록---\n");
		fprintf(fp, "%-20s %-10s %-10s\n", "이름", "가격", "재고량");
		fprintf(fp, "-----------------------------------------------\n");
		fprintf(fp, "%-20s %-10d %-10d\n", "콜라", 1000, 1);
		fprintf(fp, "%-20s %-10d %-10d\n", "사이다", 1200, 10);
		fprintf(fp, "%-20s %-10d %-10d\n", "물", 800, 10);
		fprintf(fp, "%-20s %-10d %-10d\n", "파워에이드", 1500, 10);						
	}
	fseek(fp, 0, SEEK_SET);

	// 음료 종류 읽어 오기
	while (fgetc(fp) != ':');
	fscanf(fp, "%d", &drinkTypes);
	while (fgetc(fp) != ':');
	fscanf(fp, "%d", &totalSales);
	while (fgetc(fp) != ':');
	fscanf(fp, "%d", &totalRevenue);


	// 음료 목록 갖고 오기 전 불필요한 내용 건너뛰기
	for (int i = 0; i < 5; i++) {
		char tmp[100];
		fgets(tmp, sizeof(tmp), fp);
	}

	// 음료수 목록 갖고 와서 drinks 배열 생성
	Drink* drinks = (Drink*)malloc(drinkTypes * sizeof(Drink));
	for (int i = 0; i < drinkTypes; i++) {
		fscanf(fp, "%s %d %d", drinks[i].name, &drinks[i].price, &drinks[i].stock);
	}


	while (1) {
		// 초기 화면
		printMachine(0, 0, machineMoney, userMoney);
		mode = selectMode();

		// 관리자 모드
		if (mode == 1) {
			// 비밀번호 검사		
			if (!checkPassword()) continue;

			state = 0;
			int selectedDrinkIndex;	// 수정할 음료 번호			
			int editField; // 수정할 항목
			int writeModeState = 0;	// 쓰기 모드 단계
			while (1) {
				// 관리자 모드 첫 화면 출력
				state = showAdminScreen();

				fseek(fp, 0, SEEK_SET);

				// 종료
				if (state == 0) break;

				// 읽기 모드
				else if (state == 1) {
					printFile(fp);
				}

				// 쓰기 모드
				else if (state == 2) {
					writeModeState = 0;
					while (writeModeState != 3) {
						system("cls");
						switch (writeModeState) {
							// 음료 목록 출력 및 선택
						case 0:
							selectedDrinkIndex = selectDrinkToEdit(drinks, drinkTypes);
							// 음료 종류 추가
							if (selectedDrinkIndex == -1) {
								addDrinkList(fp, &drinks, &drinkTypes, totalSales, totalRevenue);
								writeModeState = 3;
							}
							// 음료 종류 삭제
							else if (selectedDrinkIndex == -2) {
								deleteDrinkList(fp, drinks, &drinkTypes, totalSales, totalRevenue);
								writeModeState = 3;
							}
							// 뒤로 가기
							else if (selectedDrinkIndex == 0) writeModeState = 3;
							else writeModeState = 1;
							break;

							// 수정할 내용 선택					
						case 1:
							editField = selectFieldToEdit(drinks, selectedDrinkIndex);
							if (editField == 0) writeModeState = 0;
							else writeModeState = 2;
							break;

							// 선택 사항 수정
						case 2:
							while (1) {
								printf("\n수정할 내용을 입력해주세요.\n\n");
								char name[50];
								int price;
								int stock;
								char yn;
								switch (editField) {
								case 1:
									printf("이름 : ");
									scanf("%s", name);
									printf("\n입력 내용을 확인해주세요.\n");
									printf("입력 내용 : %s\n\n", name);
									break;
								case 2:
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
									printf("\n입력 내용을 확인해주세요.\n");
									printf("입력 내용 : %d\n\n", price);
									break;
								case 3:
									printf("재고량 : ");
									while (scanf("%d", &stock) != 1 || stock < 0) {
										while (getchar() != '\n');
										printf("\n잘못된 값을 입력했습니다.\n");
										printf("재고량 : ");
									}
									printf("\n입력 내용을 확인해주세요.\n");
									printf("입력 내용 : %d\n\n", stock);
									break;
								default:
									printf("입력 오류\n");
									return -1;
								}
								while (getchar() != '\n');
								printf("입력한 내용을 저장하시겠습니까? (Q : 수정 취소)\n");
								printf("Y/N : ");
								scanf("%c", &yn);
								while (getchar() != '\n');
								if (yn == 'y' || yn == 'Y') {
									switch (editField) {
									case 1:
										strcpy(drinks[selectedDrinkIndex - 1].name, name);
										break;
									case 2:
										drinks[selectedDrinkIndex - 1].price = price;
										break;
									case 3:
										drinks[selectedDrinkIndex - 1].stock = stock;
										break;
									}
									fp = writeFile(fp, drinks, drinkTypes, totalSales, totalRevenue);									
									waitEnter("\n입력한 내용이 저장되었습니다.\n\nEnter를 입력하여 계속.");									
									writeModeState = 3;
									break;
								}
								else if (yn == 'n' || yn == 'N') {
									printf("다시 입력해주세요.\n\n");
								}
								else if (yn == 'q' || yn == 'Q') {
									writeModeState = 3;
									break;
								}
							}
							break;
						case 3:
							break;
						default:
							break;
						}
					}
				}

				// 판매 기록 열람
				else if (state == 3) {
					FILE* log = fopen("salesLog.txt", "rt");
					checkFileOpen(log);
					printFile(log);
					fclose(log);
				}

				else {					
					waitEnter("\n잘못된 값입니다.\nEnter를 입력하여 다시 입력.");
					continue;
				}
			}
			printf("\n프로그램을 종료합니다.\n");
			fclose(fp);
			return 0;

			// 관리자 모드
			// 파일 입출력 사용 (파일 조회, 수정)			
			// # 파일 내용(추가 제외 가능)
			//	 - 음료 이름, 가격, 재고량, 판매량, (판매수익?)
			//	 - 총 판매량, 총 수익
			//	 - (음료 구매 기록)
		}


		// 사용자 모드
		else if (mode == 2) {
			state = 0;
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
					state = dispenseDrink(drinks, choice, machineMoney, userMoney, state, &totalSales, &totalRevenue);
					// 파일에 기록
					totalSales++;
					totalRevenue += drinks[choice - 1].price;
					drinks[choice - 1].stock--;
					writeFile(fp, drinks, drinkTypes, totalSales, totalRevenue);
					logSales(drinks, choice, totalSales);
					break;

					// 잔돈 반환
				case 3:
					printMachine(0, 1, machineMoney, userMoney);
					state = returnChange(&machineMoney, &userMoney);
					break;

					// 프로그램 종료
				case 4:
					printf("\n\n프로그램을 종료합니다.");
					fclose(fp);
					return 0;
					break;

				default:
					printf("프로그램 오류. 프로그램을 종료합니다.");
					fclose(fp);
					return -1;
				}
			}
		}

		else if (mode == 0) {
			printf("\n\n프로그램을 종료합니다.");
			fclose(fp);
			return 0;
		}

		else {			
			waitEnter("\n잘못된 값입니다.\nEnter를 입력하여 다시 입력.");
		}
	}
	return 0;
}


// 지금 하고 있는 것.

// todo
// 1. 관리자 모드 함수화


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

int selectMode() {
	int mode;
	printf("\n--- 모드 입력 ---\n");
	printf("관리자 모드 : 1\n사용자 모드 : 2\n종료 : 0\n\n");
	printf("입력 : ");
	while (scanf("%d", &mode) != 1) {
		while (getchar() != '\n');
		printf("\n숫자를 입력해주세요.\n");
		printf("다시 입력 : ");
	}
	while (getchar() != '\n');
	return mode;
}

void waitEnter(char* str) {
	printf("%s\n", str);
	while (getchar() != '\n');
}

void checkFileOpen(FILE* fp) {
	if (fp == NULL) {
		printf("파일 불러오기 실패\n");
		printf("프로그램을 종료합니다.\n");
		exit(-1);
	}
}

// 사용자 모드 관련 함수

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
		printf("\n해당 음료는 현재 재고가 없습니다.\n");
		waitEnter("Enter를 입력하여 메뉴로 돌아갑니다.");				
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
int dispenseDrink(Drink drinks[], int choice, int machineMoney, int userMoney, int state, int* totalSales, int* totalRevenue) {
	char answer;
	printf("\n--- 음료 제공 ---\n\n");
	printf("%s 드리겠습니다.\n\n", drinks[choice - 1].name);
	waitEnter("Enter키를 눌러 받아주세요.");	

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
	waitEnter("Enter키를 눌러 받아주세요.");
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


// 관리자 모드 관련 함수

// 비밀번호 확인
int checkPassword() {
	char password[5] = "1234";
	char inputPassword[5];
	printf("비밀번호를 입력하세요 : ");
	if (scanf_s("%s", inputPassword, sizeof(inputPassword)) != 1 || strcmp(password, inputPassword) != 0) {
		while (getchar() != '\n');		
		waitEnter("\n비밀번호가 틀렸습니다.\nEnter를 입력하여 돌아가기.");		
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
	waitEnter("\n\nEnter키를 눌러 돌아가기.");
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
		while (getchar() != '\n');
		printf("\n0~3의 숫자를 입력해주세요.\n");
		printf("입력 : ");
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
		scanf("%d", &price);
		printf("재고량 : ");
		scanf("%d", &stock);
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
			waitEnter("\n음료 추가 완료.\nEnter를 눌러 돌아가기");
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
	waitEnter("음료 삭제 완료.\nEnter를 눌러 돌아가기");
}