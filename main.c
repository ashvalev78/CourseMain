#include <stdio.h>
#include <malloc.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

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

void MemCheck (void *a)
{
    if (a == NULL) {
        printf ("No Memory!\n");
        exit(1);
    }
}

char *getstr () {
    int i = -1;
    char *string = NULL;
    do {
        i++;
        string = (char*)realloc(string, (i + 1) * sizeof(char));
        MemCheck(string);
        string[i] = getchar();
        if (string[0] == '\n')
            string[i] = getchar();
    } while (string[i] != '\n');
    string[i] = '\0';
    return string;
}

char **FGetABase (int *num) {
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
            MemCheck(Amass[*num]);
            if (Amass[*num][j - 1] == '\n') {
                Amass[*num][j - 1] = '\0';
                (*num)++;
                Amass = (char**)realloc(Amass, (*num + 1) * sizeof(char*));
                MemCheck(Amass);
                Amass[*num] = (char*)malloc(sizeof(char));
                MemCheck(Amass[*num]);
                j = 0;
            }
        } while (Amass[*num][j] != EOF);
        Amass[*num][j] = '\0';
    }
    fclose(base);
    return Amass;
}

int getnum (int *j,char *str) {
    int p, num = 0;
    (*j)++;
    p = 3;
    num = (str[*j] - '0') * (int)pow(10, p);
    (*j)++;
    for (p = 2; str[*j] != ' ' && str[*j] != '\0' && str[*j] != '\r'; p--, (*j)++) {
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
        MemCheck(string);
    } while ((string[k] = str[*i]) != ' ' && string[k] != '\0');
    string[k] = '\0';
    return string;
}

AUTHOR *ReverseList (AUTHOR *Ahead) {
    if (Ahead != NULL) {
        AUTHOR *tmp = Ahead;
        for (; tmp->next != NULL; tmp = tmp->prev) {
            AUTHOR *tmp1 = tmp->next;
            tmp->next = tmp->prev;
            tmp->prev = tmp1;
        }
        tmp->next = tmp->prev;
        tmp->prev = NULL;
        Ahead = tmp;
    }
    return Ahead;
}

AUTHOR *ReverseBooksList (AUTHOR *Ahead) {
    if (Ahead != NULL) {
        BOOK *tmp = Ahead->books;
        if (tmp != NULL) {
            for (; tmp->next != NULL; tmp = tmp->prev) {
                BOOK *tmp1 = tmp->next;
                tmp->next = tmp->prev;
                tmp->prev = tmp1;
            }
            tmp->next = tmp->prev;
            tmp->prev = NULL;
            Ahead->books = tmp;
        }
    }
    return Ahead;
}

BOOK *BooksListFromString (char **str, int num, int *quan, int *strnum) { // Функция для создания списка книг из полученного списка строк. Функция для последующей работы с файлами.
    BOOK *nhead = NULL, *head = NULL, *tmp = NULL;
    int p = NULL;
    if (num == 0) return NULL;
    for (int j = 0; *strnum < num && str[*strnum][0] != '*'; tmp = tmp->next, (*strnum)++) {
        j = 0;
        tmp = (BOOK*)malloc(sizeof(BOOK));
        MemCheck(tmp);
        if (p == NULL)
            nhead = tmp;
        p++;
        tmp->prev = head;
        tmp->next = NULL;
        tmp->name = NULL;
        tmp->name = getString(str[*strnum], &j);
        if (str[*strnum][j] == ' ') {
            tmp->year = getnum(&j, str[*strnum]);
        }
        head = tmp;
        if (head->prev != NULL)
            head->prev->next = head;
        (*quan)++;
    }
    (*strnum)++;
    return nhead;
} // quan - количество книг автора.

AUTHOR *AuthListFromString (char **str, int num, char **BookMass, int Bnum) { // Функция создания списка авторов из полученного списка строк. Функция для работы с файлом.
    AUTHOR *nhead = NULL, *head = NULL, *tmp = NULL;
    int BookStrNum = 0;
    for (int i = 0; i < num; i++) {
        tmp = (AUTHOR*)malloc(sizeof(AUTHOR));
        MemCheck(tmp);
        if (i == 0)
            nhead = tmp;
        tmp->next = NULL;
        tmp->prev = head;
        tmp->books = NULL;
        tmp->name = NULL;
        tmp->surname = NULL;
        for (int j = 0; str[i][j] != '\0'; j++) {
            tmp->name = getString(str[i], &j);
            if (str[i][j] == ' ') {
                j++;
                tmp->surname = getString(str[i], &j);
            }
            if (str[i][j] == ' ') {
                tmp->birth = getnum( &j, str[i]);
            }
            if (str[i][j] == ' ') {
                tmp->death = getnum( &j, str[i]);
            }
            if (str[i][j] == ' ') {
                tmp->numbook = getnum( &j, str[i]);
            }
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

BOOK *DeleteBElement (BOOK *head, int delNumber) {
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

BOOK *DeleteBFrag (BOOK *head, int num1, int num2) {
    for (int i = num1; i <= num2; i++) {
        head = DeleteBElement(head, num1);
    }
    return head;
}

AUTHOR *DeleteAElement (AUTHOR *head, int delNumber) {
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

AUTHOR *DeleteAFrag (AUTHOR *head, int num1, int num2) {
    for (int i = num1; i <= num2; i++)
        head = DeleteAElement(head, num1);
    return head;
}

char **SumArrays (char **FirstArray, char **SecondArray, int FirstNum, int SecondNum) {
    for (int i = 0; i < SecondNum; i++) {
        FirstArray = (char**)realloc(FirstArray, (FirstNum + i + 1) * sizeof(char*));
        FirstArray[FirstNum + i] = SecondArray[i];
    }
    return FirstArray;
}

char **getArray (int *numString, int structure) {
    int i;
    char **stringArray = NULL;
    printf("Enter number of elements: ");
    scanf("%d", numString);
    for (i = 0; i < *numString; i++) {
        printf("Enter the element:\n");
        stringArray = (char **) realloc(stringArray, (i + 1) * sizeof(char*));
        MemCheck(stringArray);
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

AUTHOR *GetAuth () {
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

AUTHOR *AddBookFragment (AUTHOR *AElement) {
    printf("Enter the number of element after which you want to add another\n");
    int NumBook = 0, StrNum = 0, BookNum = 0, addNumber;
    scanf("%d", &addNumber);
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

AUTHOR *AddAuthFragment (AUTHOR *Ahead) { // В данной функции все аналогично функции AddBookFragment
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

int CheckStrings(char *str1, char *str2) {
    for (int i = 0; str1[i] != '\0' && str2[i] != '\0'; i++) {
        if (str1[i] < str2[i]) {
            return 0;
        } else if (str1[i] > str2[i]) {
            return 1;
        }
    }
    if (strlen(str1) > strlen(str2))
        return 1;
    return 0;
}

AUTHOR *BookSortByAlphabet (AUTHOR *Ahead) {
    BOOK *tmp1 = Ahead->books;
    for (; tmp1 != NULL; tmp1 = tmp1->next) {
        if (tmp1->next != NULL) {
            if (CheckStrings(tmp1->name, tmp1->next->name) > 0) {
                BOOK *tmp2 = tmp1->next;
                tmp1->next = tmp2->next;
                if (tmp2->next != NULL) {
                    tmp2->next->prev = tmp1;
                }
                tmp2->prev = tmp1->prev;
                tmp2->next = tmp1;
                if (tmp1 == Ahead->books) {
                    Ahead->books = tmp2;
                } else {
                    tmp1->prev->next = tmp2;
                }
                tmp1->prev = tmp2;
                Ahead = BookSortByAlphabet(Ahead); // Рекурсивно вызываем функцию на повторную проверку, если мы меняли их местами.
            }
        }
    }
    return Ahead;
}

AUTHOR *BookSortByNum (AUTHOR *AElement) {
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
                AElement = BookSortByNum(AElement); // Рекурсивно вызываем функцию на повторную проверку, если мы меняли их местами.
            }
        }
    }
    return AElement;
}

AUTHOR *AuthSortByNum (AUTHOR *Ahead, char field) {
    int AField1 = 0, AField2 = 0;
    AUTHOR *tmp1 = Ahead;
    for (; tmp1 != NULL; tmp1 = tmp1->next) {
        if (tmp1->next != NULL) {
            switch (field) { // Выбор поля по передаваемому аргументу, чтобы не плодить большое количество одинаковых функций.
                case '1' :
                    AField1 = tmp1->birth;
                    AField2 = tmp1->next->birth;
                    break;
                case '2' :
                    AField1 = tmp1->death;
                    AField2 = tmp1->next->death;
                    break;
                case '3' :
                    AField1 = tmp1->numbook;
                    AField2 = tmp1->next->numbook;
                    break;
                default :
                    printf("No field, please, enter it again\n");
                    Ahead = AuthSortByNum(Ahead, getch());
                    break;
            }
            if (AField2 < AField1) {
                AUTHOR *tmp2 = tmp1->next;
                tmp1->next = tmp2->next;
                if (tmp2->next != NULL) {
                    tmp2->next->prev = tmp1;
                }
                tmp2->prev = tmp1->prev;
                tmp2->next = tmp1;
                if (tmp1 == Ahead) {
                    Ahead = tmp2;
                } else {
                    tmp1->prev->next = tmp2;
                }
                tmp1->prev = tmp2;
                Ahead = AuthSortByNum(Ahead, field); // Рекурсивно вызываем функцию на повторную проверку, если мы меняли их местами.
            }
        }
    }
    return Ahead;
}

AUTHOR *AuthSortByAlphabet (AUTHOR *Ahead, char field) {
    char *Field1, *Field2;
    AUTHOR *tmp1 = Ahead;
    for (; tmp1 != NULL; tmp1 = tmp1->next) {
        if (tmp1->next != NULL) {
            switch (field) { // Выбор поля по передаваемому аргументу, чтобы не плодить большое количество одинаковых функций.
                case '1' :
                    Field1 = tmp1->name;
                    Field2 = tmp1->next->name;
                    break;
                case '2' :
                    Field1 = tmp1->surname;
                    Field2 = tmp1->next->surname;
                    break;
                default :
                    printf("No field, please, enter it again\n");
                    Ahead = AuthSortByAlphabet(Ahead, getch());
                    break;
            }
            if (CheckStrings(Field1, Field2) > 0) {
                AUTHOR *tmp2 = tmp1->next;
                tmp1->next = tmp2->next;
                if (tmp2->next != NULL) {
                    tmp2->next->prev = tmp1;
                }
                tmp2->prev = tmp1->prev;
                tmp2->next = tmp1;
                if (tmp1 == Ahead) {
                    Ahead = tmp2;
                } else {
                    tmp1->prev->next = tmp2;
                }
                tmp1->prev = tmp2;
                Ahead = AuthSortByAlphabet(Ahead, field); // Рекурсивно вызываем функцию на повторную проверку, если мы меняли их местами.
            }
        }
    }
    return Ahead;
}

void PrintAuthIntoTheFile (AUTHOR *Ahead) {
    printf("WARNING! If something will go wrong, your data may be lost!\n"
                   "You can make another file if you want\n"
                   "Enter the way to the authors file\n");
    FILE *newBase = fopen(getstr(), "w");
    printf("Enter the way to the books file\n");
    FILE *BooksBase = fopen(getstr(), "w");
    for (AUTHOR *tmp = Ahead; tmp != NULL; tmp = tmp->next) {
        for (int i = 0; tmp->name[i] != '\0'; i++) {
            fputc(tmp->name[i], newBase);
        }
        fputc(' ', newBase);
        for (int i = 0; tmp->surname[i] != '\0'; i++) {
            fputc(tmp->surname[i], newBase);
        }
        fputc(' ', newBase);
        fprintf(newBase, "%d %d %d\r\n", tmp->birth, tmp->death, tmp->numbook);
        for (BOOK *btmp = tmp->books; btmp != NULL; btmp = btmp->next) {
            for (int i = 0; btmp->name[i] != '\0'; i++) {
                fputc(btmp->name[i], BooksBase);
            }
            fputc(' ', BooksBase);
            fprintf(BooksBase, "%d\r\n\0", btmp->year);
        }
        fputs("******\r\n", BooksBase);
    }
    fclose(BooksBase);
    fclose(newBase);
}

void PrintBList (BOOK *Blist) {
    if (Blist == NULL) printf("NO BOOKS!\n");
    else {
        int i = 1;
        for (BOOK *tmp = Blist; tmp != NULL; tmp = tmp->next, i++) {
            printf("\t[%d] BOOK:\tName: %s Date: %d\n", i, tmp->name, tmp->year);
        }
    }
}

void PrintAList (AUTHOR *Alist) {
    if (Alist == NULL) printf("NO AUTHOR LIST!\n");
    else {
        int i = 1;
        for (AUTHOR *tmp = Alist; tmp != NULL; tmp = tmp->next, i++) {
            printf("[%d] AUTHOR:\t%s %s Years: %d-%d Number of books: %d\n", i, tmp->name, tmp->surname, tmp->birth, tmp->death, tmp->numbook);
            printf("BOOKS:\n");
            PrintBList(tmp->books);
        }
    }
}

void FreeMass (char **Mass, int size) {
    if (Mass != NULL) {
        for (int i = 0; i < size; i++) {
            free(Mass[i]);
        }
        free(Mass);
    }
}

void FreeBooks (BOOK *Bhead) {
    if (Bhead != NULL) {
        while (Bhead->next != NULL) {
            Bhead = Bhead->next;
            free(Bhead->prev->name);
            free(Bhead->prev);
        }
        free(Bhead->name);
        free(Bhead);
    }
}

void FreeAuth (AUTHOR *Ahead) {
    if (Ahead != NULL) {
        while (Ahead->next != NULL) {
            Ahead = Ahead->next;
            free(Ahead->prev->name);
            free(Ahead->prev->surname);
            FreeBooks(Ahead->prev->books);
        }
        free(Ahead->name);
        free(Ahead->surname);
        free(Ahead);
    }
}

void menu () {
    printf("\t\tMENU\n"
                   "\tAuthor string format: Name Surname Birth Death (Number of books)\n"
                   "\tBook string format: Name_of_the_book Year\n"
                   "\tPress 0 to escape the program\n"
                   "\tPress 1 to see menu again\n"
                   "\tPress 2 to enter your lists from the file\n"
                   "\tPress 3 to enter your own lists by keyboard\n"
                   "\tPress 4 to add some elements into the lists\n"
                   "\tPress 5 to delete some elements from your lists\n"
                   "\tPress 6 to sort your lists\n"
                   "\tPress 7 to output the lists\n"
                   "\tPress 8 to save your changes\n ");
}

int main()
{
    int Anum = 0, Bnum = 0, num = 0, num1, num2, type;
    char **Auth = NULL, **Books = NULL;
    char input1, input2, field;
    AUTHOR *Ahead = NULL, *tmp = NULL;
    char ch;
    menu ();
    while ((input1 = getch()) != '0') {
        switch (input1) {
            case '1' : // Вывод меню.
                menu();
                break;
            case '2' :
                ch = getch();
                Auth = FGetABase(&Anum); // Получение массива авторов из файла.
                Books = FGetABase(&Bnum);// Получение массива книг из файла.
                Ahead = AuthListFromString(Auth, Anum, Books, Bnum); // преобразование массивов авторов и книг в списки.
                PrintAList(Ahead);
                printf("Done!\n");
                break;
            case '3' :
                ch = getch();
                Ahead = GetAuth();
                PrintAList(Ahead);
                printf("Done!\n");
                break;
            case '4' :
                ch = getch();
                printf("Press 1 to add elements into the authors list\n"
                               "Press 2 to add elements into the someones books list\n"
                               "Press 0 to exit this option\n");
                while ((input2 = getch()) != '0') {
                    switch (input2) {
                        case '1' :
                            ch = getch();
                            Ahead = AddAuthFragment(Ahead);
                            printf("Added\n");
                            break;
                        case '2' :
                            ch = getch();
                            printf("Enter the number of author, whose books list you want to change\n");
                            scanf("%d", &num);
                            tmp = Ahead;
                            for (int i = 0; i < num; i++) { // Доходим до нужного элемента.
                                tmp = tmp->next;
                            }
                            tmp = AddBookFragment(tmp);
                            printf("Added\n");
                            break;
                        default :
                            printf("You've chosen the wrong number, try again\n");
                            break;
                    }
                }
                printf("Done!\n");
                break;
            case '5' :
                ch = getch();
                printf("Press 1 to delete elements into authors list\n"
                               "Press 2 to delete elements into someones books list\n"
                               "Press 0 to exit this option\n");
                while ((input2 = getch()) != '0') {
                    switch (input2) {
                        case '1' :
                            ch = getch();
                            printf("Enter the interval you want to delete\n");
                            scanf("%d %d", &num1, &num2);
                            printf("You want to delete from %d to %d", num1, num2);
                            Ahead = DeleteAFrag(Ahead, num1, num2);
                            printf("Deleted\n");
                            break;
                        case '2' :
                            ch = getch();
                            printf("Enter the number of author, whose books list you want to change\n");
                            scanf("%d", &num);
                            tmp = Ahead;
                            for (int i = 0; i < num; i++) { // Доходим до нужного элемента.
                                tmp = tmp->next;
                            }
                            printf("Enter the interval you want to delete\n");
                            scanf("%d %d", &num1, &num2);
                            tmp->books = DeleteBFrag(tmp->books, num1, num2);
                            printf("Deleted\n");
                            break;
                        default :
                            printf("You've chosen the wrong number, try again\n");
                            break;
                    }
                }
                PrintAList(Ahead);
                printf("Done!\n");
                break;
            case '6' :
                ch = getch();
                printf("Press 1 to sort elements in authors list\n"
                               "Press 2 to sort elements in someones books list\n"
                               "Press 0 to exit this option\n");
                while ((input2 = getch()) != '0') {
                    switch (input2) {
                        case '1' :
                            printf("Choose the type of sort (1 - by number, 2 - by alphabet\n");
                            scanf("%d", &type);
                            if (type == 1) {
                                printf("Enter the field, which will be the attribute to sort\n"
                                               "1 - birth, 2 - death, 3 - Number of books\n");
                                scanf("%c", &field);
                                Ahead = AuthSortByNum(Ahead, field);
                            }
                            else {
                                printf("Enter the field, which will be the attribute to sort\n"
                                               "1 - Name, 2 - Surname\n");
                                scanf("%c", &field);
                                Ahead = AuthSortByAlphabet(Ahead, field);
                            }
                            printf("If you wanted to make reverse-sort, press 1, else - press any key)\n");
                            if ((field = getch()) == '1') {
                                Ahead = ReverseList(Ahead);
                            }
                            break;
                        case '2' :
                            ch = getch();
                            printf("Enter the number of author\n");
                            scanf("%d", &num);
                            tmp = Ahead;
                            for (int i = 0; i < num; i++) { // Доходим до нужного элемента.
                                tmp = tmp->next;
                            }
                            printf("Choose the type of sort (1 - by number, 2 - by alphabet\n");
                            scanf("%d", &type);
                            if (type == 1) {
                                tmp = BookSortByNum(tmp);
                            }
                            else {
                                tmp = BookSortByAlphabet(tmp);
                            }
                            printf("If you wanted to make reverse-sort, press 1, else - press any key)\n");
                            if ((field = getch()) == '1') {
                                tmp = ReverseBooksList(tmp);
                            }
                            break;
                        default :
                            printf("You've chosen the wrong number, try again\n");
                            break;
                    }
                }
                PrintAList(Ahead);
                printf("Done!\n");
                break;
            case '7' :
                ch = getch();
                printf("******************************[YOUR AUTHOR LIST]********************************\n");
                PrintAList(Ahead);
                printf("********************************************************************************\n");
                break;
            case '8' :
                ch = getch();
                PrintAuthIntoTheFile(Ahead);
                printf("Got it!\n");
                break;
            default :
                printf("Please, enter another number\n");
                break;
        }
    }
    FreeMass(Auth, Anum);
    FreeMass(Books, Bnum);
    FreeAuth(Ahead);
    return 0;
}
