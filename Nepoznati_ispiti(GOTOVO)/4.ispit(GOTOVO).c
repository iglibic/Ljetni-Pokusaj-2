/*napisati program koji iz datoteke"clanovi.txt" cita podatke o clanovima knjiznice,
a iz datoteke "knjige.txt" cita podatke o posudenim knjigama.
podatci o clanovima knjiznice trebaju se spremiti u jednostruku vezanu listu sortiranu po prezimenu, pa po imenu, zatim po IDu
podaci o posudenim knjigama trebaju se spremiti u jednostruku vezanu listu sortiranu po nazivu knjnige.
za 2- pronaci clana knjiznice koji je procitao najvise knjiga te ispisati njegovo ime i prezime.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NOT_OPENED (-1)
#define MAX 32

typedef struct _clan* ClanP;
typedef struct _clan {
    int id;
    char firstName[MAX];
    char lastName[MAX];
    int readBooks;
    ClanP next;
} Clan;

typedef struct knjiga* KnjigaP;
typedef struct knjiga {
    char name[MAX];
    int id_clana;
    KnjigaP next;
} Knjiga;

ClanP createNewMember(char* fName, char* lName, int id, int readBooks);
int loadFromFileM(ClanP head, const char* fileName);
int insertSortedM(ClanP head, ClanP newMember);
KnjigaP createNewBook(char* name, int id);
int loadFromFileB(KnjigaP head, const char* fileName);
int insertSortedB(KnjigaP head, KnjigaP newBook);
int freeListM(ClanP head);
int freeListB(KnjigaP head);

int loadReadBooks(ClanP headM, KnjigaP headB);
int printMostBooksRead(ClanP head);

int main() {
    ClanP headM = (ClanP)malloc(sizeof(Clan));
    if (headM == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    headM->next = NULL;

    KnjigaP headB = (KnjigaP)malloc(sizeof(Knjiga));
    if (headB == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    headB->next = NULL;

    loadFromFileM(headM, "CLANOVI.txt");
    loadFromFileB(headB, "KNJIGE.txt");
    
    loadReadBooks(headM, headB);
    printMostBooksRead(headM);

    freeListM(headM);
    freeListB(headB);

    return EXIT_SUCCESS;
}

ClanP createNewMember(char* fName, char* lName, int id, int readBooks) {
    ClanP newMember = (ClanP)malloc(sizeof(Clan));
    if (newMember == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return NULL;
    }
    strcpy(newMember->firstName, fName);
    strcpy(newMember->lastName, lName);
    newMember->id = id;
    newMember->readBooks = readBooks;
    newMember->next = NULL;

    return newMember;
}

int loadFromFileM(ClanP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("ERROR! Could not open the file!\n");
        exit(EXIT_FAILURE);
    }
    char fname[MAX], lname[MAX];
    int id;
    while (fscanf(fp, "%d %s %s", &id, fname, lname) == 3) {
        ClanP newMember = createNewMember(fname, lname, id, 0);
        if (newMember == NULL) {
            printf("ERROR! Could not allocate memmory!\n");
            return EXIT_FAILURE;
        }
        insertSortedM(head, newMember);
    }
    fclose(fp);

    return EXIT_SUCCESS;
}

int insertSortedM(ClanP head, ClanP newMember) {
    ClanP prev = head;
    ClanP curr = head->next;
    while (curr) {
        if (strcmp(newMember->lastName, curr->lastName) < 0) {
            break;
        }
        else {
            if ((strcmp(newMember->lastName, curr->lastName) == 0) && (strcmp(newMember->firstName, curr->firstName) < 0)) {
                break;
            }
            else {
                if ((strcmp(newMember->lastName, curr->lastName) == 0) && (strcmp(newMember->firstName, curr->firstName) == 0) && (newMember->id <= curr->id)) {
                    break;
                }
                else {
                    prev = curr;
                    curr = curr->next;
                }
            }
        }
    }
    prev->next = newMember;
    newMember->next = curr;

    return EXIT_SUCCESS;
}

KnjigaP createNewBook(char* name, int id) {
    KnjigaP newBook = (KnjigaP)malloc(sizeof(Knjiga));
    if (newBook == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return NULL;
    }
    strcpy(newBook->name, name);
    newBook->id_clana = id;
    newBook->next = NULL;

    return newBook;
}

int loadFromFileB(KnjigaP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("ERROR! Could not open the file!\n");
        exit(EXIT_FAILURE);
    }
    char name[MAX];
    int id;
    while (fscanf(fp, "%s %d", name, &id) == 2) {
        KnjigaP newBook = createNewBook(name, id);
        if (newBook == NULL) {
            printf("ERROR! Could not allocate memmory!\n");
            return EXIT_FAILURE;
        }
        insertSortedB(head, newBook);
    }
    fclose(fp);

    return EXIT_SUCCESS;
}

int insertSortedB(KnjigaP head, KnjigaP newBook) {
    KnjigaP prev = head;
    KnjigaP curr = head->next;
    while (curr) {
        if (strcmp(newBook->name, curr->name) <= 0) {
            break;
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
    prev->next = newBook;
    newBook->next = curr;

    return EXIT_SUCCESS;
}

int loadReadBooks(ClanP headM, KnjigaP headB) {
    ClanP temp1 = headM->next;
    while (temp1) {
        KnjigaP temp2 = headB->next;
        while (temp2) {
            if (temp1->id == temp2->id_clana) {
                temp1->readBooks++;
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
    return EXIT_SUCCESS;
}

int printMostBooksRead(ClanP head) {
    ClanP temp = head->next;
    int most = 0;
    while (temp) {
        if (temp->readBooks > most) {
            most = temp->readBooks;
        }
        temp = temp->next;
    }

    ClanP curr = head->next;
    while (curr) {
        if (curr->readBooks == most) {
            printf("Osoba s najvise procitanih knjiga (%d) je: %s %s\n", most, curr->firstName, curr->lastName);
        }
        curr = curr->next;
    }
    return EXIT_SUCCESS;
}

int freeListM(ClanP head) {
    ClanP temp = head->next;
    while (temp) {
        ClanP toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    head->next = NULL;
    return EXIT_SUCCESS;
}

int freeListB(KnjigaP head) {
    KnjigaP temp = head->next;
    while (temp) {
        KnjigaP toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    head->next = NULL;
    return EXIT_SUCCESS;
}
