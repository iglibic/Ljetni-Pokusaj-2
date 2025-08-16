/*Napisati program kojim se
a) Iz datoteke čitaju riječi i spremaju u vezanu listu na način da se riječi slažu po abecednom redu 
obrnutom od abecednog reda. Zatim ispisati tu vezanu listu.
b) Iz generirane liste ispisati samo one riječi koje se nalaze zapisane u drugoj proizvoljnoj datoteci.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NOT_OPENED (-1)
#define MAX 50

typedef struct lista* listaP;
typedef struct lista {
    char rijec[MAX];
    listaP next;
} Lista;

listaP createNewWord(char* word);
int loadFromFile(listaP head, const char* fileName);
int insertSorted(listaP head, listaP newWord);
int printList(listaP first);
int freeList(listaP head);

int printSameWords(listaP head, const char* fileName);

int main() {
    listaP head = (listaP)malloc(sizeof(Lista));
    if (head == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    head->next = NULL;

    loadFromFile(head, "RIJECI.txt");

    printList(head->next);

    printf("\n");

    printSameWords(head, "RIJECI2.txt");

    freeList(head);

    return EXIT_SUCCESS;
}

listaP createNewWord(char* word) {
    listaP newWord = (listaP)malloc(sizeof(Lista));
    if (newWord == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return NULL;
    }
    strcpy(newWord->rijec, word);
    newWord->next = NULL;

    return newWord;
}

int loadFromFile(listaP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("ERROR! Could not open the file!\n");
        return FILE_NOT_OPENED;
    }
    char word[MAX];

    while (fscanf(fp, "%s", word) == 1) {
        listaP newWord = createNewWord(word);
        if (newWord == NULL) {
            printf("ERROR! Could not allocate memmory!\n");
            return EXIT_FAILURE;
        }
        insertSorted(head, newWord);
    }
    fclose(fp);

    return EXIT_SUCCESS;
}

int insertSorted(listaP head, listaP newWord) {
    listaP prev = head;
    listaP curr = head->next;

    while (curr) {
        if (strcmp(newWord->rijec, curr->rijec) > 0) {
            break;
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
    prev->next = newWord;
    newWord->next = curr;

    return EXIT_SUCCESS;
}

int printList(listaP first) {
    listaP temp = first;
    while (temp) {
        printf("Rijec: %s\n", temp->rijec);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int freeList(listaP head) {
    listaP temp = head->next;
    while (temp) {
        listaP toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    head->next = NULL;

    return EXIT_SUCCESS;
}

int printSameWords(listaP head, const char* fileName) {
    
    char word[MAX];

    listaP temp = head->next;

    while (temp) {
        FILE* fp = fopen(fileName, "r");
        if (fp == NULL) {
            printf("ERROR! Could not open the file!\n");
            return FILE_NOT_OPENED;
        }

        while (fscanf(fp, "%s", word) == 1) {
            if (strcmp(temp->rijec, word) == 0) {
                printf("Rijec %s se ponavlja!\n", temp->rijec);
                break;
            }
        }
        fclose(fp);
        temp = temp->next;
    }

    return EXIT_SUCCESS;
}
