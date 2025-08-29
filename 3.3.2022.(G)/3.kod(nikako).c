// Napisati program koji iz datoteke membership.txt i visits.txt (imena datoteka unosi korisnik) čita podatke
// u mjesečnim članarinama korisnika i o posjetima u teretani.

// OCJENA 2: Pročitati datoteke i uvezati na način da se posjete povežu s članovima (lista u listi). Posjeta se
// veže s članarinom ako je userId isti i ako se posjeta dogodila u tom mjesecu.
// Lista članarine mora biti sortirana po godini i mjesecu. Posjete moraju biti sortirane po danu u mjesecu.
// Ispisati sve članarine i posjete u tom mjesecu.

// OCJENA 3: Prepraviti program da mjesečne članarine budu sortirane po godini, mjesecu, zatim brojem posjeta
// u tom mjesecu. Korisnika zamoli za unos mjeseca i godine, te ispisati sve članarine koje imaju najmanje
// jednu posjetu u tom mjesecu i ukupnu zaradu takvih članarina.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID_LENGTH 64
#define MAX_NAME_LENGTH 64

typedef struct _visit* VisitP;
typedef struct _visit {
    char userId[MAX_ID_LENGTH];
    int day;
    int month;
    int year;
    VisitP next;
} Visit;

typedef struct _membership* MembershipP;
typedef struct _membership {
    char userId[MAX_ID_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    int month;
    int year;
    int price;
    VisitP visits;
    MembershipP next;
} Membership;

MembershipP createNewMembership(char* id, char* fName, char* lName, int m, int y, int price);
VisitP createNewVisit(char* id, int d, int m, int y);
MembershipP findMembership(MembershipP head, char* id);
int insertSortedMembership(MembershipP head, MembershipP newMembership);
int insertSortedVisit(MembershipP membership, VisitP newVisit);
int loadMembership(MembershipP head, const char* fileName);
int loadVisit(MembershipP head, const char* fileName);
int freeVisit(VisitP head);
int freeMembership(MembershipP head);

int printMonthYearTotal(MembershipP head, int m, int y);

int countVisitsInMonth(MembershipP membership, int month, int year);

int main() {
    MembershipP head = (MembershipP)malloc(sizeof(Membership));
    if (!head) {
        printf("ERROR! Could not allocate memory!\n");
        return EXIT_FAILURE;
    }
    head->next = NULL;

    char fileName1[MAX_NAME_LENGTH], fileName2[MAX_NAME_LENGTH];

    printf("Unesite ime datoteke za clanarine: ");
    scanf("%s", fileName1);
    loadMembership(head, fileName1);

    printf("Unesite ime datoteke za posjete: ");
    scanf("%s", fileName2);
    loadVisit(head, fileName2);

    int m, y;
    printf("Upisite mjesec i godinu za ispis: ");
    scanf("%d %d", &m, &y);

    printMonthYearTotal(head, m, y);

    freeMembership(head);
    return EXIT_SUCCESS;
}

MembershipP createNewMembership(char* id, char* fName, char* lName, int m, int y, int price) {
    MembershipP newMembership = (MembershipP)malloc(sizeof(Membership));
    if (!newMembership) return NULL;

    strcpy(newMembership->userId, id);
    strcpy(newMembership->firstName, fName);
    strcpy(newMembership->lastName, lName);
    newMembership->month = m;
    newMembership->year = y;
    newMembership->price = price;
    newMembership->visits = NULL;
    newMembership->next = NULL;

    return newMembership;
}

VisitP createNewVisit(char* id, int d, int m, int y) {
    VisitP newVisit = (VisitP)malloc(sizeof(Visit));
    if (!newVisit) return NULL;

    strcpy(newVisit->userId, id);
    newVisit->day = d;
    newVisit->month = m;
    newVisit->year = y;
    newVisit->next = NULL;

    return newVisit;
}

MembershipP findMembership(MembershipP head, char* id, int month, int year) {
    MembershipP temp = head->next;
    while (temp) {
        if (strcmp(temp->userId, id) == 0 && temp->month == month && temp->year == year)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

int insertSortedMembershipByVisits(MembershipP head, MembershipP newMembership, int month, int year) {
    MembershipP prev = head;
    MembershipP curr = head->next;
    int newVisits = countVisitsInMonth(newMembership, month, year);

    while (curr) {
        int currVisits = countVisitsInMonth(curr, month, year);

        if (newMembership->year < curr->year) {
            break;
        }
        else if (newMembership->year == curr->year && newMembership->month < curr->month) {
            break;
        }
        else if (newMembership->year == curr->year && newMembership->month == curr->month && newVisits > currVisits) {
            break;
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }

    prev->next = newMembership;
    newMembership->next = curr;
    return EXIT_SUCCESS;
}

int insertSortedVisit(MembershipP membership, VisitP newVisit) {
    if (!membership || !newVisit) return EXIT_FAILURE;

    VisitP prev = NULL;
    VisitP curr = membership->visits;
    while (curr && newVisit->day > curr->day) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) {
        newVisit->next = membership->visits;
        membership->visits = newVisit;
    }
    else {
        prev->next = newVisit;
        newVisit->next = curr;
    }
    return EXIT_SUCCESS;
}

int loadMembership(MembershipP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("ERROR! Could not open the file!\n");
        return EXIT_FAILURE;
    }

    char id[MAX_ID_LENGTH], fName[MAX_NAME_LENGTH], lName[MAX_NAME_LENGTH];
    int m, y, price;
    while (fscanf(fp, "%s %s %s %d %d %d", id, fName, lName, &m, &y, &price) == 6) {
        MembershipP newMembership = createNewMembership(id, fName, lName, m, y, price);
        insertSortedMembershipByVisits(head, newMembership, m, y);
    }
    fclose(fp);
    return EXIT_SUCCESS;
}

int loadVisit(MembershipP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("ERROR! Could not open the file!\n");
        return EXIT_FAILURE;
    }

    char id[MAX_ID_LENGTH];
    int d, m, y;
    while (fscanf(fp, "%s %d %d %d", id, &d, &m, &y) == 4) {
        VisitP newVisit = createNewVisit(id, d, m, y);
        MembershipP membership = head->next;
        while (membership) {
            if (strcmp(membership->userId, id) == 0 && membership->month == m && membership->year == y) {
                insertSortedVisit(membership, newVisit);
                break;
            }
            membership = membership->next;
        }
        if (!membership) free(newVisit); // ako nema odgovarajuće članarine
    }
    fclose(fp);
    return EXIT_SUCCESS;
}

int freeVisit(VisitP head) {
    while (head) {
        VisitP temp = head;
        head = head->next;
        free(temp);
    }
    return EXIT_SUCCESS;
}

int freeMembership(MembershipP head) {
    MembershipP temp = head->next;
    while (temp) {
        freeVisit(temp->visits);
        MembershipP toFree = temp;
        temp = temp->next;
        free(toFree);
    }
    head->next = NULL;
    return EXIT_SUCCESS;
}

int printMonthYearAndTotal(MembershipP head, int month, int year) {
    MembershipP temp = head->next;
    int totalIncome = 0;
    while (temp) {
        int visits = countVisitsInMonth(temp, month, year);
        if (visits > 0) {
            printf("Osoba: %s %s, ID: %s, Iznos: %d, Broj posjeta: %d\n",
                temp->firstName, temp->lastName, temp->userId, temp->price, visits);
            totalIncome += temp->price;
        }
        temp = temp->next;
    }
    printf("Ukupna zarada: %d\n", totalIncome);
    return EXIT_SUCCESS;
}

int countVisitsInMonth(MembershipP membership, int month, int year) {
    int count = 0;
    VisitP visit = membership->visits;
    while (visit) {
        if (visit->month == month && visit->year == year) {
            count++;
        }
        visit = visit->next;
    }
    return count;
}
