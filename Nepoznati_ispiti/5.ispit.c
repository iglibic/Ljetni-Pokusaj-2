/*Napisati program koji iz datoteke "clanovi.txt" cita podatke o clanovima knjiznice,
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

ClanP createNewMember(int iD, char* fname, char* lname, int ReadBooks);
int loadFromFileM(ClanP head, const char* fileName);
int insertSortedM(ClanP head, ClanP newMember);

KnjigaP createNewBook(char* name, int id);
int loadFromFileB(KnjigaP head, const char* fileName);
int insertSortedB(KnjigaP head, KnjigaP newBook);

int freeListM(ClanP head);
int freeListB(KnjigaP head);

int booksRead(ClanP head1, KnjigaP head2);
int printMostBooksRead(ClanP head);

int main() {
    ClanP head1 = (ClanP)malloc(sizeof(Clan));
    if (head1 == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    head1->next = NULL;

    loadFromFileM(head1, "CLANOVI.txt");

    KnjigaP head2 = (KnjigaP)malloc(sizeof(Knjiga));
    if (head2 == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    head2->next = NULL;

    loadFromFileB(head2, "KNJIGE.txt");

    booksRead(head1, head2);

    printMostBooksRead(head1);

    freeListM(head1);
    freeListB(head2);

    return EXIT_SUCCESS;
}

ClanP createNewMember(int iD, char* fname, char* lname, int ReadBooks) {
    ClanP newMember = (ClanP)malloc(sizeof(Clan));
    if (newMember == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return NULL;
    }
    newMember->id = iD;
    strcpy(newMember->firstName, fname);
    strcpy(newMember->lastName, lname);
    newMember->readBooks = ReadBooks;
    newMember->next = NULL;

    return newMember;
}

int loadFromFileM(ClanP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("ERROR! Could not open the file!\n");
        return FILE_NOT_OPENED;
    }

    int id;
    char fName[MAX], lName[MAX];

    while (fscanf(fp, "%d %s %s", &id, fName, lName) == 3) {
        ClanP newMember = createNewMember(id, fName, lName, 0);
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
        else if (strcmp(newMember->lastName, curr->lastName) == 0) {
            if (strcmp(newMember->firstName, curr->firstName) < 0) {
                break;
            }
            else if (strcmp(newMember->firstName, curr->firstName) == 0) {
                if (newMember->id <= curr->id) {
                    break;
                }
                else {
                    prev = curr;
                    curr = curr->next;
                }
            }
            else {
                prev = curr;
                curr = curr->next;
            }
        }
        else {
            prev = curr;
            curr = curr->next;
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
        return FILE_NOT_OPENED;
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
        if (strcmp(newBook->name, curr->name) < 0) {
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

int booksRead(ClanP head1, KnjigaP head2) {
    ClanP curr = head1->next;

    while (curr) {
        KnjigaP temp = head2->next;

        while (temp) {
            if (curr->id == temp->id_clana) {
                curr->readBooks++;
            }
            temp = temp->next;
        }
        curr = curr->next;
    }
    return EXIT_SUCCESS;
}

int printMostBooksRead(ClanP head) {
    ClanP temp = head->next;
    int max = 0;

    while (temp) {
        if (temp->readBooks > max) {
            max = temp->readBooks;
        }
            temp = temp->next;
    }

    temp = head->next;
    while (temp) {
        if (temp->readBooks == max) {
            printf("Osoba s najviše pročitanih knjiga: %s %s", temp->firstName, temp->lastName);
            break;
        }
        else {
            temp = temp->next;
        }
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
