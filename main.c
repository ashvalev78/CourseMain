#include <stdio.h>
#include <malloc.h>
#include <conio.h>

typedef struct BOOK {
    char *name;
    int year;
    struct BOOK *next;
    struct BOOK *prev;
} BOOK;

typedef struct AUTHOR {
    char *name;
    char *surname;
    int birth;
    int death;
    int numbook;
    BOOK *books;
    struct AUTHOR *next;
    struct AUTHOR *prev;
} AUTHOR;

char *getstr() {
    int i = -1;
    char *string = NULL;
    do {
        i++;
        string = (char*)realloc(string, (i + 1) * sizeof(char));
    } while ((string[i] = getchar()) != '\n');
    string[i] = '\0';
    return string;
}

AUTHOR *fgetbase() {
    FILE *base;
    AUTHOR *Ahead, *tmp;
    BOOK *btmp = (BOOK*)malloc(sizeof(BOOK));
    printf("Enter the way to the author file\n ");
    if ((base = fopen(getstr(), "r")) != NULL) {
        tmp = Ahead = (AUTHOR*)malloc(sizeof(AUTHOR));
        fscanf(base, "%s%s%d%d%d", Ahead->name, Ahead->surname, &Ahead->birth, &Ahead->death, &Ahead->numbook);
        do {
            tmp->next = (AUTHOR*)malloc(sizeof(AUTHOR));
            (tmp->next)->prev = tmp;
            tmp = tmp->next;
        } while (fscanf(base, "%s%s%d%d%d", Ahead->name, Ahead->surname, &Ahead->birth, &Ahead->death, &Ahead->numbook) != EOF);
    }
}

AUTHOR *AuthListFromString(char **str, int num) {
    AUTHOR *head = NULL, *tmp = head;
    for (int i = 0; i < num; i++) {
        tmp = (AUTHOR*)malloc(sizeof(AUTHOR));
        tmp->prev = head;
        tmp->next = NULL;
        tmp->books = NULL;
        for (int j = 0; str[i][j] != '\0'; j++) {
            if (str[i][j] == '(') { // I decided that name will be written in the rounded scopes
                for (int k = 0; str[i][j] != ')'; k++, j++) {
                    tmp->name = (char*)realloc(tmp->name, (k + 2) * sizeof(char));
                    tmp->name[k] = str[i][j];
                    tmp->name[k + 1] = '\0';
                }
            }
            if (str[i][j] == '[') { // Surname will be written in the square scopes
                for (int k = 0; str[i][j] != ']'; k++, j++) {
                    tmp->surname = (char *) realloc(tmp->surname, (k + 2) * sizeof(char));
                    tmp->surname[k] = str[i][j];
                    tmp->surname[k + 1] = '\0';
                }
            }
            if (str[i][j] == ' ') {
                tmp->birth = ('0' - str[i][j])*1000;
                j++;
                for (int k = 2; str[i][j] != ' '; k--) {
                    tmp->birth = tmp->birth + ('0' - str[i][j]);
                }
            }
        }
    }
}

AUTHOR *getauth() {
    AUTHOR *Ahead = (AUTHOR*)malloc(sizeof(AUTHOR));
    Ahead->next = NULL;
    Ahead->numbook = 0;
    printf("Enter the author's name\n");
    Ahead->name = getstr();
    printf("Enter the author's surname\n");
    Ahead->surname = getstr();
    printf("Enter the author's birth and death years\n");
    scanf("%d, %d", &Ahead->birth, &Ahead->death);
    return Ahead;
}

BOOK *getbooks(int *num) {
    /*BOOK *Bhead, *tmp;
    tmp = Bhead = (BOOK*)malloc(sizeof(BOOK));
    printf("Enter the name of the book\n ");
    Bhead->name = getstr();
    printf("Enter the date of edition\n ");
    scanf("%d", &Bhead->year);
    Bhead->prev = NULL;
    *num++;
    for (char ch = '0'; (ch = getch()) != '`';) {
        tmp->next = (BOOK*)malloc(sizeof(BOOK));
        (tmp->next)->prev = tmp;
        tmp = tmp->next;
        printf("Enter the name of the book\n ");
        Bhead->name = getstr();
        printf("Enter the date of edition\n ");
        scanf("%d", &Bhead->year);
        *num++;
    }
    tmp->next = NULL;
    return Bhead;
     */
    BOOK *head = NULL, *tmp = head;
    for (char ch = NULL; (ch = getch()) != '`';) {
        tmp = (BOOK*)malloc(sizeof(BOOK));
        tmp->next = NULL;
        tmp->prev = head;
        printf("Enter the name of the book\n");
        tmp->name = getstr();
        printf("Enter the date of edition\n");
        scanf("%d", &tmp->year);
        *num++;
        head = tmp;
        tmp = tmp->next;
    }
    return head;
}

AUTHOR *getauthlist () {
    AUTHOR *Ahead, *tmp;
    tmp = Ahead = getauth();
    Ahead->prev = NULL;
    Ahead->books = getbooks(&Ahead->numbook);
    for (char ch = '0'; (ch = getch()) != '\b';) {
        tmp->next = (AUTHOR*)malloc(sizeof(AUTHOR));
        (tmp->next)->prev = tmp;
        tmp = tmp->next;
        tmp = getauth();
        tmp->books = getbooks(&tmp->numbook);
    }
    return Ahead;
}

void Afree(AUTHOR *head) {
    if (head != NULL) {
        while (head->next != NULL) {
            head = head->next;
            free(head->prev);
        }
        free(head);
    }
}

int main()
{
    printf("hi\n");
    return 0;
}
