#include <stdio.h>
#include "list.h"
#include "PCB.h"
#include "Commands.h"

int main() {

    List* readyQueues[3];

    for (int i = 0; i < 3; i ++) {
        readyQueues[i] = List_create();
    }

    char command;

    while (true) {

        printf("Simulation$ ");
        scanf(" %c", &command);

        switch (command) {
            case 'C':
                int priority = -1;
                while (priority != 0 || priority != 1 || priority != 2) {
                    printf("Priority(0, 1, 2): \n");
                    scanf("%d", &priority);
                }
                Create(priority, readyQueues);
                break;
            default:
                printf("%s \"%c\"\n", "Invalid Command", command);
        }
    }

    return 0;
}