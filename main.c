#include <stdio.h>
#include "list.h"

int main() {

    List* readyQueue;
    readyQueue = List_create();

    char command;

    while (true) {

        printf("Simulation$ ");
        scanf(" %c", &command);

        switch (command) {
            case 'C':
                printf("%s\n", "got C");
                break;
            default:
                printf("%s\n", "Invalid Command");
        }
    }

    return 0;
}