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

char **FGetABase(int *num) {
    FILE *base;
    char **Amass = NULL;
    int j = 0;
    printf("Enter the way to the file\n");
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
                //printf("%s\n", Amass[*num]);
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

int getnum(int *j,char *str) {
    int p, num = 0;
    (*j)++;
    p = 3;
    num = (str[*j] - '0') * (int)pow(10, p);
    (*j)++;
    for (p = 2; str[*j] != ' ' && str[*j] != '\0'; p--, (*j)++) {
        num = num + (str[*j] - '0') * (int)pow(10,p);
    }
    if (p >= 0)
        num = num / (int)pow(10, p + 1);
    return num;
}

char *getString (char *str, int *i) {
    char *string = NULL;
    int k = -1;
    (*i)--;
    do {
        k++;
        (*i)++;
        string = (char*)realloc(string, (k + 1) * sizeof(char));
    } while ((string[k] = str[*i]) != ' ');
    string[k] = '\0';
    return string;
}

BOOK *BooksListFromString(char **str, int num, int *quan, int *strnum) { // Функция для создания списка книг из полученного списка строк. Функция для последующей работы с файлами.
    BOOK *nhead = NULL, *head = NULL, *tmp;
    int p = NULL;
    if (num == 0) return NULL;
    for (int j = 0; *strnum < num && str[*strnum][0] != '*'; (*strnum)++) {
        j = 0;
        tmp = (BOOK*)malloc(sizeof(BOOK));
        if (p == NULL)
            nhead = tmp;
        p++;
        tmp->prev = head;
        tmp->next = NULL;
        tmp->name = NULL;
        tmp->name = getString(str[*strnum], &j);
        //printf("%s\n", tmp->name);
        if (str[*strnum][j] == ' ') {
            tmp->year = getnum(&j, str[*strnum]);
        }
        //printf("%d\n", tmp->year);
        head = tmp;
        if (head->prev != NULL)
            head->prev->next = head;
        tmp = tmp->next;
        (*quan)++;
    }
    (*strnum)++;
    return nhead;
} // quan - количество книг автора.

AUTHOR *AuthListFromString(char **str, int num, char **BookMass, int Bnum) { // Функция создания списка авторов из полученного списка строк. Функция для работы с файлом.
    AUTHOR *nhead = NULL, *head = NULL, *tmp;
    printf("NUMBER IS %d\n", num);
    int BookStrNum = 0;
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
            tmp->name = getString(str[i], &j);
            if (str[i][j] == ' ') {
                j++;
                tmp->surname = getString(str[i], &j);
            }
            //printf("%s\n", tmp->surname);
            if (str[i][j] == ' ') { // Функции такого рода надо будет впоследствии заменить на функцию getnum(). Собирает число из считываемых символов. Чтобы собрать число использует умножение на 10^k
                tmp->birth = getnum( &j, str[i]);
            }
            //printf("%d\n", tmp->birth);
            if (str[i][j] == ' ') { // Аналогично с функцией выше
                tmp->death = getnum( &j, str[i]);
            }
            //printf("%d\n", tmp->death);
            if (str[i][j] == ' ') { // Аналогично с функцией выше
                tmp->numbook = getnum( &j, str[i]);
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
        tmp->books = BooksListFromString(BookMass, Bnum, &quan, &BookStrNum);
        if (quan != tmp->numbook) tmp->numbook = quan;
        head = tmp;
        if (head->prev != NULL)
            head->prev->next = head;
        tmp = tmp->next;
    }
    return nhead;
}

BOOK *DeleteBElement(BOOK *head, int delNumber) {
    if (head != NULL) {
        BOOK *tmp = head;
        for (int i = 1; i < delNumber; i++) {
            if (tmp->next == NULL) return head;
            tmp = tmp->next;
        }
        if (delNumber == 1) {
            if (tmp->next == NULL) {
                head = NULL;
            } else {
                tmp->next->prev = NULL;
                head = tmp->next;
            }
        } else if (tmp->next == NULL) {
            tmp->prev->next = NULL;
        } else {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
        }
        free(tmp->name);
        free(tmp);
    }
    return head;
}

BOOK *DeleteBFrag(BOOK *head, int num1, int num2) {
    for (int i = num1; i <= num2; i++) {
        head = DeleteBElement(head, num1);
    }
    return head;
}

AUTHOR *DeleteAElement(AUTHOR *head, int delNumber) {
    if (head != NULL) {
        AUTHOR *tmp = head;
        for (int i = 1; i < delNumber; i++) {
            if (tmp->next == NULL) return head;
            tmp = tmp->next;
        }
        if (delNumber == 1) {
            if (tmp->next == NULL) {
                head = NULL;
            } else {
                tmp->next->prev = NULL;
                head = tmp->next;
            }
        } else if (tmp->next == NULL) {
            tmp->prev->next = NULL;
        } else {
            tmp->prev->next = tmp->next;
            tmp->next->prev = tmp->prev;
        }
        free(tmp->name);
        free(tmp->surname);
        tmp->books = DeleteBFrag(tmp->books, 1, tmp->numbook);
        free(tmp);
    }
    return head;
}

AUTHOR *DeleteAFrag(AUTHOR *head, int num1, int num2) {
    for (int i = num1; i <= num2; i++)
        head = DeleteAElement(head, num1);
    return head;
}

char **SumArrays(char **FirstArray, char **SecondArray, int FirstNum, int SecondNum) {
    for (int i = 0; i < SecondNum; i++) {
        FirstArray = (char**)realloc(FirstArray, (FirstNum + i + 1) * sizeof(char*));
        FirstArray[FirstNum + i] = SecondArray[i];
    }
    return FirstArray;
}

char **getArray(int *numString, int structure) {
    int pos = -1, i;
    char **stringArray = NULL;
    printf("Enter number of elements: ");
    *numString = getnum(&pos, getstr());
    printf("%d\n", *numString);
    for (i = 0; i < *numString; i++) {
        stringArray = (char **) realloc(stringArray, (i + 1) * sizeof(char*));
        stringArray[i] = NULL;
        stringArray[i] = getstr();
    }
    if (structure == 1) { // Если это массив книг, то нужна строка-разделитель.
        stringArray = (char**)realloc(stringArray, (i + 1) * sizeof(char*));
        stringArray[i] = "*\n";
        (*numString)++;
    }
    return stringArray;
}

AUTHOR *GetAuth() {
    AUTHOR *Auth;
    int Anum = 0, Bnum1 = 0, Bnum2;
    char **AuthArray = getArray(&Anum, 0);
    char **BookArray1 = NULL, **BookArray2 = NULL;
    if (Anum > 0) {
        BookArray1 = getArray(&Bnum1, 1);
    }
    for (int i = 1; i < Anum; i++) {
        Bnum2 = 0;
        BookArray2 = getArray(&Bnum2, 1);
        BookArray1 = SumArrays(BookArray1, BookArray2, Bnum1, Bnum2);
        Bnum1 += Bnum2;
    }
    Auth = AuthListFromString(AuthArray, Anum, BookArray1, Bnum1);
    return Auth;
}

AUTHOR *AddBookFragment(AUTHOR *AElement) {
    printf("Enter the number of element after which you want to add another\n");
    int pos = -1, NumBook = 0, StrNum = 0, BookNum = 0, addNumber = getnum(&pos, getstr());
    char **BookFrag = getArray(&BookNum, 1); // Получаем массив книг, который впоследствии преобразуем в список для вставки.
    BOOK *BookHead = BooksListFromString(BookFrag, BookNum, &NumBook, &StrNum); //Формируем из массива книг список книг, который потом будем вставлять.
    BOOK *BookTail = BookHead;

    for (; BookTail->next != NULL; BookTail = BookTail->next); // Создаем указатель на конец списка книг.
    AElement->numbook += NumBook; // Увеличиваем количество книг в поле автора, поскольку мы добавляем какое-то количество.
    BOOK *tmp = AElement->books; // Заходим в список книг автора.

    if (tmp != NULL) {
        for (int i = 1; i < addNumber && tmp->next != NULL; i++, tmp = tmp->next); // Доходим до нужного по номеру элемента.
    }

    if (addNumber == 0 || tmp == NULL) {
        BookTail->next = AElement->books;
        if (AElement->books != NULL)
            AElement->books->prev = BookTail;
        AElement->books = BookHead;
    } else if (tmp->next == NULL) {
        tmp->next = BookHead;
        BookHead->prev = tmp;
    } else {
        BookTail->next = tmp->next;
        tmp->next->prev = BookTail;
        BookHead->prev = tmp;
        tmp->next = BookHead;
    }
    return AElement;
}

AUTHOR *AddAuthFragment(AUTHOR *Ahead) { // В данной функции все аналогично функции AddBookFragment

    printf("Enter the number of element after which you want to add another\n");
    int pos = -1, addNumber = getnum(&pos, getstr());
    AUTHOR *AFragHead = GetAuth();
    AUTHOR *AFragTail = AFragHead;
    for (; AFragTail->next != NULL; AFragTail = AFragTail->next);
    AUTHOR *tmp = Ahead;

    if (tmp != NULL) {
        for (int i = 1; i < addNumber && tmp->next != NULL; i++, tmp = tmp->next);
    }

    if (addNumber == 0 || tmp == NULL) {
        AFragTail->next = Ahead;
        if (Ahead != NULL)
            Ahead->prev = AFragTail;
        Ahead = AFragHead;
    } else if (tmp->next == NULL) {
        tmp->next = AFragHead;
        AFragHead->prev = tmp;
    } else {
        AFragTail->next = tmp->next;
        tmp->next->prev = AFragTail;
        AFragHead->prev = tmp;
        tmp->next = AFragHead;
    }
    return Ahead;
}

AUTHOR *BookSortNum(AUTHOR *AElement) {
    int counter = 0;
    BOOK *tmp1 = AElement->books;
    for (; tmp1 != NULL; tmp1 = tmp1->next) { // Пока не прошли список целиком...
        if (tmp1->next != NULL) { // Если в списке не один элемент...
            if (tmp1->next->year < tmp1->year) { // Сравниваем первый со вторым.
                BOOK *tmp2 = tmp1->next; // Создаем второй.
                tmp1->next = tmp2->next; // Если надо поменять их местами, меняем.
                if (tmp2->next != NULL) {
                    tmp2->next->prev = tmp1;
                }
                tmp2->prev = tmp1->prev;
                tmp2->next = tmp1;
                if (tmp1 == AElement->books) {
                    AElement->books = tmp2;
                } else {
                    tmp1->prev->next = tmp2;
                }
                tmp1->prev = tmp2;
                counter++; // Отмечаем тот факт, что мы поменяли их местами.
            }
            if (counter > 0) {
                BookSortNum(AElement); // Рекурсивно вызываем функцию на повторную проверку, если мы меняли их местами.
            }
        }
    }
    return AElement;
}

AUTHOR *AuthSortByBirthNum(AUTHOR *Ahead) {
    int counter = 0;
    AUTHOR *tmp1 = Ahead;
    for (; tmp1 != NULL; tmp1 = tmp1->next) {
        if (tmp1->next != NULL) {

        }
    }
    return Ahead;
}

void PrintBList (BOOK *Blist) {
    if (Blist == NULL) printf("NO BOOKS!\n");
    else {
        int i = 1;
        for (BOOK *tmp = Blist; tmp != NULL; tmp = tmp->next, i++) {
            printf("\t[%d] BOOK:\t%s %d\n", i, tmp->name, tmp->year);
        }
    }
};

void PrintAList (AUTHOR *Alist) {
    if (Alist == NULL) printf("NO AUTHOR LIST!\n");
    else {
        int i = 1;
        for (AUTHOR *tmp = Alist; tmp != NULL; tmp = tmp->next, i++) {
            printf("[%d] AUTHOR:\t%s %s %d-%d %d\n", i, tmp->name, tmp->surname, tmp->birth, tmp->death, tmp->numbook);
            printf("BOOKS:\n");
            PrintBList(tmp->books);
        }
    }
};

int main()
{
    int Anum = 0, Bnum = 0;
    //AUTHOR *Authors = GetAuth();
    //PrintAList(Authors);
    char **Auth = FGetABase(&Anum); // Получение массива авторов из файла
    char **Books = FGetABase(&Bnum);// Получение массива книг из файла
    AUTHOR *Ahead = AuthListFromString(Auth, Anum, Books, Bnum); // преобразование массивов авторов и книг в списки.
    PrintAList(Ahead);
    printf("******************************[NEW AUTHOR LIST]********************************\n");
    //Ahead = AddBookFragment(Ahead);
    //Ahead = DeleteAFrag(Ahead, 1, 2);
    //AddAuthorFragment(Ahead, Ahead, 2);
    //Ahead = AddAuthFragment(Ahead);
    Ahead = BookSortNum(Ahead);
    PrintAList(Ahead);
    return 0;
}
