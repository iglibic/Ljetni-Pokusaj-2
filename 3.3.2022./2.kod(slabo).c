	// Napisati program koji iz datoteke membership.txt i visits.txt (imena datoteka unosi korisnik) čita podatke
	// u mjesečnim članarinama korisnika i o posjetima u teretani.

	// OCJENA 2: Pročitati datoteke i uvezati na način da se posjete povežu s članovima (lista u listi). Posjeta se
	// veže s članarinom ako je userId isti i ako se posjeta dogodila u tom mjesecu.
	// Lista članarine mora biti sortirana po godini i mjesecu. Posjete moraju biti sortirane po danu u mjesecu.
	// Ispisati sve članarine i posjete u tom mjesecu.

	#define _CRT_SECURE_NO_WARNINGS
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h> 

	#define MAX_ID_LENGTH (64)
	#define MAX_NAME_LENGTH (64)

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

	int printMonthYear(MembershipP head, int m, int y);

	int main() {
		MembershipP head = (MembershipP)malloc(sizeof(Membership));
		if (head == NULL) {
			printf("ERROR! Could not allocate memmory!\n");
			return EXIT_FAILURE;
		}
		head->next = NULL;

		char fileName1[MAX_NAME_LENGTH], fileName2[MAX_NAME_LENGTH];

		printf("Unesite ime datoteke za clanarine: \n");
		scanf("%s", fileName1);

		loadMembership(head, fileName1);

		printf("\nUnesite ime datoteke za posjete: \n");
		scanf("%s", fileName2);

		loadVisit(head, fileName2);

		int m, y;

		printf("\nUpisite mjesec i godinu za ispis: ");
		scanf("%d %d", &m, &y);

		printMonthYear(head, m, y);

		freeMembership(head);

		return EXIT_SUCCESS;
	}

	MembershipP createNewMembership(char* id, char* fName, char* lName, int m, int y, int price) {
		MembershipP newMembership = (MembershipP)malloc(sizeof(Membership));
		if (newMembership == NULL) {
			printf("ERROR! Could not allocate memmory!\n");
			return NULL;
		}
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
		if (newVisit == NULL) {
			printf("ERROR! Could not allocate memmory!\n");
			return NULL;
		}
		strcpy(newVisit->userId, id);
		newVisit->day = d;
		newVisit->month = m;
		newVisit->year = y;
		newVisit->next = NULL;

		return newVisit;
	}

	MembershipP findMembership(MembershipP head, char* id) {
		MembershipP temp = head->next;
		while (temp) {
			if (strcmp(temp->userId, id) == 0 && temp->month == m && temp->year == y) {
				return temp;
			}
			temp = temp->next;
		}
		return NULL;
	}

	int insertSortedMembership(MembershipP head, MembershipP newMembership) {
		MembershipP prev = head;
		MembershipP curr = head->next;
		while (curr) {
			if (newMembership->year < curr->year) {
				break;
			}
			else {
				if ((newMembership->year == curr->year) && (newMembership->month <= curr->month)) {
					break;
				}
				else {
					prev = curr;
					curr = curr->next;
				}
			}
		}
		prev->next = newMembership;
		newMembership->next = curr;

		return EXIT_SUCCESS;
	}

	int insertSortedVisit(MembershipP membership, VisitP newVisit) {					//nisam bas znao
		if (membership == NULL || newVisit == NULL) {
			printf("ERROR! Irregular values!\n");
			return EXIT_FAILURE;
		}
		VisitP prev = NULL;
		VisitP curr = membership->visits;
	
		while (curr && newVisit->day > curr->day) {
			prev = curr;
			curr = curr->next;
		}

		if (prev == NULL) {
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
		if (fp == NULL) {
			printf("ERROR! Could not open the file!\n");
			exit(EXIT_FAILURE);
		}
		char id[MAX_ID_LENGTH], name[MAX_NAME_LENGTH], lname[MAX_NAME_LENGTH];
		int m, y, price;
		while (fscanf(fp, "%s %s %s %d %d %d", id, name, lname, &m, &y, &price) == 6) {
			MembershipP newMembership = createNewMembership(id, name, lname, m, y, price);
			if (newMembership == NULL) {
				printf("ERROR! Could not allocate memmory!\n");
				return EXIT_FAILURE;
			}
			insertSortedMembership(head, newMembership);
		}
		fclose(fp);

		return EXIT_SUCCESS;
	}

	int loadVisit(MembershipP head, const char* fileName) {						//i ovu sam malo zajebo
		FILE* fp = fopen(fileName, "r");
		if (fp == NULL) {
			printf("ERROR! Could not open the file!\n");
			exit(EXIT_FAILURE);
		}
		char id[MAX_ID_LENGTH];
		int d, m, y;
		while (fscanf(fp, "%s %d %d %d", id, &d, &m, &y) == 4) {
			VisitP newVisit = createNewVisit(id, d, m, y);
			if (newVisit == NULL) {
				printf("ERROR! Could not allocate memmory!\n");
				return EXIT_FAILURE;
			}
			MembershipP membership = findMembership(head, id);
		
			while (membership) {
				if (membership->year == y && membership->month == m) {
					insertSortedVisit(membership, newVisit);
					break;
				}
				membership = membership->next;
			}

			if (membership == NULL) {
				free(newVisit);
				return EXIT_FAILURE;
			}
		}
		fclose(fp);

		return EXIT_SUCCESS;
	}

	int freeVisit(VisitP head) {
		VisitP temp = head;
		while (temp) {
			VisitP toFree = temp;
			temp = temp->next;
			free(toFree);
		}
		return EXIT_SUCCESS;
	}

	int freeMembership(MembershipP head) {
		MembershipP temp = head->next;
		while(temp) {
			freeVisit(temp->visits);
			MembershipP toFree = temp;
			temp = temp->next;
			free(toFree);
		}
		head->next = NULL;
		return EXIT_SUCCESS;
	}

	int printMonthYear(MembershipP head, int m, int y) {
		MembershipP temp = head->next;
		while (temp) {
			if (temp->month == m && temp->year == y) {
				printf("Osoba: %s %s, ID: %s, Iznos: %d\n", temp->firstName, temp->lastName, temp->userId, temp->price);
				VisitP visit = temp->visits;
				int found = 0;
				while (visit) {
					if (visit->month == m && visit->year == y) {
						printf("Datum posjete: %d.%d.%d\n", visit->day, visit->month, visit->year);
						found = 1;
					}
					visit = visit->next;
				}
				if (!found) {
					printf("Osoba nije imala nijednu posjetu u tom mjesecu i godini!\n");
				}
			}
			temp = temp->next;
		}
		return EXIT_SUCCESS;
	}
