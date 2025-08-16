/*Napisati program koji iz datoteke "hoteli.txt" čita popis hotela te kreira vezanu
listu sortiranu po datumu izgradnje hotela. Ispisati sortiranu vezanu listu. Izračunati
srednju vrijednost godišnjih prihoda svih hotela pa ispisati listu tako da se iz prethodne
liste izbace hoteli s prihodom manjim od izračunate srednje vrijednosti.*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILE_NOT_OPENED (-1)
#define MAX 32

typedef struct _date {
	int year;
	int month;
	int day;
}Date;

typedef struct _hotel* HotelP;
typedef struct _hotel {
	char name[MAX];
	Date date;
	int income;
	HotelP next;
}Hotel;

HotelP createNewHotel(char* hName, int income, Date date);
int loadFromFile(HotelP head, const char* fileName);
int insertSorted(HotelP head, HotelP newHotel);
int printList(HotelP first);
int freeList(HotelP head);

float calcAvgIncome(HotelP head);
int removeBelowAvg(HotelP head, float avgIncome);

int main() {
	HotelP head = (HotelP)malloc(sizeof(Hotel));
	if (head == NULL) {
		printf("ERROR! Could not allocate memmory!\n");
		return EXIT_FAILURE;
	}
	head->next = NULL;

	loadFromFile(head, "HOTELI.txt");

	printList(head->next);

	float incomeAvg = calcAvgIncome(head);

	printf("\nProsjek prihoda hotela: %f\n", incomeAvg);

	removeBelowAvg(head, incomeAvg);

	printList(head->next);

	freeList(head);

	return EXIT_SUCCESS;
}

HotelP createNewHotel(char* hName, int income, Date date) {
	HotelP newHotel = (HotelP)malloc(sizeof(Hotel));
	if (newHotel == NULL) {
		printf("ERROR! Could not allocate memmory!\n");
		return NULL;
	}
	strcpy(newHotel->name, hName);
	newHotel->income = income;
	newHotel->date = date;
	newHotel->next = NULL;

	return newHotel;
}

int loadFromFile(HotelP head, const char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("ERROR! Could not open the file!\n");
		return EXIT_FAILURE;
	}

	char name[MAX];
	int y, m, d, income;

	while (fscanf(fp, "%s %d-%d-%d %d", name, &y, &m, &d, &income) == 5) {
		Date date = { y, m, d };
		HotelP newHotel = createNewHotel(name, income, date);
		if (newHotel == NULL) {
			printf("ERROR! Could not allocate memmory!\n");
			return EXIT_FAILURE;
		}
		insertSorted(head, newHotel);
	}
	fclose(fp);

	return EXIT_SUCCESS;
}

int insertSorted(HotelP head, HotelP newHotel) {
	HotelP prev = head;
	HotelP curr = head->next;

	while (curr) {
		Date date1 = newHotel->date;
		Date date2 = curr->date;

		if (date1.year < date2.year) {
			break;
		}
		else {
			if ((date1.year == date2.year) && (date1.month < date2.month)) {
				break;
			}
			else {
				if ((date1.year == date2.year) && (date1.month == date2.month) && (date1.day <= date2.day)) {
					break;
				}
				else {
					prev = curr;
					curr = curr->next;
				}
			}
		}
	}

	prev->next = newHotel;
	newHotel->next = curr;

	return EXIT_SUCCESS;
}

int printList(HotelP first) {
	HotelP temp = first;
	while (temp) {
		printf("Ime hotela: %s, Datum izgradnje: %d-%d-%d, Godisnji prihod: %d\n", temp->name, temp->date.year, 
			temp->date.month, temp->date.day, temp->income);
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}

int freeList(HotelP head) {
	HotelP temp = head->next;
	while (temp) {
		HotelP toFree = temp;
		temp = temp->next;
		free(toFree);
	}
	head->next = NULL;

	return EXIT_SUCCESS;
}

float calcAvgIncome(HotelP head) {
	HotelP temp = head->next;
	int totalIncome = 0;
	int numOfHotels = 0;

	while (temp) {
		totalIncome += temp->income;
		numOfHotels++;
		temp = temp->next;
	}
	return (float)totalIncome / numOfHotels;
}

int removeBelowAvg(HotelP head, float avgIncome) {
	HotelP prev = head;
	HotelP curr = head->next;

	while (curr) {
		if ((float)curr->income < avgIncome) {
			prev->next = curr->next;
			free(curr);
			curr = prev->next;
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}

	return EXIT_SUCCESS;
}
