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

float getFloatFromString(char *string, int *numPosition) {
    float F1, F2, F;
    F1 = F2 = F = .0;
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

prospect *createProsp(char **baseString, int numString) {
    prospect *head = NULL, *link = NULL;
    head = link = createProspNode(baseString[0]);
    link -> prev = NULL;
    for (int i = 1; i < numString; i++) {
        link->next = createProspNode(baseString[i]);
        link->next->prev = link;
        link = link->next;
    }
    link->next = NULL;
    return head;
}


city *createCityNode(char *string, char **baseProspString, int numProspString) {
    city *link = NULL;
    if (string[0] != '\0') {
        link = (city *) malloc(sizeof(city));
        int j = -1;
        link->name = getSubstringFromString(string, &j);
        link -> foundation = getIntFromString(string, &j);
        link -> population = getFloatFromString(string, &j);
        link -> prospNumber = getIntFromString(string, &j);
        link -> prosp = createProsp(baseProspString, numProspString);
    }
    return link;
}

city *createCity(char **baseString, int numString, char ***baseProspString, int* numProspString) {


    city *head = NULL, *link = NULL;

    head = link = createCityNode(baseString[0], baseProspString[0], numProspString[0]);

    link->prev = NULL;
    for (int i = 1; i < numString; i++) {
        //вычленить кусок массива из общего исходного массива
        link->next = createCityNode(baseString[i], baseProspString[i], numProspString[i]);
        link->next->prev = link;
        link = link->next;
    }

    link->next = NULL;

    return head;
}

/*
city *create(int numNodes) {

    city *head = NULL, *link = NULL;

    head = link = (city *)malloc(sizeof(city));

    printf("Enter the name of the city: ");
    scanf("%s",link -> name);

    printf("Enter the foundation date of %s: ", link -> name);
    scanf("%u", &(link -> foundation));

    printf("Enter the population of %s built in %u: ", link -> name, link -> foundation);
    scanf("%f", &(link -> population));

    printf("Enter the number of prospects in %s (%f mln people) built in %u: ", link -> name, link -> population,
           link -> foundation);
    scanf("%u", &(link -> prospNumber));

    head -> prev = NULL;

    for (int i = 1; i < numNodes; i++) {

        link -> next = (city *)malloc(sizeof(city));
        link -> next -> prev = link;
        link = link -> next;


        printf("Enter the name of the city: ");
        scanf("%s", link -> name);

        printf("Enter the foundation date of %s: ", link -> name);
        scanf("%u", &(link -> foundation));

        printf("Enter the population of %s built in %u: ", link -> name, link -> foundation);
        scanf("%f", &(link -> population));

        printf("Enter the number of prospects in %s (%f mln people) built in %u: ", link -> name, link -> population,
               link -> foundation);
        scanf("%u", &(link -> prospNumber));

    }

    link -> next = NULL;
    return head;
}
*/
/*
char **getTextFromFile() {

    FILE *dataFile = NULL;
    dataFile = fopen("/Users/artemkaloev/GitControl/CourseMain/COURSEMAIN/dataFile.txt", "r");

    char **stringArray = NULL;






}
*/

int main() {


    printf ("\nEnterpoint\n\n");
    city *head = NULL;
    char **cityStringArray = NULL; int num1 = 2;
    cityStringArray = (char **)malloc(num1 * sizeof(char*));
    cityStringArray[0] = "Moscow 801 12.546 3";
    cityStringArray[1] = "SPb 103 127.654 1";

    char ***prospStringArray = NULL;
    int *prospNum = NULL;
    prospNum = (int *)malloc(2 * sizeof(int));
    prospNum[0] = 3;
    prospNum[1] = 1;
    prospStringArray = (char ***)malloc(2 * sizeof(char**));
    prospStringArray[0] = (char **)malloc(3 * sizeof(char*));
    prospStringArray[1] = (char **)malloc(sizeof(char*));


    prospStringArray[0][0] = "Borovaya 21";
    prospStringArray[0][1] = "Moscovskaya 111";
    prospStringArray[0][2] = "Kulich 78";

    prospStringArray[1][0] = "Gorod 222";





    head = createCity(cityStringArray, num1, prospStringArray, prospNum);

    printCityList(head);
    //printCityListReverse(head);

    printf("Endpoint\n");
    return 0;
}

