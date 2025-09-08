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

	int userMoney = 5000;	// ����� ���� �ݾ�
	int machineMoney = 0;	// ���Ǳ� ���� �ݾ�	
	int state = 0;		// ���Ǳ� ����
	int mode = 0;	// ���� ��� 
	int choice = 0;		// ����� �Է°�	

	// ���� �б� �� ����
	char name[100];
	int price;
	int stock;
	int num;

	FILE* fp = fopen("machine_data.txt", "a+t");
	if (fp == NULL) {
		printf("������ �ҷ����� ����\n");
		printf("���α׷��� �����մϴ�.\n");
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

	printf("### ���� ���Ǳ� ###\n\n");
	printMachine(0, 0, machineMoney, userMoney);
	printf("\n--- ��� �Է� ---\n");
	printf("������ ��� : 0\n����� ��� : 1\n\n");
	printf("�Է� : ");
	scanf("%d", &mode);
	while (getchar() != '\n');

	while (1) {
		if (mode == 0) {
			int answer;	// ������ �Է�						
			while (1) {
				system("cls");
				printf("--- ������ ��� ---\n\n");

				printf("�б� ��� : 0\n���� ��� : 1\n���� : -1\n\n");
				printf("�Է� : ");
				scanf("%d", &answer);
				getchar();

				fseek(fp, 0, SEEK_SET);
				if (answer == 0) {
					while (fgets(data, sizeof(data), fp) != NULL) {
						printf("%s", data);
					}
					printf("\n\nEnter�� ���� ó������ ���ư��ϴ�.");
					while (getchar() != '\n');
				}

				else if (answer == 1) {
					printf("\n�����͸� �Է��ϼ��� (EXIT �Է� �� �Է� ����) : \n");
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
// ���� ����Ʈ�� �̸� �ۼ��س�����.
// ���� ����Ʈ�� �о �������� drinks ����Ʈ�� ���鵵�� ��.

// todo
// 0. ������ ������ ������ ��� �� ���ΰ�.
//		(�� ��� : 10 �̷� ������ �ۼ��ϰ� ������, ���� ���� ��� ����?)
// 1. ������ ��� ���� (���� �����) (�����͸� ��� ���/������ ���ΰ�.)
// 2. UI�� ����� ������ ���� �������� �����غ���?


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

int returnChange(int* machineMoney, int* userMoney) {
	printf("\n--- �ܵ� ��ȯ ---\n\n");
	printf("�ܵ� %d���� �帮�ڽ��ϴ�.\n\nEnterŰ�� ���� �޾��ּ���.", *machineMoney);
	while (getchar() != '\n');
	*userMoney += *machineMoney;
	*machineMoney = 0;
	printMachine(0, 0, *machineMoney, *userMoney);
	return exitProgram();
}

int exitProgram() {
	printf("\n���Ǳ� �̿��� �����Ͻðڽ��ϱ�?\n");
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
			printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
			continue;
		}
	}
	return state;
}