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
}*///

BOOK *BooksListFromString(char **str, int num) {
    BOOK *head = NULL, *tmp = head;
    static int strnum = 0;
    int p;
    for (int j = 0; str[strnum][j] != '*' && strnum < num && str[strnum][j] != '\0'; strnum++) {
        tmp = (BOOK*)malloc(sizeof(BOOK));
        tmp->next = NULL;
        tmp->prev = head;
        tmp->name = NULL;
        for (int k = 0; str[strnum][j] != ' '; j++, k++) {
            tmp->name = (char*)realloc(tmp->name, (k + 2) * sizeof(char));
            tmp->name[k] = str[strnum][j];
            tmp->name[k + 1] = '\0';
        }
        printf("%s\n", tmp->name);
        if (str[strnum][j] == ' ') {
            j++;
            p = 3;
            tmp->year = (str[strnum][j] - '0')*((int)pow(10, p));
            j++;
            for (p = 2; str[strnum][j] != '\0'; p--, j++) {
                tmp->year += (str[strnum][j] - '0')*((int)pow(10, p));
            }
            tmp->year = tmp->year / ((int)pow(10, p + 1));
        }
        printf("%d\n", tmp->year);
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
            for (int k = 0; str[i][j] != ' '; k++, j++) {
                tmp->name = (char*)realloc(tmp->name, (k + 2) * sizeof(char));
                tmp->name[k] = str[i][j];
                tmp->name[k + 1] = '\0';
            }
            printf("%s\n", tmp->name);
            if (str[i][j] == ' ') {
                j++;
                for (int k = 0; str[i][j] != ' '; k++, j++) {
                    tmp->surname = (char *) realloc(tmp->surname, (k + 2) * sizeof(char));
                    tmp->surname[k] = str[i][j];
                    tmp->surname[k + 1] = '\0';
                }
            }
            printf("%s\n", tmp->surname);
            if (str[i][j] == ' ') { // All this func must be replaced by 1 func getnum(int *field);
                j++;
                p = 3;
                tmp->birth = (str[i][j] - '0') * (int)pow(10, p);
                j++;
                for (p = 2; str[i][j] != ' '; p--, j++) {
                    tmp->birth = tmp->birth + (str[i][j] - '0') * (int)pow(10,p);
                }
                if (p > 0)
                tmp->birth = tmp->birth / (int)pow(10, p + 1);
            }
            printf("%d\n", tmp->birth);
            if (str[i][j] == ' ') {
                j++;
                p = 3;
                tmp->death = (str[i][j] - '0') * (int)pow(10, p);
                j++;
                for (p = 2; str[i][j] != ' '; p--, j++) {
                    tmp->death = tmp->death + (str[i][j] - '0') * (int)pow(10,p);
                }
                if (p > 0)
                tmp->death = tmp->death / ((int)pow(10, p + 1));
            }
            printf("%d\n", tmp->death);
            if (str[i][j] == ' ') {
                j++;
                p = 3;
                tmp->numbook = (str[i][j] - '0') * ((int) pow(10, p));
                j++;
                for (p = 2; str[i][j] != '\0'; p--, j++) {
                    tmp->numbook = tmp->numbook + (str[i][j] - '0') * (int)pow(10, p);
                }
                if (p > 0)
                tmp->numbook = tmp->numbook / ((int) pow(10, p + 1));
            }
            printf("%d\n", tmp->numbook);
        }
        tmp->books = BooksListFromString(BookMass, 2);
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
        Astr[i] = (char *) malloc(1000 * sizeof(char));
        Astr[i] = "Name Surname 1901 2999 33\0";
        printf("%s\n", Astr[i]);
    }
    for (int k = 0; k < 2; k++) {
        Bstr[k] = (char*)malloc(1000*sizeof(char));
        Bstr[k] = "Shit 1488\0";
    }
    AUTHOR *Ahead = AuthListFromString(Astr, 3, Bstr);
    printf("hi\n");
    char ch = getch();
    return 0;
}
