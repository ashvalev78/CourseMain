#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct prospect {

    char *name;
    unsigned int number;
    struct prospect *next, *prev;

} prospect;

typedef struct city {

    unsigned int foundation, prospNumber;
    float population;
    char *name;
    prospect *prosp;
    struct city *next, *prev;

} city;


void memoryCheck(void *pointer)
{
    if (!pointer)
    {
        printf("No memory");
        exit(1);
    }
}


void freeCityList(city *head)
{
    if (head != NULL) {
        city *link = head;
        while (link != NULL)
        {
            city *tmp = link;
            link = link -> next;
            free(tmp);
        }
    } else {
        printf("The list is empty\n");
    }
    free(head);
}

/*
 * Функция посимвольно считывает строку до символа ‘\n’.
 * Функция возвращает считанную строку newStr.
 */

char *getStr ()
{
    char *newStr = NULL;
    int i = -1;
    do
    {
        i++;
        newStr = (char *)realloc(newStr, (i + 1) * sizeof(char));
        memoryCheck(newStr);
    } while((newStr[i] = getchar()) != '\n');
    newStr[i] = '\0';
    return newStr;
}

void printProspList(prospect *head) {
    if (head != NULL) {
        prospect *link = head;
        for (int i = 1; link != NULL; i++, link = link->next) {
            printf("Prosp: Node #%d\nName - [%s]\nNumber - [%d]\n", i, link->name, link->number);
        }
        puts("The end of the list\n");
    } else {
        printf("Your list is empty\n");
    }
}

/*
 * В функцию передается указатель на первый элемент списка head.
	Функция выводит на экран содержимое информационного поля всех элементов списка.
 */
void printCityList(city *head) {
    if (head != NULL) {
        city *link = head;
        for (int i = 1; link != NULL; i++, link = link->next) {
            printf("City: Node #%d\nName - [%s]\nFoundDate - [%u]\nPopulation - [%f]\nNumberOfProspects - [%u]\n", i,
                   link->name, link->foundation, link->population, link->prospNumber);
            printProspList(link -> prosp);
            printf("\n\n\n");
        }
        puts("The end of the list\n");
    } else {
        printf("Your list is empty\n");
    }
}


/*
 * В функцию передается указатель на первый элемент списка head.
	Функция выводит на экран содержимое информационного поля всех элементов списка в обратном порядке.
 */
void printCityListReverse(city *head) {
    if (head != NULL)
    {
        city *link = head;
        int i;
        for (i = 1; link -> next != NULL; i++, link = link -> next);
        do
        {
            printf("Node #%d\nName - [%s]\nFoundDate - [%u]\nPopulation - [%f]\nNumberOfProspects - [%u]\n",
                   i--, link->name, link->foundation, link->population, link->prospNumber);
            link = link -> prev;
        } while(link != NULL);
        puts("The beginning of the list\n");
    } else {
        printf("Your list is empty\n");
    }
}


//numPosition это позиция элемента (его индекс) начала строки/числа в исходной строке "минус" один
unsigned int getIntFromString(char *string, int *firstPosition) {
    unsigned int N = 0;
    int *j = firstPosition, elderPositionU;
    for (elderPositionU = 0; string[++*j] != ' ' && string[*j] != '\0' && string[*j] != '.'; elderPositionU++);

    int k;
    for (k = elderPositionU; k > 1; k--) {
        N += (int) pow(10, k - 1) * (string[*j - k] - '0');
    }
    N += (string[*j - k] - '0');
    return N;
}

//Флоат обязательно должен иметь вид 0.0
float getFloatFromString(char *string, int *numPosition) {
    float F2, F;
    int F1 = 0;
    F2 = F = .0;
    F1 = getIntFromString(string, numPosition);

    int eldPosition = *numPosition;
    F2 = getIntFromString(string, numPosition);
    F2 *= pow(10, -(*numPosition - eldPosition - 1));
    F = F1 + F2;
    return F;
}

char *getSubstringFromString(char *string, int *numPosition) {
    char* substring = NULL;
    int *j = numPosition;
    do {
        (*j)++;
        substring = (char *) realloc(substring, (*j + 1) * sizeof(char));
    } while ((substring[*j] = string[*j]) != ' '); //если строка в конце? проверка на '\0'
    substring[*j] = '\0';
    return substring;
}


prospect *createProspNode(char *string) {
    prospect *link = NULL;
    if (string[0] != '\0') {
        link = (prospect *) malloc(sizeof(prospect));
        int j = -1;
        link -> name = getSubstringFromString(string, &j);
        link -> number = getIntFromString(string, &j);
    }
    return link;
}

prospect *createProsp(char **baseString, int numString, int *countProsp) {
    prospect *head = NULL, *link = NULL;
    if (numString != 0) {
        head = link = createProspNode(baseString[(*countProsp)++]);
        link->prev = NULL;
        for (int i = 1; i < numString; i++, (*countProsp)++) {
            link->next = createProspNode(baseString[*countProsp]);
            link->next->prev = link;
            link = link->next;
        }
        link->next = NULL;
    }
    //тильда
    (*countProsp)++;
    return head;
}


city *createCityNode(char *string, char **baseProspString, int *countProsp) {
    city *link = NULL;
    if (string[0] != '\0') {
        link = (city *) malloc(sizeof(city));
        int j = -1;
        link->name = getSubstringFromString(string, &j);
        link -> foundation = getIntFromString(string, &j);
        link -> population = getFloatFromString(string, &j);
        link -> prospNumber = getIntFromString(string, &j);
        link -> prosp = createProsp(baseProspString, link -> prospNumber, countProsp);
    }
    return link;
}

city *createCity(char **baseString, int numString, char **baseProspString) {
    city *head = NULL, *link = NULL;
    static int countProsp = 0;
    head = link = createCityNode(baseString[0], baseProspString, &countProsp);
    link->prev = NULL;
    for (int i = 1; i < numString; i++) {
        //вычленить кусок массива из общего исходного массива
        link->next = createCityNode(baseString[i], baseProspString, &countProsp);
        link->next->prev = link;
        link = link->next;
    }
    link->next = NULL;
    return head;
}

char **getStrArray (int *numString) {
    char *wayToFile = NULL;
    printf("Enter the way to the file: ");
    wayToFile = getStr();

    FILE *dataFile = NULL;
    char **stringArray = NULL;

    //проверка на успешное открытие файла
    if ((dataFile = fopen(wayToFile, "r")) != NULL) {
        fseek(dataFile, 0, SEEK_END);
        int file_size = ftell(dataFile);
        if (file_size != 0) {
            fseek(dataFile, 0, SEEK_SET);
            int i;
            for (i = 0; !feof(dataFile); i++) {
                stringArray = (char **) realloc(stringArray, (i + 1) * sizeof(char *));
                stringArray[i] = NULL;
                int j = -1;
                do {
                    j++;
                    stringArray[i] = (char *) realloc(stringArray[i], (j + 1) * sizeof(char));
                    memoryCheck(stringArray[i]);
                } while ((stringArray[i][j] = (char) fgetc(dataFile)) != '\n' && stringArray[i][j] != EOF);
                stringArray[i][j] = '\0';
            }
            *numString = i;
            return stringArray;
        }
    }
}

prospect *deleteProsp(prospect *headProsp, int delNumber) {

    if (headProsp != NULL) {
        prospect *link = headProsp;
        printf("P-1\n");
        for (int i = 1; i < delNumber; i++) {
            if (link->next == NULL) return headProsp;
            link = link->next;
        }
        printf("P-2\n");

        if (delNumber == 1) {
            link -> next -> prev = NULL;
            headProsp = link -> next;
        } else if (link -> next == NULL) {
            link -> prev -> next = NULL;
        } else {
            link->prev->next = link->next;
            link->next->prev = link->prev;
        }
        printf("P-3\n");

        free(link->name);
        free(link);
        printf("P-4\n");
    }
    return headProsp;
}

prospect *addProspNode(city *headCity, prospect *headProsp, const char* SET_VALUE) {

    /*
     * Функция добавляет элемент в начало, конец списка, либо после заданного номера в списке.
     * Если список пуст - создает и добавляет в список первый элемент.
     *

    switch (SET_VALUE) {
        case "END":

    }


    prospect *link = headProsp;
    link = createProspNode(link);
    link -> next = NULL;
*/
    return NULL;

}

int main() {

    printf ("\nEnterpoint\n\n");

    int numStringCity = 0, numStringProsp = 0;
    char **stringArrayCity = NULL, **stringArrayProsp = NULL;
    //dataFile = fopen("/Users/artemkaloev/GitControl/CourseMain/COURSEMAIN/dataFile.txt", "r");
    ///Users/artemkaloev/GitControl/CourseMain/COURSEMAIN/dataProspFile.txt
    stringArrayCity = getStrArray(&numStringCity);
    stringArrayProsp = getStrArray(&numStringProsp);

    printf("\nCity: \n");
    for (int i = 0; i < numStringCity; i++) {
        for (int j = 0; stringArrayCity[i][j] != '\0'; j++) {
            printf("%c", stringArrayCity[i][j]);
        }
        printf("\n");
    }

    printf("\nProsp: \n");
    for (int i = 0; i < numStringProsp; i++) {
        for (int j = 0; stringArrayProsp[i][j] != '\0'; j++) {
            printf("%c", stringArrayProsp[i][j]);
        }
        printf("\n");
    }

    city *mainHead = NULL;
    mainHead = createCity(stringArrayCity, numStringCity, stringArrayProsp);

    printCityList(mainHead);
    //printCityListReverse(mainHead);

    printf("Endpoint2\n");

    mainHead -> prosp = deleteProsp(mainHead -> prosp, 1);
    printCityList(mainHead);


    //проблемы: флоат 0 без точки = краш

    printf("Endpoint3\n");



    printf("ENDPOINT\n");
    return 0;
}


//Добавление элемента списказ


