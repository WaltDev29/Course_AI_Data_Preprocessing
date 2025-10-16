#pragma once

#define FILE_NAME "machine_data.txt"

typedef struct {
	char name[50];
	int price;
	int stock;
} Drink;

void checkFileOpen(FILE* fp);
int selectMode();
void waitEnter(char* str);
// 사용자 모드 관련
void printMachine(int drink, int change, int machineMoney, int userMoney);
int insertMoney(int* userMoney, int* machineMoney);
int selectMenu(int drinkTypes, Drink drinks[], int* choice, int* machineMoney);
int dispenseDrink(Drink drinks[], int choice, int machineMoney, int userMoney, int state);
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