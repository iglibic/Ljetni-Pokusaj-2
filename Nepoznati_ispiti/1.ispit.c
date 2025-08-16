/*Napisati program koji generira 15 slučajnih brojeva u opsegu od 100 - 120 i sprema ih u
vezanu listu, po redoslijedu generiranja, a ne sortirano. U listi ne smije biti duplih vrijednosti.
a) Potrebno je pronaći minimalnu i maksimalnu vrijednost, te iz njih izračunati srednju
vrijednost
((min+max)/2) i nakon toga sve vrijednosti koje su veće od srednje prebaciti na kraj liste.
Ispisati
minimalnu, maksimalnu i srednju vrijednost kao i rezultantnu listu*/

#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NOT_OPENED (-1)

typedef struct lista* ListaP;
typedef struct lista {
    int el;
    ListaP next;
} Lista;

int generateUniqueNumb(int usedNumbers[]);
ListaP createNewElement(int el);
int addNewElement(ListaP head, int el);
int printList(ListaP first);
int freeList(ListaP head);

int findMin(ListaP head);
int findMax(ListaP head);
ListaP findLast(ListaP head);
int moveAboveMid(ListaP head, float mid);

int main() {
    srand((unsigned)time(NULL));

    ListaP head = (ListaP)malloc(sizeof(Lista));
    if (head == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return EXIT_FAILURE;
    }
    head->next = NULL;

    int usedNumbers[15] = { 0 };

    for (int i = 0; i < 15; i++) {
        addNewElement(head, generateUniqueNumb(usedNumbers));
    }

    printList(head->next);

    int min = findMin(head->next);
    int max = findMax(head->next);

    printf("\nMinimalni element: %d\n", min);
    printf("Maksimalni element: %d\n", max);

    float srVr = (float)(min + max) / 2;

    printf("Srednja vrijednost je: %f\n", srVr);

    moveAboveMid(head, srVr);

    printList(head->next);

    freeList(head);

    return EXIT_SUCCESS;
}

int generateUniqueNumb(int usedNumbers[]) {
    int value;
    do {
        value = rand() % 21 + 100;
    } while (usedNumbers[value - 100]);
    usedNumbers[value - 100] = 1;

    return value;
}

ListaP createNewElement(int el) {
    ListaP newElement = (ListaP)malloc(sizeof(Lista));
    if (newElement == NULL) {
        printf("ERROR! Could not allocate memmory!\n");
        return NULL;
    }
    newElement->el = el;
    newElement->next = NULL;

    return newElement;
}

int addNewElement(ListaP head, int el) {
    ListaP temp = head;

    while (temp->next) {
        temp = temp->next;
    }

    ListaP newElement = createNewElement(el);

    temp->next = newElement;

    return EXIT_SUCCESS;
}

int printList(ListaP first) {
    ListaP temp = first;
    while (temp) {
        printf("Element: %d\n", temp->el);
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

int findMin(ListaP head) {
    ListaP temp = head;
    int min = 300;
    while (temp) {
        if (temp->el < min) {
            min = temp->el;
        }
        temp = temp->next;
    }

    return min;
}

int findMax(ListaP head) {
    ListaP temp = head;
    int max = 10;
    while (temp) {
        if (temp->el > max) {
            max = temp->el;
        }
        temp = temp->next;
    }

    return max;
}

ListaP findLast(ListaP head) {
    ListaP temp = head;
    while (temp->next) {
        temp = temp->next;
    }
    return temp;
}

int moveAboveMid(ListaP head, float mid) {
    ListaP prev = head;
    ListaP curr = head->next;            
    ListaP last = findLast(head);

    ListaP originalLast = last;

    while (curr != originalLast) {
        if (curr->el > mid) {
            prev->next = curr->next;
            last->next = curr;
            curr->next = NULL;
            last = curr;
            curr = prev->next;
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }

    return EXIT_SUCCESS;
}
