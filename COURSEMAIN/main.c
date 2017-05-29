#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
void printStringArray(char **stringArray, int numString);
int compareStrings(char *string1, char *string2);

void freeArray(char** array, int size);
void freeProspList(prospect *head);
void freeCityList(city *head);

void memoryCheck(void *pointer);
char *getStr ();

void printProspList(prospect *head);
void printProspListReverse(prospect *head);
void printCityList(city *head);
void printCityListReverse(city *head);

int getIntFromBeginString(char *string);
unsigned int getIntFromString(char *string, int *firstPosition);
float getFloatFromString(char *string, int *numPosition);
char *getSubstringFromString(char *string, int *numPosition);

prospect *createProspNode(char *string);
prospect *createProsp(char **baseString, int numString, int *countProsp);
city *createCityNode(char *string, char **baseProspString, int *countProsp);
city *createCity(char **baseString, int numString, char **baseProspString);

char **getStrArray(int *numString, int prosp);
char **getStrArrayFromFile (int *numString);
city *getCityListFromConsole();

city *deleteProsp(city *linkCity, int delNumber);
city *deleteIntervalProsp(city* linkCity);
city *deleteCity(city *headCity, int delNumber);
city *deleteIntervalCity(city *headCity);

city *addIntervalProsp(city *linkCity);
city *addIntervalCity(city *headCity);

city *sortProspListNum(city *linkCity);
city *sortProspListString(city *linkCity);
city *sortCityListNum(city *headCity, int input);
city *sortCityListString(city *head);

void putListToFile(city *head);
city *chooseNode(city *head);

void memoryCheck(void *pointer)
{
    if (!pointer)
    {
        printf("No memory");
        exit(1);
    }
}

void freeArray(char** array, int size) {
    if (array != NULL) {
        for (int i = 0; i < size; i++) {
            free(array[i]);
        }
        free(array);
    }
}

void freeProspList(prospect *head) {
    if (head != NULL) {
        prospect *link = head;
        while (link -> next != NULL) {
            link = link -> next;
            free(link -> prev -> name);
            free(link -> prev);
        }
        free(link -> name);
        free(link);
    }
}

void freeCityList(city *head) {
    if (head != NULL) {
        city *link = head;
        while (link -> next != NULL) {
            link = link -> next;
            free(link -> prev -> name);
            freeProspList(link -> prev -> prosp);
            free(link -> prev);
        }
        free(link -> name);
        freeProspList(link -> prosp);
        free(link);
    }
}

char *getStr () {
    /*
    *  Функция посимвольно считывает строку до символа ‘\n’.
    *  Функция возвращает считанную строку newStr.
    */

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

void printProspListReverse(prospect *head) {
    if (head != NULL)
    {
        prospect *link = head;
        int i;
        for (i = 1; link -> next != NULL; i++, link = link -> next);
        do
        {
            printf("Node #%d\nName - [%s]\nNumber - [%u]\n", i--, link->name, link->number);
            link = link -> prev;
        } while(link != NULL);
        puts("The beginning of the list\n");
    } else {
        printf("Your list is empty\n");
    }
}

void printCityList(city *head) {
    /*
    *   В функцию передается указатель на первый элемент списка head.
	*   Функция выводит на экран содержимое информационного поля всех элементов списка.
    */
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

void printCityListReverse(city *head) {
    /*
    *   В функцию передается указатель на первый элемент списка head.
	*   Функция выводит на экран содержимое информационного поля всех элементов списка в обратном порядке.
    */

    if (head != NULL)
    {
        city *link = head;
        int i;
        for (i = 1; link -> next != NULL; i++, link = link -> next);
        do
        {
            printf("Node #%d\nName - [%s]\nFoundDate - [%u]\nPopulation - [%f]\nNumberOfProspects - [%u]\n",
                   i--, link->name, link->foundation, link->population, link->prospNumber);
            printProspList(link -> prosp);
            link = link -> prev;
        } while(link != NULL);
        puts("The beginning of the list\n");
    } else {
        printf("Your list is empty\n");
    }
}

int getIntFromBeginString(char *string) {
    unsigned int N = 0;
    int j = -1, elderPositionU;
    for (elderPositionU = 0; string[++j] != ' ' && string[j] != '\0' && string[j] != '.' && string[j] != '\n'; elderPositionU++);

    int k;
    for (k = elderPositionU; k > 1; k--) {
        N += (int) pow(10, k - 1) * (string[j - k] - '0');
    }
    N += (string[j - k] - '0');
    return N;
}

//numPosition это позиция элемента (его индекс) начала строки/числа в исходной строке "минус" один
unsigned int getIntFromString(char *string, int *firstPosition) {

    /*
     * N - результирующая переменная
     * elderPositionU - количество цифр в числе
     * k - переменная, хранящая значение для возведения 10 в степень
     * firstPosition = j  - текущая позиция элемента в строке (для начала строки равен "-1")
     * string - обрабатываемая строка
     */

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

float getFloatFromString(char *string, int *numPosition) {

    /*
     * F - результирующее число
     * F1 - целая часть вещественного числа
     * F2 - мантисса
     * numPosition - текущая позиция элемента в строке "минус" один
     * eldPosition - позиция элемента, разделяющего мантиссу и целую часть числа
     * string - обрабатываемая строка
     */

    float F2, F;
    int F1 = 0;
    F2 = F = .0;
    F1 = getIntFromString(string, numPosition);

    if (string[*numPosition] == '.') {
        int eldPosition = *numPosition;
        F2 = getIntFromString(string, numPosition);
        F2 *= pow(10, -(*numPosition - eldPosition - 1));
    }
    F = F1 + F2;
    return F;
}

char *getSubstringFromString(char *string, int *numPosition) {

    /*
     * substring - результирующая строка
     * string - исходная строка
     * numPosition - текущая позиция элемента в строке "минус" один
     */

    char* substring = NULL;
    do {
        (*numPosition)++;
        substring = (char *) realloc(substring, (*numPosition + 1) * sizeof(char));
        memoryCheck(substring);
    } while ((substring[*numPosition] = string[*numPosition]) != ' ' && substring[*numPosition] != '\n');
    substring[*numPosition] = '\0';
    return substring;
}

prospect *createProspNode(char *string) {

        //ограниченное кол-о проспнодов
    prospect *link = NULL;
    if (string[0] != '\0') {
        link = (prospect *) malloc(sizeof(prospect));
        memoryCheck(link);
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
        memoryCheck(link);
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
    city *head = NULL;
    if (baseString != NULL && baseProspString != NULL) {
        city *link;
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
    }
    return head;
}

char **getStrArray(int *numString, int prosp) {
    char **stringArray = NULL;
    *numString = getIntFromBeginString(getStr());

    puts("Enter strings to form the list");
    int i;
    for (i = 0; i < *numString; i++) {
        stringArray = (char **) realloc(stringArray, (i + 1) * sizeof(char *));
        memoryCheck(stringArray);
        //stringArray[i] = NULL;
        stringArray[i] = getStr();
    }
    if (prosp) {
        stringArray = (char**)realloc(stringArray, (i + 1) * sizeof(char*));
        memoryCheck(stringArray);
        stringArray[i] = "~\n";
        (*numString)++;
    }
    return stringArray;
}

char **getStrArrayFromFile (int *numString) {
    printf("Enter the way to the file: ");
    FILE *dataFile = NULL;
    char **stringArray = NULL;
    if ((dataFile = fopen(getStr(), "r")) != NULL) {
        fseek(dataFile, 0, SEEK_END);
        if (ftell(dataFile) != 0) {
            fseek(dataFile, 0, SEEK_SET);
            int i;
            for (i = 0; !feof(dataFile); i++) {
                stringArray = (char **) realloc(stringArray, (i + 1) * sizeof(char *));
                memoryCheck(stringArray);
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
        }
        fclose(dataFile);
    }
    return stringArray;
}

city *getCityListFromConsole() {
    printf("Warning!\n"
                   "Print 'city' data in format 'Name FoundationDate Population NumberOfProspect'\n"
                   "Print 'prospect' data in format 'Name Number'\n"
                   "The use of another input format entails errors and an emergency program termination\n");
    city *head = NULL;
    int cityNum = 0, prospNum1 = 0, prospNum2 = 0;
    char **cityStringArray = NULL, **prospStringArray1 = NULL, **prospStringArray2 = NULL;
    printf("Enter number of cities: ");
    cityStringArray = getStrArray(&cityNum, 0);
    if (cityNum != 0) {
        printf("Enter number of prospects in the city #1: ");
        prospStringArray1 = getStrArray(&prospNum1, 1);
        for (int i = 1; i < cityNum; i++) {
            prospNum2 = 0;
            printf("Enter number of prospects in the city #%i: ", i + 1);
            prospStringArray2 = getStrArray(&prospNum2, 1);
            prospStringArray1 = sumArrays(prospStringArray1, prospStringArray2, prospNum1, prospNum2);
            prospNum1 += prospNum2;
            char **tmp = prospStringArray2;
            prospStringArray2 = NULL;
            free(tmp);
        }
        head = createCity(cityStringArray, cityNum, prospStringArray1);
    }
    return head;
}

city *deleteProsp(city *linkCity, int delNumber) {
    prospect *headProsp = linkCity -> prosp;
    if (headProsp != NULL) {
        prospect *link = headProsp;
        for (int i = 1; i < delNumber; i++) {
            if (link->next == NULL) return linkCity;
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
    return linkCity;
}

city *deleteIntervalProsp(city* linkCity) {

    printf("Enter the 1st position: ");
    int setPosition1 = getIntFromBeginString(getStr());

    printf("Enter the 2st position: ");
    int setPosition2 = getIntFromBeginString(getStr());

    for (int i = setPosition1; i <= setPosition2; i++) {
        linkCity = deleteProsp(linkCity, setPosition1);
        printProspList(linkCity -> prosp);
    }
    return linkCity;
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
        //link = deleteIntervalProsp(link);
        //free(link -> prosp);
        freeProspList(headCity -> prosp);
        free(link->name);
        free(link);
    }
    return headCity;
}

city *deleteIntervalCity(city *headCity) {

    printf("Enter the 1st position: ");
    int setPosition1 = getIntFromBeginString(getStr());

    printf("Enter the 2st position: ");
    int setPosition2 = getIntFromBeginString(getStr());

    for (int i = setPosition1; i <= setPosition2; i++) {
        headCity = deleteCity(headCity, setPosition1);
        printCityList(headCity);
    }
    return headCity;

}

city *addIntervalProsp(city *linkCity) {
    if (linkCity != NULL) {
        //numAddNodes - кол-во проспектов, которые добавятся в список, positiontoAdd - позиция, после которой будет всталвен элемент
        printf("Enter the position of node after which nodes will be added: ");
        int numberAddNodes, SET_POSITION = 0, positionToAdd = getIntFromBeginString(getStr());
        printf("Enter the number of nodes that will be added to the prospect list: ");
        prospect *subHead = createProsp(getStrArray(&numberAddNodes, 0), numberAddNodes, &SET_POSITION);
        //printProspList(subHead);

        prospect *subTail;
        for (subTail = subHead; subTail->next != NULL; subTail = subTail->next);
        prospect *link = linkCity->prosp;

        if (link != NULL) {
            for (int i = 1; i < positionToAdd && link->next != NULL; link = link->next, i++);
        }

        if (positionToAdd == 0 || link == NULL) {
            subTail->next = linkCity->prosp;
            if (linkCity->prosp != NULL) {
                linkCity->prosp->prev = subTail;
            }
            linkCity->prosp = subHead;
        } else if (link->next == NULL) {
            link->next = subHead;
            subHead->prev = link;
        } else {
            subTail->next = link->next;
            link->next->prev = subTail;
            subHead->prev = link;
            link->next = subHead;
        }
        (linkCity -> prospNumber) += numberAddNodes;
    } else {
        puts("Cannot add prospects to the empty city list");
    }
    return linkCity;
}

city *addIntervalCity(city *headCity) {
    printf("Enter the position of node after which nodes will be added: ");
    int positionToAdd = getIntFromBeginString(getStr());
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

/*
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
*/

city *sortProspListNum(city *linkCity) {
    prospect *link = linkCity->prosp;
    for (; link != NULL; link = link->next) { // Пока не прошли список целиком...
        if (link->next != NULL) { // Если в списке не один элемент...
            if (link->next->number < link->number) { // Сравниваем первый со вторым.
                prospect *tmp = link->next; // Создаем второй.
                link->next = tmp->next; // Если надо поменять их местами, меняем.
                if (tmp->next != NULL) {
                    tmp->next->prev = link;
                }
                tmp->prev = link->prev;
                tmp->next = link;
                if (link == linkCity->prosp) {
                    linkCity -> prosp = tmp;
                } else {
                    link->prev->next = tmp;
                }
                link->prev = tmp;
                linkCity = sortProspListNum(linkCity); // Рекурсивно вызываем функцию на повторную проверку, если мы меняли их местами.
            }
        }
    }
    return linkCity;
}

city *sortProspListString(city *linkCity) {
    prospect *link = linkCity->prosp;
    for (; link != NULL; link = link->next) { // Пока не прошли список целиком...
        if (link->next != NULL) { // Если в списке не один элемент...
            if (compareStrings(link->name, link->next->name) == 2) {
                prospect *tmp = link->next; // Создаем второй.
                link->next = tmp->next; // Если надо поменять их местами, меняем.
                if (tmp->next != NULL) {
                    tmp->next->prev = link;
                }
                tmp->prev = link->prev;
                tmp->next = link;
                if (link == linkCity->prosp) {
                    linkCity -> prosp = tmp;
                } else {
                    link->prev->next = tmp;
                }
                link->prev = tmp;
                linkCity = sortProspListString(linkCity); // Рекурсивно вызываем функцию на повторную проверку, если мы меняли их местами.
            }
        }
    }
    return linkCity;
}

city *sortCityListNum(city *headCity, int input) {
    float field1 = 0, field2 = 0;
    city *link = headCity;
    for (; link != NULL; link = link->next) {
        if (link->next != NULL) {
            switch (input) {
                case 1 :
                    field1 = (float) link->foundation;
                    field2 = (float) link->next->foundation;
                    break;
                case 2 :
                    field1 = (float) link->prospNumber;
                    field2 = (float) link->next->prospNumber;
                    break;
                case 3 :
                    field1 = link->population;
                    field2 = link->next->population;
                    break;
                default:
                    ("No field to sort. Enter the field: \n");
                    input = getIntFromBeginString(getStr());
                    break;
            }
            if (field2 < field1) {
                city *tmp = link->next;
                link->next = tmp->next;
                if (tmp->next != NULL) {
                    tmp->next->prev = tmp;
                }
                tmp->prev = link->prev;
                tmp->next = link;
                if (link == headCity) {
                    headCity = tmp;
                } else {
                    link->prev->next = tmp;
                }
                link->prev = tmp;
                headCity = sortCityListNum(headCity, input); // Отмечаем тот факт, что мы поменяли их местами.
            }
        }
    }

    return headCity;
}

city *sortCityListString(city *head) {
    city *link = head;
    for (; link != NULL; link = link->next) { // Пока не прошли список целиком...
        if (link->next != NULL) { // Если в списке не один элемент...
            if (compareStrings(link->name, link->next->name) == 2) {
                city *tmp = link->next; // Создаем второй.
                link->next = tmp->next; // Если надо поменять их местами, меняем.
                if (tmp->next != NULL) {
                    tmp->next->prev = link;
                }
                tmp->prev = link->prev;
                tmp->next = link;
                if (link == head) {
                    head = tmp;
                } else {
                    link->prev->next = tmp;
                }
                link->prev = tmp;
                head = sortCityListString(head); // Рекурсивно вызываем функцию на повторную проверку, если мы меняли их местами.
            }
        }
    }
    return head;
}

void putListToFile(city *head) {

    FILE *dataFileCity = NULL, *dataFileProsp = NULL;
    printf("Enter the way to the 'city' file: ");
    dataFileCity = fopen(getStr(),"w");

    printf("Enter the way to the 'prospect' file: ");
    dataFileProsp = fopen(getStr(), "w");

    city *link = head;
    for(; link != NULL; link = link -> next) {
        for (int i = 0; link->name[i] != '\0'; i++) {
            fputc(link -> name[i], dataFileCity);
        }
        fprintf(dataFileCity, " %u %f %u", link->foundation, link->population, link->prospNumber);
        if (link -> next != NULL) fprintf(dataFileCity, "\n");


        prospect *linkProsp = link -> prosp;
        for (; linkProsp != NULL; linkProsp = linkProsp->next) {
            for (int i = 0; linkProsp->name[i] != '\0'; i++) {
                fputc(linkProsp -> name[i], dataFileProsp);
            }
            fprintf(dataFileProsp, " %u\n", linkProsp->number);

        }
        fprintf(dataFileProsp, "~");
        if (link -> next != NULL) fprintf(dataFileProsp, "\n");
    }

    fclose(dataFileCity);
    fclose(dataFileProsp);
    puts("Changes are saved");
}

//возвращает (1), если 1-ая строка должна стоять по алфавиту раньше 2-ой, иначе возвращает (2), если наоборот.
//если строки равны, возвращает 0
int compareStrings(char *string1, char *string2) {
    int i;
    for (i = 0; string1[i] != '\0' || string2[i] != '\0'; i++) {
        if (string1[i] > string2[i]) return 2;
        else if (string1[i] < string2[i]) return  1;
    }
    if (strlen(string1) < strlen(string2)) return 1;
    else if (strlen(string1) > strlen(string2)) return 2;
    else return 0;
}

char **sumArrays(char **firstArray, char **secondArray, int firstNum, int secondNum) {
    for (int i = 0; i < secondNum; i++) {
        firstArray = (char **) realloc(firstArray, (firstNum + i + 1) * sizeof(char *));
        memoryCheck(firstArray);
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

city *chooseNode(city *head) {
    city *link = head;
    if (head != NULL) {
        printf("Choose the city node in which prospect list will be changed: ");
        int selected = getIntFromBeginString(getStr());
        for (int i = 1; i < selected && link -> next != NULL; link = link->next, i++);
        printf("You choosed the city '%s'", link -> name);
    } else {
        puts("City list is empty!");
    }
    return link;
}

int main() {

    city *head = NULL;
    char **cityStringArray = NULL, **prospStringArray = NULL;
    int cityStringNum, prospStringNum;

    int input = 1;
    while (input != 0) {
        printf("Main menu. Choose one point from the menu below\n\n"
                       "Press (1) to create list\n"
                       "Press (2) to add node(-s)\n"
                       "Press (3) to delete node(-s)\n"
                       "Press (4) to sort nodes\n"
                       "Press (5) to print list\n"
                       "Press (6) to save changes to the file\n"
                       "Press (0) to finish the program\n");
        input = getIntFromBeginString(getStr());
        switch (input) {
            case 1:
                if (head != NULL) {
                    puts("The list has been already created. Deleting that and continue . . .");
                    freeCityList(head);
                    head = NULL;
                }
                puts("Pressed 1\n");
                printf("Submenu 1. Choose one point from the menu below\n"
                               "Press (1) to create list using console\n"
                               "Press (2) to create list using the files\n"
                               "Press any other key to back to the main menu\n");
                int subInput1;
                subInput1 = getIntFromBeginString(getStr());
                switch (subInput1) {
                    case 1:
                        head = getCityListFromConsole();
                        printCityList(head);
                        break;
                    case 2:
                        head = createCity(getStrArrayFromFile(&cityStringNum), cityStringNum,
                                          getStrArrayFromFile(&prospStringNum));
                        printCityList(head);
                        break;
                    default:
                        puts("Return to the main menu . . .\n");
                        break;
                }
                break;
            case 2:
                puts("Pressed 2\n");
                printf("Submenu 2. Choose one point from the menu below\n"
                               "Press (1) to add node(-s) to the prospect list\n"
                               "Press (2) to add node(-s) to the city list\n"
                               "Press any other key to back to the main menu\n");
                int subInput2;
                subInput2 = getIntFromBeginString(getStr());
                switch (subInput2) {
                    case 1:
                        head = addIntervalProsp(chooseNode(head));
                        printCityList(head);
                        break;
                    case 2:
                        head = addIntervalCity(head);
                        printCityList(head);
                        break;
                    default:
                        puts("Return to the main menu . . .\n");
                        break;
                }
                break;
            case 3:
                puts("Pressed 3\n");
                printf("Submenu 3. Choose one point from the menu below\n"
                               "Press (1) to delete node(-s) to the prospect list\n"
                               "Press (2) to delete node(-s) to the city list\n"
                               "Press any other key to back to the main menu\n");
                int subInput3;
                subInput3 = getIntFromBeginString(getStr());
                switch (subInput3) {
                    case 1:
                        head = deleteIntervalProsp(chooseNode(head));
                        printCityList(head);
                        break;
                    case 2:
                        head = deleteIntervalCity(head);
                        printCityList(head);
                        break;
                    default:
                        puts("Return to the main menu . . .");
                        break;
                }
                break;
            case 4:
                puts("Pressed 4\n");
                printf("Submenu 4. Choose one point from the menu below\n"
                               "Press (1) to sort nodes by the field 'name' in the city list\n"
                               "Press (2) to sort nodes by the field 'foundation' in the city list\n"
                               "Press (3) to sort nodes by the field 'number of prospects' in the city list\n"
                               "Press (4) to sort nodes by the field 'population' in the city list\n"
                               "Press (5) to sort nodes by the field 'name' in the prospect list\n"
                               "Press (6) to sort nodes by the field 'number' in the prospect list\n"
                               "Press any other key to back to the main menu\n");
                int subInput4;
                subInput4 = getIntFromBeginString(getStr());
                switch (subInput4) {
                    case 1:
                        head = sortCityListString(head);
                        printCityList(head);
                        break;
                    case 2:
                        head = sortCityListNum(head, 1);
                        printCityList(head);
                        break;
                    case 3:
                        head = sortCityListNum(head, 2);
                        printCityList(head);
                        break;
                    case 4:
                        head = sortCityListNum(head, 3);
                        printCityList(head);
                        break;
                    case 5:
                        head = sortProspListString(chooseNode(head));
                        printCityList(head);
                        break;
                    case 6:
                        head = sortProspListNum(chooseNode(head));
                        printCityList(head);
                        break;
                    default:
                        break;
                }
                break;
            case 5:
                puts("Pressed 5\n");
                printf("Submenu 5. Choose one point from the menu below\n"
                               "Press (1) to print city list\n"
                               "Press (2) to print prospect list\n"
                               "Press (3) to print city list reverse\n"
                               "Press (4) to print prospect list reverse\n"
                               "Press any other key to back to the main menu\n");
                int subInput5;
                subInput5 = getIntFromBeginString(getStr());
                switch (subInput5) {
                    case 1:
                        printCityList(head);
                        break;
                    case 2:
                        printProspList(chooseNode(head)->prosp);
                        break;
                    case 3:
                        printCityListReverse(head);
                        break;
                    case 4:
                        printProspListReverse(chooseNode(head)->prosp);
                        break;
                    default:
                        break;
                }
                break;
            case 6:
                puts("Pressed 6\n");
                putListToFile(head);
                break;
            case 0:
                puts("Pressed 0. Finish the program\n");
                freeCityList(head);
                break;
            default:
                break;
        }
    }
    return 0;
}