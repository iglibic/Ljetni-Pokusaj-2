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

typedef struct lista* ListaP;
typedef struct lista {
    char rijec[MAX];
    ListaP next;
} Lista;

ListaP createNewWord(char* word);
int loadFromFile(ListaP head, const char* fileName);
int insertSorted(ListaP head, ListaP newWord);
int printList(ListaP first);
int freeList(ListaP head);

int printSameWords(ListaP head, const char* fileName);

int main() {
    ListaP head = (ListaP)malloc(sizeof(Lista));
    if (head == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    head->next = NULL;

    loadFromFile(head, "RIJECI.txt");

    printf("Pocetna lista: \n");
    printList(head->next);

    printf("\nKonacna lista: \n");
    printSameWords(head, "RIJECI2.txt");

    freeList(head);

    return EXIT_SUCCESS;
}

ListaP createNewWord(char* word) {
    ListaP newWord = (ListaP)malloc(sizeof(Lista));
    if (newWord == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return NULL;
    }
    strcpy(newWord->rijec, word);
    newWord->next = NULL;

    return newWord;
}

int loadFromFile(ListaP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("ERROR! Could not open the file!\n");
        return EXIT_FAILURE;
    }
    char word[MAX];

    while (fscanf(fp, "%s", word) == 1) {
        ListaP newWord = createNewWord(word);
        if (newWord == NULL) {
            printf("ERROR! Could not allocate memmory!\n");
            return EXIT_FAILURE;
        }
        insertSorted(head, newWord);
    }
    fclose(fp);

    return EXIT_SUCCESS;
}

int insertSorted(ListaP head, ListaP newWord) {
    ListaP prev = head;
    ListaP curr = head->next;
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

int printList(ListaP first) {
    ListaP temp = first;
    while (temp) {
        printf("Rijec: %s\n", temp->rijec);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int freeList(ListaP head) {
    ListaP temp = head->next;
    while (temp) {
        ListaP toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    head->next = NULL;
    return EXIT_SUCCESS;
}

int printSameWords(ListaP head, const char* fileName) {
    ListaP temp1 = head->next;
    while (temp1) {

        FILE* fp = fopen(fileName, "r");
        if (fp == NULL) {
            printf("ERROR! Could not open the file!\n");
            return EXIT_FAILURE;
        }
        char word[MAX];

        while (fscanf(fp, "%s", word) == 1) {
            if (strcmp(temp1->rijec, word) == 0) {
                printf("Rijec: %s\n", word);
                break;
            }
        }
        fclose(fp);

        temp1 = temp1->next;
    }
    return EXIT_SUCCESS;
}
