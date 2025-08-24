/*Napisati program koji stvara dvije vezane liste, svaku od 20 elemenata, na način da se za svaki element 
liste generiraju dvije vrijednosti. Prva je vrijednost u opsegu od 100–130 (i u listi se ne smiju ponavljati 
vrijednosti), a druga je prioritet u opsegu od 1–5 (bit će ponavljanja). Elementi u listi moraju biti 
sortirani prvo po prioritetu, pa po vrijednosti.

Ocjena 2:
Ispisati vezanu listu i pronaći vrijednosti koje se ponavljaju u obje liste (uspoređuje se po value 
elementu), te ispisati te vrijednosti uz njihov prioritet i nakon toga ih izbrisati iz liste.
Ispisati nove liste.*/

#define _CRT_SECURE_NOWARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct _data* DataP;
typedef struct _data {
    int value;
    int priority;
    DataP next;
} Data;

DataP createNewData(int val, int pr);
int generateUniqueValue(int usedValues[]);
int generatePriority();
int insertSorted(DataP head, int value, int priority);
int printList(DataP first);
int freeList(DataP head);

int findSameValue(DataP head1, DataP head2);
int deleteSameValue(DataP head, int value);

int main() {
    srand((unsigned int)time(NULL));

    DataP head1 = (DataP)malloc(sizeof(Data));
    DataP head2 = (DataP)malloc(sizeof(Data));

    if (head1 == NULL || head2 == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    head1->next = NULL;
    head2->next = NULL;

    int usedValues1[31] = { 0 };
    int usedValues2[31] = { 0 };

    for (int i = 0; i < 20; i++) {
        insertSorted(head1, generateUniqueValue(usedValues1), generatePriority());
        insertSorted(head2, generateUniqueValue(usedValues2), generatePriority());
    }

    printf("Prva lista: \n");
    printList(head1->next);

    printf("\nDruga lista: \n");
    printList(head2->next);

    printf("Trazim vrijednosti koje su se pojavile u obe liste...\n");
    findSameValue(head1, head2);

    printf("Konacna prva lista: \n");
    printList(head1->next);

    printf("\nKonacna druga lista: \n");
    printList(head2->next);

    freeList(head1);
    freeList(head2);

    return EXIT_SUCCESS;
}

DataP createNewData(int val, int pr) {
    DataP newData = (DataP)malloc(sizeof(Data));
    if (newData == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return NULL;
    }
    newData->value = val;
    newData->priority = pr;
    newData->next = NULL;

    return newData;
}

int generateUniqueValue(int usedValues[]) {
    int value;
    do {
        value = rand() % 31 + 100;
    } while (usedValues[value - 100]);
    usedValues[value - 100] = 1;
    return value;
}

int generatePriority() {
    return rand() % 5 + 1;
}

int insertSorted(DataP head, int value, int priority) {
    DataP prev = head;
    DataP curr = head->next;
    while (curr) {
        if (priority < curr->priority) {
            break;
        }
        else {
            if ((priority == curr->priority) && (value <= curr->value)) {
                break;
            }
            else {
                prev = curr;
                curr = curr->next;
            }
        }
    }
    DataP newData = createNewData(value, priority);
    if (newData == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    prev->next = newData;
    newData->next = curr;

    return EXIT_SUCCESS;
}

int printList(DataP first) {
    DataP temp = first;
    while (temp) {
        printf("Vrijednost: %d, Prioritet: %d\n", temp->value, temp->priority);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int freeList(DataP head) {
    DataP temp = head->next;
    while (temp) {
        DataP toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    head->next = NULL;

    return EXIT_SUCCESS;
}

int findSameValue(DataP head1, DataP head2) {
    DataP temp1 = head1->next;
    while (temp1) {
        DataP temp2 = head2->next;
        while (temp2) {
            if (temp1->value == temp2->value) {
                printf("Vrijednost: %d se ponavalja u obe liste.\n", temp1->value);
                deleteSameValue(head1, temp1->value);
                deleteSameValue(head2, temp2->value);
                temp1 = head1;                              //obrises temp1 i ne postoji ti pa ga treba opet inicijalizirati
                                                            //temp2 ne treba jer u novom krugu whilea ide nova vrijednost
                break;                                      //trenutni while prekines jer si nasao tu vrijednost i ides dalje
            }
            else {
                temp2 = temp2->next;
            }
        }
        temp1 = temp1->next;
    }
    return EXIT_SUCCESS;
}

int deleteSameValue(DataP head, int value) {
    DataP prev = head;
    DataP curr = head->next;

    while (curr) {
        if (curr->value == value) {
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
