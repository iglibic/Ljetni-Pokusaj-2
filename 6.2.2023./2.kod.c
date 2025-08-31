// Napisati program koji za tri različita predmeta iz datoteka čita
// podatke o studentima koji su položili predmet i gradi tri vezane
// liste. Svaki predmet ima svoju datoteku (Subject1.txt,
// Subject2.txt, Subject3.txt).

// OCJENA 2:
// Iz tri gornje vezane liste napraviti novu na način da za svakog
// studenta postoji samo jedan element u listi, tj. ukoliko je
// student položio više predmeta treba grupirati njegove ocjene u 
// jedan element liste. Ukoliko student nije položio neki predmet,
// tada je njegova ocjena iz tog predmeta 1. Ispisati listu u
// formatu:
//      - lastName firstName (Subject1, Subject2, Subject3)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN (32)
#define MAX_SUBJECTS (3)

struct student;
typedef struct _student* StudentP;
typedef struct _student {
    char firstName[MAX_NAME_LEN];
    char lastName[MAX_NAME_LEN];
    int grades[MAX_SUBJECTS];
    StudentP next;
} Student;

StudentP createNewStudent(char* fName, char* lName, int grade[3]);
int loadFromFile(StudentP head, const char* fileName, int index);
int addNewStudent(StudentP head, StudentP newStudent, int index);
int printList(StudentP first);
int freeList(StudentP head);

int main() {
    StudentP head = (StudentP)malloc(sizeof(Student));
    if (head == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    head->next = NULL;

    loadFromFile(head, "Subject1.txt", 0);
    loadFromFile(head, "Subject2.txt", 1);
    loadFromFile(head, "Subject3.txt", 2);

    printf("Lista: \n");
    printList(head->next);

    freeList(head);

    return EXIT_SUCCESS;
}

StudentP createNewStudent(char* fName, char* lName, int grade[3]) {
    StudentP newStudnet = (StudentP)malloc(sizeof(Student));
    if (newStudnet == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return NULL;
    }
    strcpy(newStudnet->firstName, fName);
    strcpy(newStudnet->lastName, lName);
    for (int i = 0; i < 3; i++) {
        newStudnet->grades[i] = grade[i];
    }
    newStudnet->next = NULL;

    return newStudnet;
}

int loadFromFile(StudentP head, const char* fileName, int index) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("ERROR! Could not open the file!\n");
        return EXIT_FAILURE;
    }
    char fname[MAX_NAME_LEN], lname[MAX_NAME_LEN];
    int grade;
    while (fscanf(fp, "%s %s %d", lname, fname, &grade) == 3) {
        int grades[3] = { 1, 1, 1 };
        grades[index] = grade;
        StudentP newStudent = createNewStudent(fname, lname, grades);
        if (newStudent == NULL) {
            printf("ERROR! Could not allocate memmory!\n");
            return EXIT_FAILURE;
        }
        addNewStudent(head, newStudent, index);
    }
    fclose(fp);

    return EXIT_SUCCESS;
}

int addNewStudent(StudentP head, StudentP newStudent, int index) {
    if (head->next == NULL) {
        head->next = newStudent;
        return EXIT_SUCCESS;
    }
    StudentP temp = head->next;
    while (temp->next) {
        temp = temp->next;
    }

    StudentP curr = head->next;
    while (curr) {
        if (strcmp(newStudent->firstName, curr->firstName) == 0 && strcmp(newStudent->lastName, curr->lastName) == 0) {
            curr->grades[index] = newStudent->grades[index];
            free(newStudent);
            return EXIT_SUCCESS;
        }
        else {
            curr = curr->next;
        }
    }
    temp->next = newStudent;

    return EXIT_SUCCESS;
}

int printList(StudentP first) {
    StudentP temp = first;
    while (temp) {
        printf(" -  %s %s (%d, %d, %d)\n", temp->lastName, temp->firstName, temp->grades[0], temp->grades[1], temp->grades[2]);
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
