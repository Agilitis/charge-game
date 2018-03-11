#include <stdbool.h>
#include <stdio.h>


void printConsoleMenu(bool testMode){

    printf("Choose: \n");
    printf("1. New game\n");
    printf("2. Highschore \n");
    printf("0. Exit(Also, I am a coward)\n");
    if(testMode)
    {
        printf("\nTest menu: \n");
        printf("3. Save player \n");
    }

}


