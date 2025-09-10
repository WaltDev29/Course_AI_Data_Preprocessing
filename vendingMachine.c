#define _CRT_SECURE_NO_WARNINGS
#define FILE_NAME "machine_data.txt"

#include <stdio.h>
#include <stdlib.h>
#include "vending.h"

int main(void) {
	// ��ȣ�ۿ� ���� ����
	int userMoney = 5000;	// ����� ���� �ݾ�
	int machineMoney = 0;	// ���Ǳ� ���� �ݾ�	
	int mode = 0;	// ���� ��� 
	int state = 0;		// ���Ǳ� ����
	int choice = 0;		// ����� �Է°�	

	// ���� �б�� ����
	int drinkTypes;	// ���� ���� ���� ��
	int totalSales;	// �� �Ǹŷ�
	int totalRevenue;	// �� ����	

	// ���� ����
	FILE* fp = fopen("test.txt", "a+t");
	checkFileOpen(fp);
	// Ŀ�� ��ġ ó������
	fseek(fp, 0, SEEK_SET);

	if (fgetc(fp) == EOF) {
		printf("������ ����ֽ��ϴ�.");
		fprintf(fp, "���� ���� : 4\n�� �Ǹŷ� : 0\n�� ���� : 0\n\n");
		fprintf(fp, "---���� ���---\n");
		fprintf(fp, "%-20s %-10s %-10s\n", "�̸�", "����", "���");
		fprintf(fp, "-----------------------------------------------\n");
		fprintf(fp, "%-20s %-10d %-10d\n", "�ݶ�", 1000, 1);
		fprintf(fp, "%-20s %-10d %-10d\n", "���̴�", 1200, 10);
		fprintf(fp, "%-20s %-10d %-10d\n", "��", 800, 10);
		fprintf(fp, "%-20s %-10d %-10d\n", "�Ŀ����̵�", 1500, 10);
	}
	fseek(fp, 0, SEEK_SET);

	// ���� ���� �о� ����
	while (fgetc(fp) != ':');
	fscanf(fp, "%d", &drinkTypes);
	while (fgetc(fp) != ':');
	fscanf(fp, "%d", &totalSales);
	while (fgetc(fp) != ':');
	fscanf(fp, "%d", &totalRevenue);


	// ���� ��� ���� ���� �� ���ʿ��� ���� �ǳʶٱ�
	for (int i = 0; i < 5; i++) {
		char tmp[100];
		fgets(tmp, sizeof(tmp), fp);
	}

	// ����� ��� ���� �ͼ� drinks �迭 ����
	Drink* drinks = (Drink*)malloc(drinkTypes * sizeof(Drink));
	for (int i = 0; i < drinkTypes; i++) {
		fscanf(fp, "%s %d %d", drinks[i].name, &drinks[i].price, &drinks[i].stock);
	}


	while (1) {
		// �ʱ� ȭ��
		printMachine(0, 0, machineMoney, userMoney);
		mode = selectMode();

		// ������ ���
		if (mode == 1) {
			// ��й�ȣ �˻�		
			if (!checkPassword()) continue;

			state = 0;
			int selectedDrinkIndex;	// ������ ���� ��ȣ			
			int editField; // ������ �׸�
			int writeModeState = 0;	// ���� ��� �ܰ�
			while (1) {
				// ������ ��� ù ȭ�� ���
				state = showAdminScreen();

				fseek(fp, 0, SEEK_SET);

				// ����
				if (state == 0) break;

				// �б� ���
				else if (state == 1) {
					printFile(fp);
				}

				// ���� ���
				else if (state == 2) {
					writeModeState = 0;
					while (writeModeState != 3) {
						system("cls");
						switch (writeModeState) {
							// ���� ��� ��� �� ����
						case 0:
							selectedDrinkIndex = selectDrinkToEdit(drinks, drinkTypes);
							// ���� ���� �߰�
							if (selectedDrinkIndex == -1) {
								addDrinkList(fp, &drinks, &drinkTypes, totalSales, totalRevenue);
								writeModeState = 3;
							}
							// ���� ���� ����
							else if (selectedDrinkIndex == -2) {
								deleteDrinkList(fp, drinks, &drinkTypes, totalSales, totalRevenue);
								writeModeState = 3;
							}
							// �ڷ� ����
							else if (selectedDrinkIndex == 0) writeModeState = 3;
							else writeModeState = 1;
							break;

							// ������ ���� ����					
						case 1:
							editField = selectFieldToEdit(drinks, selectedDrinkIndex);
							if (editField == 0) writeModeState = 0;
							else writeModeState = 2;
							break;

							// ���� ���� ����
						case 2:
							while (1) {
								printf("\n������ ������ �Է����ּ���.\n\n");
								char name[50];
								int price;
								int stock;
								char yn;
								switch (editField) {
									// �̸� ����
								case 1:
									printf("�̸� : ");
									scanf("%s", name);
									printf("\n�Է� ������ Ȯ�����ּ���.\n");
									printf("�Է� ���� : %s\n\n", name);
									break;
									// ���� ����
								case 2:
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
									printf("\n�Է� ������ Ȯ�����ּ���.\n");
									printf("�Է� ���� : %d\n\n", price);
									break;
									// ��� ����
								case 3:
									printf("��� : ");
									while (scanf("%d", &stock) != 1 || stock < 0) {
										waitEnter("\n�߸��� ���� �Է��߽��ϴ�.\n��� : ");
									}
									printf("\n�Է� ������ Ȯ�����ּ���.\n");
									printf("�Է� ���� : %d\n\n", stock);
									break;
								default:
									printf("�Է� ����\n");
									return -1;
								}
								// �Է� ���� ����
								waitEnter("�Է��� ������ �����Ͻðڽ��ϱ�? (Q : ���� ���)\nY/N : ");
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
									waitEnter("\n�Է��� ������ ����Ǿ����ϴ�.\n\nEnter�� �Է��Ͽ� ���.\n");
									writeModeState = 3;
									break;
								}
								else if (yn == 'n' || yn == 'N') {
									printf("\n�ٽ� �Է����ּ���.\n\n");
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

				// �Ǹ� ��� ����
				else if (state == 3) {
					FILE* log = fopen("salesLog.txt", "rt");
					checkFileOpen(log);
					printFile(log);
					fclose(log);
				}

				else {
					waitEnter("\n�߸��� ���Դϴ�.\nEnter�� �Է��Ͽ� �ٽ� �Է�.\n");
					continue;
				}
			}
			printf("\n���α׷��� �����մϴ�.\n");
			fclose(fp);
			return 0;

			// ������ ���
			// ���� ����� ��� (���� ��ȸ, ����)			
			// # ���� ����(�߰� ���� ����)
			//	 - ���� �̸�, ����, ���, �Ǹŷ�, (�Ǹż���?)
			//	 - �� �Ǹŷ�, �� ����
			//	 - (���� ���� ���)
		}


		// ����� ���
		else if (mode == 2) {
			state = 0;
			while (1) {
				switch (state) {
					// ���� ����
				case 0:
					printMachine(0, 0, machineMoney, userMoney);
					state = insertMoney(&userMoney, &machineMoney);
					break;

					// �޴� ���
				case 1:
					printMachine(0, 0, machineMoney, userMoney);
					state = selectMenu(drinkTypes, drinks, &choice, &machineMoney);
					break;

					// ���� ����
				case 2:
					printMachine(1, 0, machineMoney, userMoney);
					state = dispenseDrink(drinks, choice, machineMoney, userMoney, state);
					// ���Ͽ� ���
					totalSales++;
					totalRevenue += drinks[choice - 1].price;
					drinks[choice - 1].stock--;
					writeFile(fp, drinks, drinkTypes, totalSales, totalRevenue);
					logSales(drinks, choice, totalSales);
					break;

					// �ܵ� ��ȯ
				case 3:
					printMachine(0, 1, machineMoney, userMoney);
					state = returnChange(&machineMoney, &userMoney);
					break;

					// ���α׷� ����
				case 4:
					printf("\n\n���α׷��� �����մϴ�.");
					fclose(fp);
					return 0;
					break;

				default:
					printf("���α׷� ����. ���α׷��� �����մϴ�.");
					fclose(fp);
					return -1;
				}
			}
		}

		else if (mode == 0) {
			printf("\n\n���α׷��� �����մϴ�.");
			fclose(fp);
			return 0;
		}

		else {
			waitEnter("\n�߸��� ���Դϴ�.\nEnter�� �Է��Ͽ� �ٽ� �Է�.\n");
		}
	}
	return 0;
}