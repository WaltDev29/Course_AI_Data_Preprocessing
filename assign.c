#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* name;
	int price;
} Drink;

void printMachine(int drink, int change, int machineMoney, int userMoney);
void insertMoney(int* userMoney, int* machineMoney, int* state);
void selectMenu(int* drinkTypes, Drink drinks[], int* choice, int* machineMoney, char* answer, int* state);
void dispenseDrink(Drink drinks[], int* choice, char* answer, int* machineMoney, int* state);
void returnChange(int* machineMoney, int* userMoney, int* state);
void exitProgram(char* answer, int* state);

int main(void) {
	int userMoney = 5000;	// ����� ���� �ݾ�
	int machineMoney = 0;	// ���Ǳ� ���� �ݾ�
	int floatingMoney = 0;
	int state = 0;		// ���Ǳ� ����
	int mode = 0;	// ���� ��� 
	int choice = 0;		// ����� �Է°�
	char answer;	// ����� ����		

	Drink coke = { "�ݶ�", 1000 };
	Drink cider = { "���̴�", 1200 };
	Drink water = { "��", 800 };
	Drink powerAde = { "�Ŀ����̵�", 1500 };
	Drink drinks[] = { coke, cider, water, powerAde };

	int drinkTypes = sizeof(drinks) / sizeof(Drink);

	printf("### ���� ���Ǳ� ###\n\n");
	printMachine(0, 0, machineMoney, userMoney);
	printf("\n--- ��� �Է� ---\n");
	printf("������ ��� : 0\n����� ��� : 1\n\n");
	printf("�Է� : ");
	scanf("%d", &mode);
	while (getchar() != '\n');

	while (1) {
		if (mode == 0) {
			// ������ ���
			// ���� ����� ��� (���� ��ȸ, ����)			
			// # ���� ����(�߰� ���� ����)
			//	 - ���� �̸�, ����, ���, �Ǹŷ�, (�Ǹż���?)
			//	 - �� �Ǹŷ�, �� ����
			//	 - (���� ���� ���)
		}
		else if (mode == 1) {
			while (1) {
				switch (state) {
					// ���� ����
				case 0:
					printMachine(0, 0, machineMoney, userMoney);
					insertMoney(&userMoney, &machineMoney, &state);
					/*printf("\n--- ���� ���� ---\n\n");
					printf("���� �ݾ��� �Է����ּ��� : ");
					while (scanf("%d", &floatingMoney) != 1 || (floatingMoney > userMoney || floatingMoney <= 0) || floatingMoney % 10 != 0) {
						while (getchar() != '\n');
						if (floatingMoney % 10 != 0) {
							printf("\n10�� ������ �Է����ּ���.\n\n");
						}
						else {
							printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
							printf("���� ������ �ݾ׿� �°� �Է����ּ���.\n\n");
						}
						printf("���� �ݾ��� �Է����ּ��� : ");
					}
					while (getchar() != '\n');
					userMoney -= floatingMoney;
					machineMoney += floatingMoney;
					state = 1;*/
					break;

					// �޴� ���
				case 1:
					printMachine(0, 0, machineMoney, userMoney);
					selectMenu(&drinkTypes, drinks, &choice, &machineMoney, &answer, &state);
					/*printf("\n--- �޴� ---\n\n");
					for (int i = 0; i < drinkTypes; i++) {
						printf("%d. %s : %d��\n", i + 1, drinks[i].name, drinks[i].price);
					}

					printf("\n��ȣ�� �Է����ּ��� : ");
					while (scanf("%d", &choice) != 1 || (choice <= 0 || choice > drinkTypes)) {
						while (getchar() != '\n');
						printf("1~%d�� ���ڸ� �Է����ּ���.\n", drinkTypes);
						printf("\n��ȣ�� �Է����ּ��� : ");
					}
					while (getchar() != '\n');

					if (drinks[choice - 1].price > machineMoney) {
						printf("\n�ܵ��� �����մϴ�.\n");
						printf("������ �� �����ðڽ��ϱ�?\n\n");
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
								printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
								printf("Y/N : ");
								continue;
							}
						}
					}
					else machineMoney -= drinks[choice - 1].price;

					state = 2;*/
					break;

					// ���� ����
				case 2:
					printMachine(1, 0, machineMoney, userMoney);
					dispenseDrink(drinks, &choice, &answer, &machineMoney, &state);
					/*printf("\n--- ���� ���� ---\n\n");
					printf("%s �帮�ڽ��ϴ�.\n\nEnterŰ�� ���� �޾��ּ���.\n", drinks[choice - 1].name);
					while (getchar() != '\n');
					printf("���ְ� �弼��!\n\n");
					printf("���Ḧ �� �ֹ��Ͻ� �ǰ���?\n");
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
							printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
							printf("Y/N : ");
							continue;
						}
					}*/
					break;

					// �ܵ� ��ȯ
				case 3:
					printMachine(0, 1, machineMoney, userMoney);
					returnChange(&machineMoney, &userMoney, &state);
					/*printf("\n--- �ܵ� ��ȯ ---\n\n");
					printf("�ܵ� %d���� �帮�ڽ��ϴ�.\n\nEnterŰ�� ���� �޾��ּ���.", machineMoney);
					while (getchar() != '\n');
					userMoney += machineMoney;
					machineMoney = 0;
					state = 0;*/
					exitProgram(&answer, &state);					
					break;
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



// todo
// 1. ������ ��� ���� (���� �����) (�����͸� ��� ����� ���ΰ�.)
// 2. ���α׷� ���� �� ���Ϸκ��� �����͸� �о ����
// 3. ���� �ܰ�� ���ư� �� �ֵ���


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

void insertMoney(int* userMoney, int* machineMoney, int* state) {
	int floatingMoney;
	printf("\n--- ���� ���� ---\n\n");
	printf("���� �ݾ��� �Է����ּ��� : ");
	while (scanf("%d", &floatingMoney) != 1 || (floatingMoney > *userMoney || floatingMoney <= 0) || floatingMoney % 10 != 0) {
		while (getchar() != '\n');
		if (floatingMoney % 10 != 0) {
			printf("\n10�� ������ �Է����ּ���.\n\n");
		}
		else {
			printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
			printf("���� ������ �ݾ׿� �°� �Է����ּ���.\n\n");
		}
		printf("���� �ݾ��� �Է����ּ��� : ");
	}
	while (getchar() != '\n');
	*userMoney -= floatingMoney;
	*machineMoney += floatingMoney;
	*state = 1;
}

void selectMenu(int* drinkTypes, Drink drinks[], int* choice, int* machineMoney, char* answer, int* state) {
	printf("\n--- �޴� ---\n\n");
	for (int i = 0; i < *drinkTypes; i++) {
		printf("%d. %s : %d��\n", i + 1, drinks[i].name, drinks[i].price);
	}

	printf("\n��ȣ�� �Է����ּ��� : ");
	while (scanf("%d", choice) != 1 || (*choice <= 0 || *choice > *drinkTypes)) {
		while (getchar() != '\n');
		printf("1~%d�� ���ڸ� �Է����ּ���.\n", *drinkTypes);
		printf("\n��ȣ�� �Է����ּ��� : ");
	}
	while (getchar() != '\n');

	if (drinks[*choice - 1].price > *machineMoney) {
		printf("\n�ܵ��� �����մϴ�.\n");
		printf("������ �� �����ðڽ��ϱ�?\n\n");
		printf("Y/N : ");
		while (1) {
			scanf("%c", answer);
			if (*answer == 'y' || *answer == 'Y') {
				while (getchar() != '\n');
				*state = 0;
				break;
			}
			else if (*answer == 'n' || *answer == 'N') {
				while (getchar() != '\n');
				*state = 1;
				break;
			}
			else {
				while (getchar() != '\n');
				printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
				printf("Y/N : ");
				continue;
			}
		}
	}
	else {
		*machineMoney -= drinks[*choice - 1].price;
		*state = 2;
	}
}

void dispenseDrink(Drink drinks[], int* choice, char* answer, int* machineMoney, int* state) {
	printf("\n--- ���� ���� ---\n\n");
	printf("%s �帮�ڽ��ϴ�.\n\nEnterŰ�� ���� �޾��ּ���.\n", drinks[*choice - 1].name);
	while (getchar() != '\n');
	printf("���ְ� �弼��!\n\n");
	printf("���Ḧ �� �ֹ��Ͻ� �ǰ���?\n");
	printf("Y/N : ");
	while (1) {
		scanf("%c", answer);
		if (*answer == 'y' || *answer == 'Y') {
			while (getchar() != '\n');
			*state = 1;
			break;
		}
		else if (*answer == 'n' || *answer == 'N') {
			while (getchar() != '\n');
			if (*machineMoney > 0) *state = 3;
			else exitProgram(answer, state);
			break;
		}
		else {
			while (getchar() != '\n');
			printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
			printf("Y/N : ");
			continue;
		}
	}
}

void returnChange(int* machineMoney, int* userMoney, int* state) {
	printf("\n--- �ܵ� ��ȯ ---\n\n");
	printf("�ܵ� %d���� �帮�ڽ��ϴ�.\n\nEnterŰ�� ���� �޾��ּ���.", *machineMoney);
	while (getchar() != '\n');
	*userMoney += *machineMoney;
	*machineMoney = 0;
}

void exitProgram(char* answer, int* state) {
	printf("\n���Ǳ� �̿��� �����Ͻðڽ��ϱ�?\n");
	printf("Y/N : ");
	while (1) {
		scanf("%c", answer);
		if (*answer == 'y' || *answer == 'Y') {
			while (getchar() != '\n');
			*state = 4;
			break;
		}
		else if (*answer == 'n' || *answer == 'N') {
			while (getchar() != '\n');
			*state = 0;
			break;
		}
		else {
			while (getchar() != '\n');
			printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
			printf("Y/N : ");
			continue;
		}
	}
}