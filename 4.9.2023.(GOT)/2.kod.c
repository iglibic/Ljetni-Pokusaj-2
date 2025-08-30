// Napisati program koji iz datoteke „riječi.txt“ čita riječi i slaže ih po redoslijedu pojavljivanja. Pri tome
// ukoliko riječ već postoji u vezanoj listi, ne treba je ponovo dodavati u listu, već se uvećava brojač.

// OCJENA 2:
// Ispisati vezanu listu i pronaći pet riječi koje su se pojavile najviše puta, te ih pomaknuti na početak 
// liste.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NOT_OPENED (-1)
#define MAX_NAME 32

typedef struct _rijeci* RijeciP;
typedef struct _rijeci {
    char word[MAX_NAME];
    int nORepeats;
    RijeciP next;
} Rijeci;

RijeciP createNewWord(char* word, int numOfRepeats);
int loadFromFile(RijeciP head, const char* fileName);
int addNew(RijeciP head, char* word);
int printList(RijeciP first);
int freeList(RijeciP head);

int printMostReps(RijeciP head);

int main() {
    RijeciP head = (RijeciP)malloc(sizeof(Rijeci));
    if (head == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    head->next = NULL;

    loadFromFile(head, "RIJECI.txt");

    printf("Pocetna lista: \n");
    printList(head->next);

    printf("\n5 rijeci s najvecim brojem ponavljanja: \n");
    printMostReps(head);

    freeList(head);

    return EXIT_SUCCESS;
}

RijeciP createNewWord(char* word, int numOfRepeats) {
    RijeciP newWord = (RijeciP)malloc(sizeof(Rijeci));
    if (newWord == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return NULL;
    }
    strcpy(newWord->word, word);
    newWord->nORepeats = numOfRepeats;
    newWord->next = NULL;

    return newWord;
}

int loadFromFile(RijeciP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("ERROR! Could not open the file!\n");
        exit(EXIT_FAILURE);
    }
    char word[MAX_NAME];
    while (fscanf(fp, "%s", word) == 1) {
        addNew(head, word);
    }
    fclose(fp);

    return EXIT_SUCCESS;
}

int addNew(RijeciP head, char* word) { 
    RijeciP prev = head;                                                        //cgpt al sam i sam morao ovako...
    RijeciP temp = head->next;

    while (temp) {
        if (strcmp(temp->word, word) == 0) {
            temp->nORepeats++;
            return EXIT_SUCCESS;
        }
        prev = temp;
        temp = temp->next;
    }

    RijeciP newWord = createNewWord(word, 1);
    prev->next = newWord;

    return EXIT_SUCCESS;
}


int printList(RijeciP first) {
    RijeciP temp = first;
    while (temp) {
        printf("Rijec: %s, Broj ponavljanja: %d\n", temp->word, temp->nORepeats);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int freeList(RijeciP head) {
    RijeciP temp = head->next;
    while (temp) {
        RijeciP toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    head->next = NULL;
    
    return EXIT_SUCCESS;
}

int printMostReps(RijeciP head) {                                           //cgpt ugl s mojom izvornom idejom, njegov je printed
    RijeciP temp = head->next;
    int maxreps = 0;

    while (temp) {
        if (temp->nORepeats > maxreps) {
            maxreps = temp->nORepeats;
        }
        temp = temp->next;
    }

    int printed = 0;
    while(printed < 5 && maxreps > 0){
        RijeciP curr = head->next;
        while (curr) {
            if (curr->nORepeats == maxreps) {
                printf("Rijec: %s, Broj ponavljanja: %d\n", curr->word, curr->nORepeats);
                printed++;
                if (printed == 5) {
                    return EXIT_SUCCESS;
                }
            }
            curr = curr->next;
        }
        maxreps--;
    }
    return EXIT_SUCCESS;
}
