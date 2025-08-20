/*Napisi program koji iz datoteke (ime se unosi u programu) čita kategoriju, naziv i količinu artikala
u skladištu. Izgraditi vezanu listu kategorija, a svaka kategorija ima dodatnu listu artikala koji joj pripadaju.
Nakon toga iz druge datoteke (ime se unosi u programu), koja predstavlja račun, čitaju podaci i umanjuje stanja na skladištu.
Ispisati kategoriju, a zatim i nazive artikala čija je količina pala ispod 5.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 32
#define FILE_NOT_OPENED (-1)

typedef struct _artikal* ArtikalP;
typedef struct _artikal {
    char name[MAX];
    int amount;
    ArtikalP next;
} Artikal;

typedef struct _kategorija* KategorijaP;
typedef struct _kategorija {
    char name[MAX];
    ArtikalP artikli;
    KategorijaP next;
} Kategorija;

/* --- Prototipovi --- */
KategorijaP createNewCategory(char* name);
ArtikalP createNewArtikl(char* name, int amount);

int addArtikl(KategorijaP head, char* catName, char* artName, int amount);
int loadFromFile(KategorijaP head, const char* fileName);

int removeProducts(KategorijaP head, const char* fileName);
int printBelowFive(KategorijaP head);

int freeAll(KategorijaP head);

/* --- MAIN --- */
int main() {
    KategorijaP head = (KategorijaP)malloc(sizeof(Kategorija));
    if (!head) {
        printf("ERROR! Could not allocate memory!\n");
        return EXIT_FAILURE;
    }
    head->next = NULL;
    head->artikli = NULL;

    char filename[MAX];
    printf("Unesite ime datoteke sa skladistem: ");
    scanf("%s", filename);

    if (loadFromFile(head, filename) != EXIT_SUCCESS) {
        freeAll(head);
        return EXIT_FAILURE;
    }

    char racun[MAX];
    printf("Unesite ime datoteke sa racunom: ");
    scanf("%s", racun);

    removeProducts(head, racun);

    printf("\nArtikli ispod 5 komada:\n");
    printBelowFive(head);

    freeAll(head);

    return EXIT_SUCCESS;
}

/* --- FUNKCIJE --- */
KategorijaP createNewCategory(char* name) {
    KategorijaP newCategory = (KategorijaP)malloc(sizeof(Kategorija));
    if (!newCategory) return NULL;

    strcpy(newCategory->name, name);
    newCategory->artikli = NULL;
    newCategory->next = NULL;

    return newCategory;
}

ArtikalP createNewArtikl(char* name, int amount) {
    ArtikalP newArtikl = (ArtikalP)malloc(sizeof(Artikal));
    if (!newArtikl) return NULL;

    strcpy(newArtikl->name, name);
    newArtikl->amount = amount;
    newArtikl->next = NULL;

    return newArtikl;
}

/* Dodaje artikl u odgovarajucu kategoriju (ako nema kategorije – kreira novu) */
int addArtikl(KategorijaP head, char* catName, char* artName, int amount) {
    KategorijaP prev = head;
    KategorijaP curr = head->next;

    while (curr && strcmp(curr->name, catName) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        curr = createNewCategory(catName);
        if (!curr) return EXIT_FAILURE;
        prev->next = curr;
    }

    ArtikalP newArt = createNewArtikl(artName, amount);
    if (!newArt) return EXIT_FAILURE;

    newArt->next = curr->artikli;
    curr->artikli = newArt;

    return EXIT_SUCCESS;
}

int loadFromFile(KategorijaP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("ERROR! Could not open the file %s!\n", fileName);
        return FILE_NOT_OPENED;
    }

    char category[MAX], name[MAX];
    int amount = 0;

    while (fscanf(fp, "%s %s %d", category, name, &amount) == 3) {
        addArtikl(head, category, name, amount);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

int removeProducts(KategorijaP head, const char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("ERROR! Could not open the file %s!\n", fileName);
        return FILE_NOT_OPENED;
    }

    char name[MAX];
    int amount;

    while (fscanf(fp, "%s %d", name, &amount) == 2) {
        KategorijaP cat = head->next;
        while (cat) {
            ArtikalP art = cat->artikli;
            while (art) {
                if (strcmp(art->name, name) == 0) {
                    art->amount -= amount;
                }
                art = art->next;
            }
            cat = cat->next;
        }
    }

    fclose(fp);
    return EXIT_SUCCESS;
}

int printBelowFive(KategorijaP head) {
    KategorijaP cat = head->next;
    while (cat) {
        int printed = 0;
        ArtikalP art = cat->artikli;
        while (art) {
            if (art->amount < 5) {
                if (!printed) {
                    printf("Kategorija: %s\n", cat->name);
                    printed = 1;
                }
                printf("\t%s (%d)\n", art->name, art->amount);
            }
            art = art->next;
        }
        cat = cat->next;
    }
    return EXIT_SUCCESS;
}

int freeAll(KategorijaP head) {
    KategorijaP cat = head->next;
    while (cat) {
        ArtikalP art = cat->artikli;
        while (art) {
            ArtikalP tmpA = art;
            art = art->next;
            free(tmpA);
        }
        KategorijaP tmpC = cat;
        cat = cat->next;
        free(tmpC);
    }
    free(head);
    return EXIT_SUCCESS;
}
