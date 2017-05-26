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

char **sumArrays(char **firstArray, char** secondArray, int firstNum, int SecondNum);

city *getCityListFromConsole();

void printStringArray(char **stringArray, int numString);

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
    for (elderPositionU = 0; string[++*j] != ' ' && string[*j] != '\0' && string[*j] != '.' && string[*j] != '\n'; elderPositionU++);

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

//numString - количество проспектов для данного города,  countProsp - номер элемента, с которого начинается считывание всего массива строк
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
    int countProsp = 0;
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

char **getStrArray(int *numString, int prosp) {
    int SET_POSITION = -1;
    char **stringArray = NULL;
    printf("Enter number of nodes: ");
    *numString = getIntFromString(getStr(), &SET_POSITION);
    printf("%d\n", *numString);
    int i;
    for (i = 0; i < *numString; i++) {
        stringArray = (char **) realloc(stringArray, (i + 1) * sizeof(char *));
        stringArray[i] = NULL;
        stringArray[i] = getStr();
    }
    if (prosp) {
        stringArray = (char**)realloc(stringArray, (i + 1) * sizeof(char*));
        stringArray[i] = "~\n";
        (*numString)++;
    }
    return stringArray;
}

char **getStrArrayFromFile (int *numString) {
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

prospect *deleteProsp(city *linkCity, int delNumber) {
    prospect *headProsp = linkCity -> prosp;
    if (headProsp != NULL) {
        prospect *link = headProsp;
        for (int i = 1; i < delNumber; i++) {
            if (link->next == NULL) return headProsp;
            link = link->next;
        }
        if (delNumber == 1) {
            if (link -> next == NULL) {
                headProsp = NULL;
            } else {
                link->next->prev = NULL;
                headProsp = link->next;
            }
        } else if (link -> next == NULL) {
            link -> prev -> next = NULL;
        } else {
            link->prev->next = link->next;
            link->next->prev = link->prev;
        }
        (linkCity -> prospNumber)--;
        free(link->name);
        free(link);
    }
    return headProsp;
}

prospect *deleteIntervalProsp(city* linkCity, int setPosition1, int setPosition2) {

    for (int i = setPosition1; i <= setPosition2; i++) {
        linkCity -> prosp = deleteProsp(linkCity, setPosition1);
        printProspList(linkCity -> prosp);
    }
    return linkCity -> prosp;
}

city *deleteCity(city *headCity, int delNumber) {
    if (headCity != NULL) {
        city *link = headCity;
        for (int i = 1; i < delNumber; i++) {
            if (link->next == NULL) return headCity;
            link = link->next;
        }
        if (delNumber == 1) {
            if (link -> next == NULL) {
                headCity = NULL;
            } else {
                link->next->prev = NULL;
                headCity = link->next;
            }
        } else if (link -> next == NULL) {
            link -> prev -> next = NULL;
        } else {
            link->prev->next = link->next;
            link->next->prev = link->prev;
        }
        link -> prosp = deleteIntervalProsp(link, 1, link -> prospNumber);
        free(link->name);
        free(link);
    }
    return headCity;
}

city *deleteIntervalCity(city *headCity, int setPosition1, int setPosition2) {

    for (int i = setPosition1; i <= setPosition2; i++) {
        headCity = deleteCity(headCity, setPosition1);
        printCityList(headCity);
    }
    return headCity;

}

city *addIntervalProsp(city *linkCity) {
    //numAddNodes - кол-во проспектов, которые добавятся в список, positiontoAdd - позиция, после которой будет всталвен элемент
    printf("Enter the position of node after which nodes will be added: ");
    int numberAddNodes, SET_POSITION_1 = -1, SET_POSITION_2 = 0, positionToAdd = getIntFromString(getStr(), &SET_POSITION_1);
    prospect *subHead = createProsp(getStrArray(&numberAddNodes, 0), numberAddNodes, &SET_POSITION_2);
    printProspList(subHead);

    prospect *subTail;
    for (subTail = subHead; subTail -> next != NULL; subTail = subTail -> next);
    prospect *link = linkCity -> prosp;

    if (link != NULL) {
        for (int i = 1; i < positionToAdd && link->next != NULL; link = link->next, i++);
    }

    if (positionToAdd == 0 || link == NULL) {
        subTail -> next = linkCity -> prosp;
        if (linkCity -> prosp != NULL) {
            linkCity -> prosp -> prev = subTail;
        }
        linkCity -> prosp = subHead;
    } else if (link -> next == NULL) {
        link -> next = subHead;
        subHead -> prev = link;
    } else {
        subTail -> next = link -> next;
        link -> next -> prev = subTail;
        subHead -> prev = link;
        link -> next = subHead;
    }
    return linkCity;
}

city *addIntervalCity(city *headCity) {
    printf("Enter the position of node after which nodes will be added: ");
    int SET_POSITION_1 = -1, positionToAdd = getIntFromString(getStr(), &SET_POSITION_1);
    city *subHead = getCityListFromConsole();
    printCityList(subHead);

    city *subTail;
    for (subTail = subHead; subTail -> next != NULL; subTail = subTail -> next);

    city *link = headCity;
    if (link != NULL) {
        for (int i = 1; i < positionToAdd && link->next != NULL; link = link->next, i++);
    }

    if (positionToAdd == 0 || link == NULL) {
        subTail -> next = headCity;
        if (headCity != NULL) {
            headCity -> prev = subTail;
        }
        headCity = subHead;
    } else if (link -> next == NULL) {
        link -> next = subHead;
        subHead -> prev = link;
    } else {
        subTail -> next = link -> next;
        link -> next -> prev = subTail;
        subHead -> prev = link;
        link -> next = subHead;
    }
    return headCity;

}

prospect *addProsp(city* linkCity, int addNumber, prospect *addNode) {
    prospect *head = linkCity -> prosp;
    prospect *link = head;
    if (head != NULL) {
        for (int i = 1; i < addNumber; i++) {
            if (link->next == NULL) break;
            link = link->next;
        }
        if (link->next == NULL) {
            link->next = addNode;
            link->next->prev = link;
            link->next->next = NULL;
        } else {
            prospect *tmp = link -> next;
            link -> next = addNode;
            link -> next -> prev = link;
            link -> next -> next = tmp;
            tmp -> prev = link -> next;
        }
    } else {
        link = addNode;
        link -> next = NULL;
        link -> prev = NULL;
    }
    link = link -> next;
    (linkCity -> prospNumber)++;
    return head;
}

/*
prospect *addIntervalProsp(city *linkCity, int addNumber, int numAddNodes, char **strAddNodes) {

    for (int i = 0; i < numAddNodes; i++) {
        linkCity -> prosp = addProsp(linkCity, addNumber, strAddNodes[i]);
        printProspList(linkCity -> prosp);
    }
    return linkCity -> prosp;
}
*/

city *getCityListFromConsole() {
    city *head = NULL;
    int cityNum = 0, prospNum1 = 0, prospNum2 = 0;
    char **cityStringArray = NULL, **prospStringArray1 = NULL, **prospStringArray2 = NULL;
    cityStringArray = getStrArray(&cityNum, 0);
    if (cityNum != 0) {
        printf("cityNum != 0\n");
        prospStringArray1 = getStrArray(&prospNum1, 1);
        printStringArray(prospStringArray1, prospNum1);

        for (int i = 1; i < cityNum; i++) {
            prospNum2 = 0;
            prospStringArray2 = getStrArray(&prospNum2, 1);
            printStringArray(prospStringArray2, prospNum2);
            prospStringArray1 = sumArrays(prospStringArray1, prospStringArray2, prospNum1, prospNum2);
            prospNum1 += prospNum2;
            printStringArray(prospStringArray1, prospNum1);

        }

        printf("ЩА БУИТ МЯСО! \n---------------\n");

        printStringArray(cityStringArray, cityNum);
        printStringArray(prospStringArray1, prospNum1);

        printf("---------------------------ЩА БУИТ МЯСО!\n");
        printf("cityNUM = %d", cityNum);

        head = createCity(cityStringArray, cityNum, prospStringArray1);

    }
    return head;
}

char **sumArrays(char **firstArray, char **secondArray, int firstNum, int secondNum) {
    for (int i = 0; i < secondNum; i++) {
        firstArray = (char **) realloc(firstArray, (firstNum + i + 1) * sizeof(char *));
        firstArray[firstNum + i] = secondArray[i];
    }
    return firstArray;
}

void printStringArray(char **stringArray, int numString) {

    for (int i = 0; i < numString; i++) {
        for (int j = 0; stringArray[i][j] != '\0'; j++) {
            printf("%c", stringArray[i][j]);
        }
        printf("\n");
    }
}

int main() {


    int input = 1;
    printf("Hello! It's enterpoint of the program. Choose one point from the menu below\n\n"
                   "Press (1) to add node(-s)\n"
                   "Press (2) to delete node(-s)\n"
                   "Press (3) to sort nodes\n"
                   "Press (4) to save changes to the file\n"
                   "Press (0) to finish the program\n");
    while (input != 0) {
        int SET_POSITION = -1;
        input = getIntFromString(getStr(), &SET_POSITION);
        switch (input) {
            case 1:
                SET_POSITION = -1;
                printf("Pressed 1\n");
                city *head = NULL;
                head = getCityListFromConsole();
                printCityList(head);
                head = addIntervalProsp(head);
                printProspList(head -> prosp);
                break;
            case 2:
                printf("Pressed 2\n");
                city *head1 = NULL;
                head1 = getCityListFromConsole();
                printCityList(head1);
                head1 = addIntervalCity(head1);
                printf("ending\n");
                printCityList(head1);
                break;
            case 0:
                printf("Pressed 0. Finish the program\n");
                exit(1);
            default:
                break;
        }

    }





    printf ("\nEnterpoint\n\n");

    char **cityStringBase = NULL;
    int numCityStringArr;
    //cityStringBase = getStrArray(&numCityStringArr);
    printf("\nCity: \n");
    for (int i = 0; i < numCityStringArr; i++) {
        for (int j = 0; cityStringBase[i][j] != '\0'; j++) {
            printf("%c", cityStringBase[i][j]);
        }
        printf("\n");
    }

    char **prospectStringBase = NULL;
    int numProspStringArr;
    //prospectStringBase = getStrArray(&numProspStringArr);
    printf("\nProsp: \n");
    for (int i = 0; i < numProspStringArr; i++) {
        for (int j = 0; prospectStringBase[i][j] != '\0'; j++) {
            printf("%c", prospectStringBase[i][j]);
        }
        printf("\n");
    }

    printf("Endpoint5\n");

    city *head = NULL;
    head = createCity(cityStringBase, numCityStringArr, prospectStringBase);
    printCityList(head);

    printf("endpoint6\n");
    printf("Enter the new node: ");
    head ->prosp = addProsp(head -> prosp, 1, getStr());
    printProspList(head -> prosp);


    int numStringCity = 0, numStringProsp = 0;
    char **stringArrayCity = NULL, **stringArrayProsp = NULL;
    /*
    /Users/artemkaloev/GitControl/CourseMain/COURSEMAIN/dataFile.txt
    /Users/artemkaloev/GitControl/CourseMain/COURSEMAIN/dataProspFile.txt
     */

    stringArrayCity = getStrArrayFromFile(&numStringCity);
    stringArrayProsp = getStrArrayFromFile(&numStringProsp);

    city *mainHead = NULL;
    mainHead = createCity(stringArrayCity, numStringCity, stringArrayProsp);

    printCityList(mainHead);
    //printCityListReverse(mainHead);

    printf("Endpoint2\n");

    int setPosition1 = 1, setPosition2 = 1;

    mainHead = deleteIntervalCity(mainHead,setPosition1, setPosition2);

    //проблемы: флоат 0 без точки = краш

    printf("Endpoint3\n");
    printCityList(mainHead);

    printf("Endpoint4\n");
    mainHead -> prosp = deleteProsp(mainHead, 1);
    printCityList(mainHead);






    /*
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
     */

    printf("ENDPOINT\n");
    return 0;
}


//Добавление элемента списказ


