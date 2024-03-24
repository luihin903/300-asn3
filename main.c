#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "list.h"
#include "PCB.h"
#include "Commands.h"
#include "threads.h"

List* readyQueues[3];
List* sendingQueue;
List* receivingQueue;
char command;
PCB* init;
PCB* running;
int cpuClock = 0;

int main() {

    pthread_t input, os;
    int pid;
    int priority;
    char* msg = (char*)malloc(40 * sizeof(char));

    for (int i = 0; i < 3; i ++) {
        readyQueues[i] = List_create();
    }
    sendingQueue = (List*)malloc(sizeof(List));
    receivingQueue = (List*)malloc(sizeof(List));

    init = (PCB*)malloc(sizeof(PCB));
    init->id = 0;
    init->priority = -1;
    init->state = READY;

    running = init;
    running->state = RUNNING;

    printf("(Process \"init\": { pid : %d })\n", init->id);

    while (true) {

        printf("Simulation$ ");
        scanf(" %c", &command);

        switch (command) {
            case 'C':
                printf("Priority(0, 1, 2): ");
                scanf("%d", &priority);
                Create(priority);
                break;
            case 'F':
                Fork();
                break;
            case 'K':
                printf("pid: ");
                scanf("%d", &pid);
                Kill(pid);
                break;
            case 'E':
                Exit();
                break;
            case 'Q':
                Quantum();
                break;
            case 'S':
                printf("target pid: ");
                scanf("%d", &pid);
                printf("message: ");
                scanf("%s", msg);
                Send(pid, msg);
                break;
            case 'I':
                printf("pid: ");
                scanf("%d", &pid);
                Procinfo(pid);
                break;
            case 'T':
                Totalinfo();
                break;
            default:
                printf("%s \"%c\"\n", "Invalid Command", command);
        }
    }

    return 0;
}