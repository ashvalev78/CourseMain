#include <stdio.h>
#include <malloc.h>
#include <conio.h>
#include <math.h>

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

typedef struct BOOK {
    char *name;
    int year;
    struct BOOK *next;
    struct BOOK *prev;
} BOOK;

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


char **FGetABase(int *num) {
    FILE *base;
    char **Amass = NULL;
    int j = 0;
    printf("Enter the way to the author file\n");
    if ((base = fopen(getstr(), "r")) != NULL) {
        fseek(base, 0, SEEK_END);
        if (ftell(base) == 0) return NULL;
        fseek(base, 0, SEEK_SET);
        Amass = (char**)malloc(sizeof(char*));
        Amass[*num] = (char*)malloc(sizeof(char));
        do {
            Amass[*num][j] = (char)fgetc(base);
            j++;
            Amass[*num] = (char*)realloc(Amass[*num], (j + 1) * sizeof(char));
            if (Amass[*num][j - 1] == '\n') {
                Amass[*num][j - 1] = '\0';
                printf("%s\n", Amass[*num]);
                (*num)++;
                Amass = (char**)realloc(Amass, (*num + 1) * sizeof(char*));
                Amass[*num] = (char*)malloc(sizeof(char));
                j = 0;
            }
        } while (Amass[*num][j] != EOF);
        Amass[*num][j] = '\0';
    }
    fclose(base);
    return Amass;
}

int getnum(int *j, int i, char **str) {
    int p, num = 0;
    (*j)++;
    p = 3;
    num = (str[i][*j] - '0') * (int)pow(10, p);
    (*j)++;
    for (p = 2; str[i][*j] != ' ' && str[i][*j] != '\0'; p--, (*j)++) {
        num = num + (str[i][*j] - '0') * (int)pow(10,p);
    }
    if (p >= 0)
        num = num / (int)pow(10, p + 1);
    return num;
}

BOOK *BooksListFromString(char **str, int num, int *quan) { // Функция для создания списка книг из полученного списка строк. Функция для последующей работы с файлами.
    BOOK *nhead = NULL, *head = NULL, *tmp = head;
    static int strnum = 0;
    int p = NULL;
    if (num == 0) return NULL;
    for (int j = 0; strnum < num && str[strnum][j] != '*'; strnum++) {
        j = 0;
        tmp = (BOOK*)malloc(sizeof(BOOK));
        if (p == NULL)
            nhead = tmp;
        p++;
        tmp->prev = head;
        tmp->next = NULL;
        tmp->name = NULL;
        for (int k = 0; str[strnum][j] != ' '; j++, k++) { // Здесь все функции построены по тому же принципу, что и в списке авторов.
            tmp->name = (char*)realloc(tmp->name, (k + 2) * sizeof(char));
            tmp->name[k] = str[strnum][j];
            tmp->name[k + 1] = '\0';
        }
        //printf("%s\n", tmp->name);
        if (str[strnum][j] == ' ') {
            tmp->year = getnum(&j, strnum, str);
            /*j++;
            p = 3;
            tmp->year = (str[strnum][j] - '0')*((int)pow(10, p));
            j++;
            for (p = 2; str[strnum][j] != '\0'; p--, j++) {
                tmp->year += (str[strnum][j] - '0') * ((int)pow(10, p));
            }
            if (p >= 0)
            tmp->year = tmp->year / ((int)pow(10, p + 1));*/
        }
        //printf("%d\n", tmp->year);
        head = tmp;
        if (head->prev != NULL)
            head->prev->next = head;
        tmp = tmp->next;
        (*quan)++;
    }
    strnum++;
    return nhead;
}

AUTHOR *AuthListFromString(char **str, int num, char **BookMass, int Bnum) { // Функция создания списка авторов из полученного списка строк. Функция для работы с файлом.
    AUTHOR *nhead = NULL, *head = NULL, *tmp;
    for (int i = 0; i < num; i++) {
        tmp = (AUTHOR*)malloc(sizeof(AUTHOR));
        if (i == 0)
            nhead = tmp;
        tmp->next = NULL;
        tmp->prev = head;
        tmp->books = NULL;
        tmp->name = NULL;
        tmp->surname = NULL;
        for (int j = 0; str[i][j] != '\0'; j++) { // Функция получения имени автора, принцип понятен основан на простом считывании всех символов от начала стоки и до пробела.
            for (int k = 0; str[i][j] != ' '; k++, j++) {
                tmp->name = (char*)realloc(tmp->name, (k + 2) * sizeof(char));
                tmp->name[k] = str[i][j];
                tmp->name[k + 1] = '\0';
            }
            //printf("%s\n", tmp->name);
            if (str[i][j] == ' ') {
                j++;
                for (int k = 0; str[i][j] != ' '; k++, j++) { // Функция получения фамилии автора, принцип действия понятен,я думаю.
                    tmp->surname = (char *) realloc(tmp->surname, (k + 2) * sizeof(char));
                    tmp->surname[k] = str[i][j];
                    tmp->surname[k + 1] = '\0';
                }
            }
            //printf("%s\n", tmp->surname);
            if (str[i][j] == ' ') { // Функции такого рода надо будет впоследствии заменить на функцию getnum(). Собирает число из считываемых символов. Чтобы собрать число использует умножение на 10^k
                tmp->birth = getnum( &j, i, str);
            }
            //printf("%d\n", tmp->birth);
            if (str[i][j] == ' ') { // Аналогично с функцией выше
                tmp->death = getnum( &j, i, str);
            }
            //printf("%d\n", tmp->death);
            if (str[i][j] == ' ') { // Аналогично с функцией выше
                tmp->numbook = getnum( &j, i, str);
                /*j++;
                p = 3;
                tmp->numbook = (str[i][j] - '0') * ((int) pow(10, p));
                j++;
                for (p = 2; str[i][j] != '\0'; p--, j++) {
                    tmp->numbook = tmp->numbook + (str[i][j] - '0') * (int)pow(10, p);
                }
                if (p >= 0)
                tmp->numbook = tmp->numbook / ((int) pow(10, p + 1));*/
            }
            //printf("%d\n", tmp->numbook);
        }
        int quan = 0;
        tmp->books = BooksListFromString(BookMass, Bnum, &quan);
        if (quan != tmp->numbook) tmp->numbook = quan;
        head = tmp;
        if (head->prev != NULL)
            head->prev->next = head;
        tmp = tmp->next;
    }
    return nhead;
}

char **GetArrayFromKeyboard() {
    int i = 0;
    char ch = NULL, **Array = NULL;
    while ((ch = getch()) != '`') {
        Array[i] =
    }
    return Array;
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

void PrintBList (BOOK *Blist) {
    if (Blist == NULL) printf("NO BOOK LIST!\n");
    else {
        for (BOOK *tmp = Blist; tmp != NULL; tmp = tmp->next)
            printf("%s\t%d\n", tmp->name, tmp->year);
    }
};

void PrintAList (AUTHOR *Alist) {
    if (Alist == NULL) printf("NO AUTHOR LIST!\n");
    else {
        for (AUTHOR *tmp = Alist; tmp != NULL; tmp = tmp->next) {
            printf("%s\t%s\t%d-%d\t%d\n", tmp->name, tmp->surname, tmp->birth, tmp->death, tmp->numbook);
            if (tmp->books != NULL)
            PrintBList(tmp->books);
            else printf("No books\n");
        }
    }
};

int main()
{
    int Anum = 0, Bnum = 0;
    char **Auth = FGetABase(&Anum);
    char **Books = FGetABase(&Bnum);
    AUTHOR *Ahead = AuthListFromString(Auth, Anum, Books, Bnum);
    PrintAList(Ahead);
    return 0;
}
