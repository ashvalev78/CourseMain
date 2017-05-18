#include <stdio.h>
#include <stdlib.h>


typedef struct prospect {

    char *name;
    unsigned int number;
    struct prospect *next, *prev;

} prospect;

typedef struct city {

    unsigned int foundation, prospNumber;
    unsigned float population;
    char *name;
    prospect *prosp;
    struct city *next, *prev;

} city;



char **getTextFromFile() {

    FILE *dataFile = NULL;
    dataFile = fopen("/Users/artemkaloev/GitControl/CourseMain/COURSEMAIN/dataFile.txt", "r");

    char **stringArray = NULL;






}

int main() {
    printf("Hello, World!\n");
    printf("Artem Commit\n");


    printf ("Enterpoint\n");
    //
    return 0;
}

