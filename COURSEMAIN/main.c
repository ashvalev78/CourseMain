#include <stdio.h>
#include <stdlib.h>


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


/*
 * В функцию передается указатель на первый элемент списка head.
	Функция выводит на экран содержимое информационного поля всех элементов списка.
 */
void printCityList(city *head) {

    if (head != NULL) {
        city *link = head;

        for (int i = 1; link != NULL; i++, link = link->next) {

            printf("Node #%d Name - [%s]\nFoundDate - [%u]\nPopulation - [%f]\nNumberOfProspects - [%u]\n",
                   i++, link->name, link->foundation, link->population, link->prospNumber);

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
            printf("Node #%d Name - [%s]\nFoundDate - [%u]\nPopulation - [%f]\nNumberOfProspects - [%u]\n",
                   i--, link->name, link->foundation, link->population, link->prospNumber);
            link = link -> prev;
        } while(link != NULL);
        puts("The beginning of the list\n");
    } else {
        printf("Your list is empty\n");
    }
}

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

/*
char **getTextFromFile() {

    FILE *dataFile = NULL;
    dataFile = fopen("/Users/artemkaloev/GitControl/CourseMain/COURSEMAIN/dataFile.txt", "r");

    char **stringArray = NULL;






}
*/

int main() {
    printf("Hello, World!\n");
    printf("Artem Commit\n");

    printf ("Enterpoint\n");

    int num = 3;
    city *head = NULL;
    head = create(num);

    printCityList(head);
    printCityListReverse(head);

    printf("Endpoint\n");
    //
    return 0;
}

