#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "test.txt"

// ����ü ����
typedef struct {
	char name[50];
	int price;
	int stock;
} Drink;


// �Լ� ����
void printMachine(int drink, int change, int machineMoney, int userMoney);
int insertMoney(int* userMoney, int* machineMoney);
int selectMenu(int drinkTypes, Drink drinks[], int* choice, int* machineMoney);
int dispenseDrink(Drink drinks[], int choice, int machineMoney, int userMoney, int state);
int returnChange(int* machineMoney, int* userMoney);
int exitProgram();
int askYesNo(int state1, int state2);

void writeFile(FILE* fp, Drink drinks[], int drinkTypes, int totalSales, int totalRevenue);


int main(void) {
	// ��ȣ�ۿ� ���� ����
	int userMoney = 5000;	// ����� ���� �ݾ�
	int machineMoney = 0;	// ���Ǳ� ���� �ݾ�	
	int state = 0;		// ���Ǳ� ����
	int mode = 0;	// ���� ��� 
	int choice = 0;		// ����� �Է°�	

	// ���� �б�� ����
	int drinkTypes;	// ���� ���� ���� ��
	int totalSales;	// �� �Ǹŷ�
	int totalRevenue;	// �� ����


	// ���� ����
	FILE* fp = fopen(FILE_NAME, "a+t");
	if (fp == NULL) {
		printf("������ �ҷ����� ����\n");
		printf("���α׷��� �����մϴ�.\n");
		return -1;
	}
	// Ŀ�� ��ġ ó������
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
		char name[50];
		int price;
		int stock;
		fscanf(fp, "%s %d %d", name, &price, &stock);
		strcpy(drinks[i].name, name);
		drinks[i].price = price;
		drinks[i].stock = stock;
	}

	// ������ ���	
	printf("���� ���� : %d��\n", drinkTypes);
	printf("�� �Ǹŷ� : %d��\n", totalSales);
	printf("�� ���� : %d��\n", totalRevenue);
	// drinks �迭
	printf("\n���� ���\n");
	for (int i = 0; i < drinkTypes; i++) {
		printf("%-20s %-10d %-10d\n", drinks[i].name, drinks[i].price, drinks[i].stock);
	}
	
	/*fclose(fp);
	return 0;*/


	while (1) {
		// �ʱ� ȭ��
		printMachine(0, 0, machineMoney, userMoney);
		printf("\n--- ��� �Է� ---\n");
		printf("������ ��� : 0\n����� ��� : 1\n\n");
		printf("�Է� : ");
		scanf("%d", &mode);
		while (getchar() != '\n');


		// ������ ���
		if (mode == 0) {
			int answer;	// ������ �Է�	
			int selectedDrinkIndex;	// ������ ���� ��ȣ
			char data[100]; // ���� ���� ���� ����
			while (1) {
				system("cls");
				printf("--- ������ ��� ---\n\n");

				printf("�б� ��� : 0\n���� ��� : 1\n���� : -1\n\n");
				printf("�Է� : ");
				scanf("%d", &answer);
				getchar();

				fseek(fp, 0, SEEK_SET);

				// �б� ���
				if (answer == 0) {
					system("cls");
					while (fgets(data, sizeof(data), fp) != NULL) {
						printf("%s", data);
					}
					printf("\n\nEnter�� ���� ó������ ���ư��ϴ�.");
					while (getchar() != '\n');
				}

				// ���� ���
				else if (answer == 1) {
					system("cls");
					// ���� ��� ���
					printf("---���� ���---\n\n");
					printf("   %-17s %-10s %-10s\n", "�̸�", "����", "���");
					printf("-----------------------------------------------\n");
					for (int i = 0; i < drinkTypes; i++) {
						printf("%d. %-17s %-10d %-10d\n", i + 1, drinks[i].name, drinks[i].price, drinks[i].stock);
					}

					// ������ ���� ����
					printf("\n������ ���Ḧ �����ϼ���.\n");
					printf("0 : �ڷ� ����\n\n");
					printf("�Է� : ");
					while (scanf("%d", &selectedDrinkIndex) != 1 || (selectedDrinkIndex < 0 || selectedDrinkIndex > drinkTypes)) {
						while (getchar() != '\n');
						printf("\n0~%d�� ���ڸ� �Է����ּ���.\n", drinkTypes);
						printf("�Է� : ");
					}
					if (selectedDrinkIndex == 0) continue;

					// ������ ���� ����					
					int editField;
					system("cls");
					printf("1. �̸� : %s\n2. ���� : %d\n3. ��� : %d\n\n", drinks[selectedDrinkIndex -1].name, drinks[selectedDrinkIndex -1].price, drinks[selectedDrinkIndex -1].stock);
					printf("������ �׸��� ��ȣ�� �����ϼ���.\n");
					printf("0 : �ڷ� ����\n\n");
					printf("�Է� : ");
					while (scanf("%d", &editField) != 1 || (editField < 0 || editField > 3)) {
						while (getchar() != '\n');
						printf("\n0~3�� ���ڸ� �Է����ּ���.\n");
						printf("�Է� : ");
					}
					if (editField == 0) continue;

					// ���� ���� ����
					while (1) {
						printf("\n������ ������ �Է����ּ���.\n\n");
						char name[50];
						int price;
						int stock;
						char yn;
						switch (editField) {
						case 1:
							printf("�̸� : ");
							scanf("%s", name);
							printf("\n�Է� ������ Ȯ�����ּ���.\n");
							printf("�Է� ���� : %s\n\n", name);
							break;
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
						case 3:
							printf("��� : ");
							while (scanf("%d", &stock) != 1 || stock < 0) {
								while (getchar() != '\n');
								printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
								printf("��� : ");
							}
							printf("\n�Է� ������ Ȯ�����ּ���.\n");
							printf("�Է� ���� : %d\n\n", stock);
							break;
						default:
							printf("�Է� ����\n");
							return -1;
						}
						while (getchar() != '\n');
						printf("�Է��� ������ �����Ͻðڽ��ϱ�? (Q : ���� ���)\n");
						printf("Y/N : ");
						scanf("%c", &yn);
						while (getchar() != '\n');
						if (yn == 'y' || yn == 'Y') {					
							strcpy(drinks[selectedDrinkIndex - 1].name, name);
							writeFile(fp, drinks, drinkTypes, totalSales, totalRevenue);
							printf("\n�Է��� ������ ����Ǿ����ϴ�.\n\n");
							printf("Enter�� �Է��Ͽ� ���.\n");
							while (getchar() != '\n');
							break;
						}
						else if (yn == 'n' || yn == 'N') {
							printf("�ٽ� �Է����ּ���.\n\n");
						}
						else if (yn == 'q' || yn == 'Q') break;
					}
				}

				else break;
			}
			printf("���α׷��� �����մϴ�.\n");
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
		else if (mode == 1) {
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
					break;

					// �ܵ� ��ȯ
				case 3:
					printMachine(0, 1, machineMoney, userMoney);
					state = returnChange(&machineMoney, &userMoney);
					break;

					// ���α׷� ����
				case 4:
					printf("\n\n���α׷��� �����մϴ�.");
					return 0;
					break;

				default:
					printf("���α׷� ����. ���α׷��� �����մϴ�.");
					return -1;
				}
			}
		}

		else {
			printf("�߸��� ���Դϴ�. ���α׷��� �����մϴ�.");
			return -1;
		}
	}
	return 0;
}


// ���� �ϰ� �ִ� ��.
// ������ ������ �����ϰ� ������ ���� �Է��ϴ� �ͱ��� ����.
// ���� �Լ��� �и��ؾ� �ڵ� ¥�� ���� �� ����.

// todo
// 0. ������ �׸� ���������� ������ �����ϴ� ���μ����� �����ؾ� ��. (����ϰ�...)
// 1. ����� ��忡���� �ൿ�� ���� �����͸� ����ؾ��� (���ų�¥,�Ǹ�ǰ��,����,���������Ʈ ��?)
// 2. ���� ����� �ٸ� ������ ���� �ϴ� �� ������.
// 3. ó���� ������ ���� ���� �ƴ϶� �����ڸ��, ����ڸ��� ������ ������ ������ ���� ����� ������?
// 4. UI�� ����� ������ ���� �������� �����غ���?


// ���Ǳ� ��� �Լ�
void printMachine(int drink, int change, int machineMoney, int userMoney) {
	system("cls");
	printf("### ���� ���Ǳ� ###\n");
	printf("���� �ݾ� : %5d��\n\n", userMoney);
	printf("-----------------------------------------------------------------------------\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t-------------\t-------------\t-------------\t-------------\t    -\n");
	printf("-\t-    �ݶ�   -\t-   ���̴�  -\t-     ��    -\t- �Ŀ����̵�-\t    -\n");
	printf("-\t-           -\t-           -\t-           -\t-           -\t    -\n");
	printf("-\t-   1000��  -\t-   1200��  -\t-   800��   -\t-   1500��  -\t    -\n");
	printf("-\t-           -\t-           -\t-           -\t-           -\t    -\n");
	printf("-\t-  +++++++  -\t-  +++++++  -\t-  +++++++  -\t-  +++++++  -\t    -\n");
	printf("-\t-  +  1  +  -\t-  +  2  +  -\t-  +  3  +  -\t-  +  4  +  -\t    -\n");
	printf("-\t-  +++++++  -\t-  +++++++  -\t-  +++++++  -\t-  +++++++  -\t    -\n");
	printf("-\t-------------\t-------------\t-------------\t-------------\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                                 \t\t    -\n");
	printf("-\t                                  \t ���� �ݾ� : %5d��\t    -\n", machineMoney);
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


// ����� ��� ���� �Լ�

// ���� ����
int insertMoney(int* userMoney, int* machineMoney) {
	int state;
	int cash;
	printf("\n--- ���� ���� ---\n\n");
	printf("���� �ݾ��� �Է����ּ��� (0 : ���α׷� ����) : ");
	while (scanf("%d", &cash) != 1 || (cash > *userMoney || cash < 0) || cash % 10 != 0) {
		while (getchar() != '\n');
		if (cash % 10 != 0) {
			printf("\n10�� ������ �Է����ּ���.\n\n");
		}
		else {
			printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
			printf("���� ������ �ݾ׿� �°� �Է����ּ���.\n\n");
		}
		printf("���� �ݾ��� �Է����ּ��� : ");
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
// �޴� ����
int selectMenu(int drinkTypes, Drink drinks[], int* choice, int* machineMoney) {
	int state;
	printf("\n--- �޴� ---\n\n");
	for (int i = 0; i < drinkTypes; i++) {
		printf("%d. %s : %d��\n", i + 1, drinks[i].name, drinks[i].price);
	}

	printf("\n��ȣ�� �Է����ּ��� (0 : ó������) : ");
	while (scanf("%d", choice) != 1 || (*choice < 0 || *choice > drinkTypes)) {
		while (getchar() != '\n');
		printf("1~%d�� ���ڸ� �Է����ּ���.\n", drinkTypes);
		printf("\n��ȣ�� �Է����ּ��� : ");
	}
	while (getchar() != '\n');
	if (*choice == 0) {
		if (*machineMoney <= 0) state = 0;
		else state = 3;
		return state;
	}

	if (drinks[*choice - 1].price > *machineMoney) {
		char answer;
		printf("\n�ܵ��� �����մϴ�.\n");
		printf("������ �� �����ðڽ��ϱ�?\n\n");
		state = askYesNo(0, 1);
	}
	else {
		*machineMoney -= drinks[*choice - 1].price;
		state = 2;
	}
	return state;
}
// ���� ����
int dispenseDrink(Drink drinks[], int choice, int machineMoney, int userMoney, int state) {
	char answer;
	printf("\n--- ���� ���� ---\n\n");
	printf("%s �帮�ڽ��ϴ�.\n\nEnterŰ�� ���� �޾��ּ���.\n", drinks[choice - 1].name);
	while (getchar() != '\n');
	printMachine(0, 0, machineMoney, userMoney);
	printf("\n���ְ� �弼��!\n\n");
	printf("���Ḧ �� �ֹ��Ͻ� �ǰ���?\n");

	state = askYesNo(1, 3);
	if (state == 1 && machineMoney <= 0) state = 0;
	if (state == 3 && machineMoney <= 0) state = exitProgram();


	return state;
}
// �ܵ� ��ȯ
int returnChange(int* machineMoney, int* userMoney) {
	printf("\n--- �ܵ� ��ȯ ---\n\n");
	printf("�ܵ� %d���� �帮�ڽ��ϴ�.\n\nEnterŰ�� ���� �޾��ּ���.", *machineMoney);
	while (getchar() != '\n');
	*userMoney += *machineMoney;
	*machineMoney = 0;
	printMachine(0, 0, *machineMoney, *userMoney);
	return exitProgram();
}
// ���Ǳ� ����
int exitProgram() {
	printf("\n���Ǳ� �̿��� �����Ͻðڽ��ϱ�?\n");
	return askYesNo(4, 0);
}
// Yes or No �Է� �ޱ�
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
			printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
			continue;
		}
	}
	return state;
}


// ������ ��� ���� �Լ�

// ���� ���� ���� �Լ�
void writeFile(FILE* fp, Drink drinks[], int drinkTypes, int totalSales, int totalRevenue) {
	FILE* temp = fopen("temp.txt", "w+t");
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
}