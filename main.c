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
Semaphore* semaphores[5];
char command;
PCB* init;
PCB* running;
int cpuClock = 0;

int main() {

    pthread_t input, os;
    int arg, arg2;
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
                scanf("%d", &arg);
                Create(arg);
                break;
            case 'F':
                Fork();
                break;
            case 'K':
                printf("pid: ");
                scanf("%d", &arg);
                Kill(arg);
                break;
            case 'E':
                Exit();
                break;
            case 'Q':
                Quantum();
                break;
            case 'S':
                printf("target pid: ");
                scanf("%d", &arg);
                printf("message: ");
                scanf("%s", msg);
                Send(arg, msg);
                break;
            case 'R':
                Receive();
                break;
            case 'Y':
                printf("target pid: ");
                scanf("%d", &arg);
                printf("message: ");
                scanf("%s", msg);
                Reply(arg, msg);
                break;
            case 'N':
                printf("sid(0~4): ");
                scanf("%d", &arg);
                printf("initial value(>= 0): ");
                scanf("%d", &arg2);
                NewSemaphore(arg, arg2);
                break;
            case 'P':
                printf("sid: ");
                scanf("%d", &arg);
                SemaphoreP(arg);
                break;
            case 'V':
                printf("sid: ");
                scanf("%d", &arg);
                SemaphoreV(arg);
                break;
            case 'I':
                printf("pid: ");
                scanf("%d", &arg);
                Procinfo(arg);
                break;
            case 'T':
                Totalinfo();
                break;
            default:
                printf("Invalid Command \"%c\"\n", command);
        }
    }

    return 0;
}