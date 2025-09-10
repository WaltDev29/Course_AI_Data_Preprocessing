#define _CRT_SECURE_NO_WARNINGS
#define FILE_NAME "machine_data.txt"

#include <stdio.h>
#include <stdlib.h>
#include "vending.h"

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
									// 이름 수정
								case 1:
									printf("이름 : ");
									scanf("%s", name);
									printf("\n입력 내용을 확인해주세요.\n");
									printf("입력 내용 : %s\n\n", name);
									break;
									// 가격 수정
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
									// 재고량 수정
								case 3:
									printf("재고량 : ");
									while (scanf("%d", &stock) != 1 || stock < 0) {
										waitEnter("\n잘못된 값을 입력했습니다.\n재고량 : ");
									}
									printf("\n입력 내용을 확인해주세요.\n");
									printf("입력 내용 : %d\n\n", stock);
									break;
								default:
									printf("입력 오류\n");
									return -1;
								}
								// 입력 내용 저장
								waitEnter("입력한 내용을 저장하시겠습니까? (Q : 수정 취소)\nY/N : ");
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
									waitEnter("\n입력한 내용이 저장되었습니다.\n\nEnter를 입력하여 계속.\n");
									writeModeState = 3;
									break;
								}
								else if (yn == 'n' || yn == 'N') {
									printf("\n다시 입력해주세요.\n\n");
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
					waitEnter("\n잘못된 값입니다.\nEnter를 입력하여 다시 입력.\n");
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
					state = dispenseDrink(drinks, choice, machineMoney, userMoney, state);
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
			waitEnter("\n잘못된 값입니다.\nEnter를 입력하여 다시 입력.\n");
		}
	}
	return 0;
}