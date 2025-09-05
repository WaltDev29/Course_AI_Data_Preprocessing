#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char* name;
	int price;
} Drink;

void PrintMachine(int drink, int change, int machineMoney, int userMoney);

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
	PrintMachine(0, 0, machineMoney, userMoney);
	printf("\n--- ��� �Է� ---\n");
	printf("������ ��� : 0\n����� ��� : 1\n\n");
	printf("�Է� : ");
	scanf("%d", &mode);
	while (getchar() != '\n');

	while (1) {
		if (mode == 0) {

		}
		else if (mode == 1) {
			while (1) {
				switch (state) {
					// ���� ����
				case 0:
					PrintMachine(0, 0, machineMoney, userMoney);
					printf("\n--- ���� ���� ---\n\n");
					printf("���� �ݾ��� �Է����ּ��� : ");
					while (scanf("%d", &floatingMoney) != 1 || (floatingMoney > userMoney || floatingMoney <= 0)) {
						while (getchar() != '\n');
						if (floatingMoney % 10 != 0) {
							printf("10�� ������ �Է����ּ���.\n\n");
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
					state = 1;
					break;

					// �޴� ���
				case 1:
					PrintMachine(0, 0, machineMoney, userMoney);
					printf("\n--- �޴� ---\n\n");
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
						break;
					}
					else machineMoney -= drinks[choice - 1].price;

					state = 2;
					break;

					// ���� ����
				case 2:
					PrintMachine(1, 0, machineMoney, userMoney);
					printf("\n--- ���� ���� ---\n\n");
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
					}
					break;

					// �ܵ� ��ȯ
				case 3:
					PrintMachine(0, 1, machineMoney, userMoney);
					printf("\n--- �ܵ� ��ȯ ---\n\n");
					printf("�ܵ� %d���� �帮�ڽ��ϴ�.\n\nEnterŰ�� ���� �޾��ּ���.", machineMoney);
					while (getchar() != '\n');
					userMoney += machineMoney;
					machineMoney = 0;
					state = 0;
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
// 3. ���� �ܰ�� ���ư� �� �ֵ���
// 4. �� �ܰ� �Լ�ȭ


// ���Ǳ� ��� �Լ�
void PrintMachine(int drink, int change, int machineMoney, int userMoney) {
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