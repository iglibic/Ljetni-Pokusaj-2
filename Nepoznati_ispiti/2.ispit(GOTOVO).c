/*Napisati program koji iz datoteke "hoteli.txt" čita popis hotela te kreira vezanu
listu sortiranu po datumu izgradnje hotela. Ispisati sortiranu vezanu listu. Izračunati
srednju vrijednost godišnjih prihoda svih hotela pa ispisati listu tako da se iz prethodne
liste izbace hoteli s prihodom manjim od izračunate srednje vrijednosti.*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

HotelP createNewHotel(char* name, int income, Date date);
int loadFromFile(HotelP head, const char* fileName);
int insertSorted(HotelP head, HotelP newHotel);
int printList(HotelP first);
int freeList(HotelP head);

float calcAvgIncome(HotelP head);
int removeBlwAvg(HotelP head, float avg);

int main() {
	HotelP head = (HotelP)malloc(sizeof(Hotel));
	if (head == NULL) {
		printf("ERROR! Could not allocate memmory1!\n");
		return EXIT_FAILURE;
	}
	head->next = NULL;

	loadFromFile(head, "HOTELI.txt");

	printf("Pocetna lista: \n");
	printList(head->next);

	float avgIncome = calcAvgIncome(head);
	printf("\nProsjecni godisnji prihod svih hotela je: %f\n", avgIncome);

	printf("\nUklanjanje hotela ciji je prihod manji od prosjecnog...\n");
	removeBlwAvg(head, avgIncome);

	printf("\nKonacna lista: \n");
	printList(head->next);

	freeList(head);

	return EXIT_SUCCESS;
}

HotelP createNewHotel(char* name, int income, Date date) {
	HotelP newHotel = (HotelP)malloc(sizeof(Hotel));
	if (newHotel == NULL) {
		printf("ERROR! Could not allocate memmory2!\n");
		return NULL;
	}
	strcpy(newHotel->name, name);
	newHotel->income = income;
	newHotel->date = date;
	newHotel->next = NULL;

	return newHotel;
}

int loadFromFile(HotelP head, const char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("ERROR! Could not open the file!\n");
		exit(EXIT_FAILURE);
	}
	char name[MAX];
	int d, m, y, salary;

	while (fscanf(fp, "%s %d-%d-%d %d", name, &y, &m, &d, &salary) == 5) {			//%[^0-9] ili %[^\t\n0-9]
		Date date = { y, m, d };
		HotelP newHotel = createNewHotel(name, salary, date);
		if (newHotel == NULL) {
			printf("ERROR! Could not allocate memmory3!\n");
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
		printf("Godina izgradnje: %d-%d-%d, Ime hotela: %s, Godisnji prihodi: %d\n", temp->date.year, 
			temp->date.month, temp->date.day, temp->name, temp->income);
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
	int total = 0;
	float totalIncome = 0.0;
	while (temp) {
		totalIncome += temp->income;
		total++;
		temp = temp->next;
	}
	return totalIncome / total;
}

int removeBlwAvg(HotelP head, float avg) {
	HotelP prev = head;
	HotelP curr = head->next;
	while (curr) {
		if ((float)curr->income < avg) {
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
