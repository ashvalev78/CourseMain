#include <stdio.h>
#include <malloc.h>
#include <conio.h>
#include <math.h>

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
/*
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
}*/

BOOK *BooksListFromString(char **str, int anum) {   //зачем передавать номер строки и массив строк, когда можно                         
    BOOK *head = NULL, *tmp = head;                 //просто передать одну строку из массива?
    int p;
    for (int j = 0; str[anum][j] != '\0'; j++) {
        tmp = (BOOK*)malloc(sizeof(BOOK));
        tmp->next = NULL;
        tmp->prev = head;                           //зачем каждый раз передвигать head, если потом все равно "head = tmp;" ?
        tmp->name = NULL;
        if (str[anum][j] == '(') {
            for (int k = 0; str[anum][j] != ')'; j++, k++) {
                tmp->name = (char*)realloc(tmp->name, (k + 2) * sizeof(char));
                tmp->name[k] = str[anum][j];
                tmp->name[k + 1] = '\0';
            }
        }
        if (str[anum][j] == ' ') {
            p = 3;
            tmp->year = ('0' - str[anum][j])*((int)pow(10,p));
            j++;
            for (p = 3; str[anum][j] != ' '; --p, j++) {
                tmp->year = tmp->year + ('0' - str[anum][j])*((int)pow(10,p));
            }
            tmp->year = tmp->year/((int)pow(10,p));
        }
        head = tmp;
        tmp = tmp->next;
    }
    return head;
}

AUTHOR *AuthListFromString(char **str, int num, char **BookMass) {
    AUTHOR *head = NULL, *tmp;
    int p;
    for (int i = 0; i < num; i++) {
        tmp = (AUTHOR*)malloc(sizeof(AUTHOR));
        tmp->prev = head;
        tmp->next = NULL;
        tmp->books = NULL;
        tmp->name = NULL;
        tmp->surname = NULL;
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
            printf("OK1\n");
            if (str[i][j] == ' ') { // All this func must be replaced by 1 func getnum(int *field);
                p = 3;
                printf("OK3\n");
                tmp->birth = (str[i][j] - '0')*((int)pow(10,p));
                printf("%c %d\n", str[i][j], tmp->birth);
                j++;
                for (p = 3; str[i][j] != ' '; --p, j++) {
                    tmp->birth = tmp->birth + (str[i][j] - '0')*((int)pow(10,p));
                }
                tmp->birth = tmp->birth/((int)pow(10,p));
            }
            if (str[i][j] == ' ') {
                p = 3;
                tmp->death = (str[i][j] - '0')*((int)pow(10,p));
                j++;
                for (p = 3; str[i][j] != ' '; --p, j++) {
                    tmp->death = tmp->death + (str[i][j] - '0')*((int)pow(10,p));
                }
                tmp->death = tmp->death/((int)pow(10,p));
            }
            if (str[i][j] == ' ') {
                p = 3;
                tmp->numbook = (0 - str[i][j]) * ((int) pow(10, p));
                j++;
                for (p = 3; str[i][j] != ' '; --p, j++) {
                    tmp->numbook = tmp->numbook + (str[i][j] - '0') * ((int) pow(10, p));
                }
                tmp->numbook = tmp->numbook / ((int) pow(10, p));
            }
        }
        printf("OK2\n");
        tmp->books = BooksListFromString(BookMass, i);
        head = tmp;
        tmp = tmp->next;
    }
    return head;
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

/*
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
*/
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
    char **Astr = (char**)malloc(3*sizeof(char*)), **Bstr = (char**)malloc(2*sizeof(char*));
    for (int i = 0; i < 3; i++) {
        Astr[i] = (char*)malloc(1000*sizeof(char));
        Astr[i] = "(Name)[Surname] 1901 1999 23\n";
    }
    for (int k = 0; k < 2; k++) {
        Bstr[k] = (char*)malloc(1000*sizeof(char));
        Bstr[k] = "(Shit) 1488\n";
    }
    AUTHOR *Ahead = AuthListFromString(Astr, 3, Bstr);
    printf("hi\n");
    char ch = getch();
    return 0;
}
