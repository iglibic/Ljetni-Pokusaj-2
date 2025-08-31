/*Napisati program koji iz datoteke "studenti.txt" čita prezime i
ime studenta kao i njegov ID, te kreira vezanu listu sortiranu
po prezimenu i imenu studenta.
Nakon toga iz datoteka "predmet1.txt" i "predmet2.txt" se čita
ID studenta, ocjena i datum polaganja predmeta. Pročitane
podatke je potrebno unijeti u postojeću vezanu listu. Ispisati
listu.

OCJENA 2: Unijeti jedan datum i izbrisati iz liste sve one osobe
koje su položile bilo koji od predmeta na taj datum. Ispisati novu
vezanu listu.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NOT_OPENED (-1)
#define MAX_NAME_LENGTH (32)

struct _date;
typedef struct _date {
	int year;
	int month;
	int day;
} Date;

struct _student;
typedef struct _student* StudentP;
typedef struct _student {
	char ID[10];
	char firstName[MAX_NAME_LENGTH];
	char lastName[MAX_NAME_LENGTH];
	int grade[2];
	StudentP next;
	Date date[2];
} Student;

StudentP createNewStudent(char* id, char* fname, char* lname);
int loadFromFile(StudentP head, const char* fileName);
int insertSorted(StudentP head, StudentP newStudent);
int loadGrades(StudentP head, const char* fileName, int index);
int insertGradeAndDate(StudentP head, char* id, int grade, int index, Date date);
int printList(StudentP first);
int freeList(StudentP head);

int deleteByDate(StudentP head, Date date);

int main() {
	StudentP head = (StudentP)malloc(sizeof(Student));
	if (head == NULL) {
		printf("ERROR! Could not allocate memmory!\n");
		return EXIT_FAILURE;
	}
	head->next = NULL;

	loadFromFile(head, "studentii.txt");

	loadGrades(head, "predmet1.txt", 0);
	loadGrades(head, "predmet2.txt", 1);

	printf("Pocetna lista: \n");
	printList(head->next);

	int d, m, y;

	printf("Unesite datum za brisanje(DD/MM/YYY): \n");
	scanf("%d.%d.%d", &d, &m, &y);

	Date date = { y, m, d };

	deleteByDate(head, date);

	printf("\nLista nakon brisanja: \n");
	printList(head->next);

	freeList(head);

	return EXIT_SUCCESS;
}

StudentP createNewStudent(char* id, char* fname, char* lname) {
	StudentP newStudent = (StudentP)malloc(sizeof(Student));
	if (newStudent == NULL) {
		printf("ERROR! Could not allocate memmory!\n");
		return NULL;
	}
	Date date = { 0, 0, 0 };
	strcpy(newStudent->ID, id);
	strcpy(newStudent->firstName, fname);
	strcpy(newStudent->lastName, lname);
	for (int i = 0; i < 2; i++) {
		newStudent->grade[i] = 1;
		newStudent->date[i] = date;
	}
	newStudent->next = NULL;

	return newStudent;
}

int loadFromFile(StudentP head, const char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("ERROR! Could not allocate memmory!\n");
		return EXIT_FAILURE;
	}
	char fname[MAX_NAME_LENGTH], lname[MAX_NAME_LENGTH], id[10];
	while (fscanf(fp, "%s %s %s", id, fname, lname) == 3) {
		StudentP newStudent = createNewStudent(id, fname, lname);
		if (newStudent == NULL) {
			printf("ERROR! Could not allocate memmory!\n");
			return EXIT_FAILURE;
		}
		insertSorted(head, newStudent);
	}
	fclose(fp);

	return EXIT_SUCCESS;
}

int insertSorted(StudentP head, StudentP newStudent) {
	StudentP prev = head;
	StudentP curr = head->next;
	while (curr) {
		if (strcmp(newStudent->lastName, curr->lastName) < 0) {
			break;
		}
		else {
			if ((strcmp(newStudent->lastName, curr->lastName) == 0) && (strcmp(newStudent->firstName, curr->firstName) <= 0)) {
				break;
			}
			else {
				prev = curr;
				curr = curr->next;
			}
		}
	}
	prev->next = newStudent;
	newStudent->next = curr;

	return EXIT_SUCCESS;
}

int loadGrades(StudentP head, const char* fileName, int index) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("ERROR! Could not allocate memmory!\n");
		return EXIT_FAILURE;
	}
	char id[10];
	int grade, d, m, y;
	while (fscanf(fp, "%s %d %d.%d.%d", id, &grade, &y, &m, &d) == 5) {
		Date date = { y, m, d };
		insertGradeAndDate(head, id, grade, index, date);
	}
	fclose(fp);

	return EXIT_SUCCESS;
}

int insertGradeAndDate(StudentP head, char* id, int grade, int index, Date date) {
	StudentP temp = head;
	while (temp) {
		if (strcmp(temp->ID, id) == 0) {
			temp->grade[index] = grade;
			temp->date[index] = date;
			return EXIT_SUCCESS;
		}
		else {
			temp = temp->next;
		}
	}

	return EXIT_SUCCESS;
}

int printList(StudentP first) {
	StudentP temp = first;
	while (temp) {
		printf("ID: %s, Osoba: %s %s\n", temp->ID, temp->firstName, temp->lastName);
		for (int i = 0; i < 2; i++) {
			printf("Ocjena: %d, Datum: %d.%d.%d\n", temp->grade[i], temp->date[i].day, temp->date[i].month, temp->date[i].year);
		}
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}

int freeList(StudentP head) {
	StudentP temp = head->next;
	while(temp) {
		StudentP toFree = temp;
		temp = temp->next;
		free(toFree);
	}
	head->next = NULL;

	return EXIT_SUCCESS;
}

int deleteByDate(StudentP head, Date date) {
	StudentP prev = head;
	StudentP curr = head->next;
	while (curr) {
		Date date1 = curr->date[0];
		Date date2 = curr->date[1];
		if ((date1.year == date.year && date1.month == date.month && date1.day == date.day) ||
			(date2.year == date.year && date2.month == date.month && date2.day == date.day)) {
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
