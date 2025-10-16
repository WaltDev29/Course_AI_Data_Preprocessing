#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "vending.h"

// ����� ��� ���� �Լ�

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
// ��� ����
int selectMode() {
	int mode;
	printf("\n--- ��� �Է� ---\n");
	printf("������ ��� : 1\n����� ��� : 2\n���� : 0\n\n");
	printf("�Է� : ");
	while (scanf("%d", &mode) != 1) {
		waitEnter("\n���ڸ� �Է����ּ���.\n�ٽ� �Է� : ");
	}
	while (getchar() != '\n');
	return mode;
}

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
		if (drinks[i].stock <= 0) printf("%d. %s : %d�� (����)\n", i + 1, drinks[i].name, drinks[i].price);
		else printf("%d. %s : %d��\n", i + 1, drinks[i].name, drinks[i].price);
	}

	printf("\n��ȣ�� �Է����ּ��� (0 : ó������) : ");
	while (scanf("%d", choice) != 1 || (*choice < 0 || *choice > drinkTypes)) {
		while (getchar() != '\n');
		printf("1~%d�� ���ڸ� �Է����ּ���.\n", drinkTypes);
		printf("\n��ȣ�� �Է����ּ��� : ");
	}
	while (getchar() != '\n');
	if (*choice != 0 && drinks[*choice - 1].stock <= 0) {
		waitEnter("\n�ش� ����� ���� ��� �����ϴ�.\nEnter�� �Է��Ͽ� �޴��� ���ư��ϴ�.\n");
		return state = 1;
	}
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
	printf("%s �帮�ڽ��ϴ�.\n\n", drinks[choice - 1].name);
	waitEnter("EnterŰ�� ���� �޾��ּ���.\n");

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
	printf("�ܵ� %d���� �帮�ڽ��ϴ�.\n\n", *machineMoney);
	waitEnter("EnterŰ�� ���� �޾��ּ���.\n");
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
			printf("\n�߸��� ���� �Է��߽��ϴ�.\n");
			continue;
		}
	}
	return state;
}