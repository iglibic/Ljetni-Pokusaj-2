/*Napisati program koji iz datoteke "studenti.txt" čita prezime i
ime studenta kao i njegov ID, te kreira vezanu listu sortiranu
po prezimenu i imenu studenta.
Nakon toga iz datoteka "predmet1.txt" i "predmet2.txt" se čita
ID studenta, ocjena i datum polaganja predmeta. Pročitane
podatke je potrebno unijeti u postojeću vezanu listu. Ispisati
listu.*/

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

StudentP createNewStudent(StudentP head, char* id, char* fname, char* lname, int grade[2], Date dat[2]);
int loadFromData(StudentP head, char* fileName);
int insertSorted(StudentP head, StudentP student);
int loadFromFile1(StudentP head, char* fileName);
int loadFromFile2(StudentP head, char* fileName);
int printList(StudentP first);
int freeList(StudentP head);

int main() {
	StudentP head = malloc(sizeof(Student));
	if (head == NULL) {
		printf("ERROR! Could not allocate the memmory!\n");
		return EXIT_FAILURE;
	}
	head->next = NULL;

	loadFromData(head, "studentii.txt");
	loadFromFile1(head, "predmet1.txt");
	loadFromFile2(head, "predmet2.txt");

	printList(head->next);

	freeList(head);

	return EXIT_SUCCESS;
}

StudentP createNewStudent(StudentP head, char* id, char* fname, char* lname, int grade[2], Date date[2]) {
	StudentP newStudent = malloc(sizeof(Student));
	if (newStudent == NULL) {
		printf("ERROR! Could not allocate the memmory!\n");
		return NULL;
	}
	strcpy(newStudent->ID, id);
	strcpy(newStudent->firstName, fname);
	strcpy(newStudent->lastName, lname);
	
	for (int i = 0; i < 2; i++) {
		newStudent->grade[i] = grade[i];                 //ovako kad imas niz
		newStudent->date[i] = date[i];					 //isto i za datume
	}

	return newStudent;
}

int loadFromData(StudentP head, char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("ERROR! Could not open the file!\n");
		return EXIT_FAILURE;
	}

	char id[10], fname[MAX_NAME_LENGTH], lname[MAX_NAME_LENGTH];

	while (fscanf(fp, "%s %s %s", id, fname, lname) == 3) {
		int grade[2] = {0,0};
		Date date[2] = { {0,0,0}, {0,0,0} };

		StudentP newStudent = createNewStudent(head, id, fname, lname, grade, date);
		if (newStudent == NULL) {
			printf("ERROR! Could not allocate the memmory!\n");
			return EXIT_FAILURE;
		}
		insertSorted(head, newStudent);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}

int insertSorted(StudentP head, StudentP student) {							//pogledati ovaj sort (PREZIME i IME	)
	StudentP prev = head;
	StudentP curr = head->next;
	while (curr) {
		if (strcmp(student->lastName, curr->lastName) < 0) {
			break;
		}
		else if (strcmp(student->lastName, curr->lastName) == 0) {
			if (strcmp(student->firstName, curr->firstName) <= 0) {
				break;
			}
		}
		prev = curr;
		curr = curr->next;
	}

	student->next = curr;
	prev->next = student;

	return EXIT_SUCCESS;
}

int loadFromFile1(StudentP head, char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("ERROR! Could not open the file!\n");
		return EXIT_FAILURE;
	}
	char id[10];
	int grade, d, m, y;

	while (fscanf(fp, "%s %d %d.%d.%d", id, &grade, &y, &m, &d) == 5) {
		StudentP temp = head->next;
		Date date = { y, m, d };
		while (temp) {
			if (strcmp(temp->ID, id) == 0) {
				temp->grade[0] = grade;
				temp->date[0] = date;
				break;
			}
			temp = temp->next;
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

int loadFromFile2(StudentP head, char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("ERROR! Could not open the file!\n");
		return EXIT_FAILURE;
	}
	char id[10];
	int grade, d, m, y;

	while (fscanf(fp, "%s %d %d.%d.%d", id, &grade, &y, &m, &d) == 5) {
		StudentP temp = head->next;
		Date date = { y, m, d };
		while (temp) {
			if (strcmp(temp->ID, id) == 0) {
				temp->grade[1] = grade;
				temp->date[1] = date;
				break;
			}
			temp = temp->next;
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}

int printList(StudentP first) {
	StudentP temp = first;
	while (temp) {
		printf("ID: %s\n", temp->ID);
		printf("First name: %s\n", temp->firstName);
		printf("Last name: %s\n", temp->lastName);

		for (int i = 0; i < 2; i++) {																	//poseban print za nizove!?
			if (temp->grade[i] != 0) {
				printf("Predmet %d - Ocjena %d, Datum: %d.%d.%d\n", i + 1, temp->grade[i],
					temp->date[i].day, temp->date[i].month, temp->date[i].year);
			}
			else {
				printf("Predmet %d - Ocjena nije upisana!\n", i + 1);
			}
		}
		printf("\n");
		temp = temp->next;
	}
	return EXIT_SUCCESS;
}

int freeList(StudentP head) {
	StudentP temp = head->next;
	while (temp) {
		StudentP toFree = temp;
		temp = temp->next;
		free(toFree);
	}
	head->next = NULL;

	return EXIT_SUCCESS;
}
