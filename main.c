#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "list.h"
#include "PCB.h"
#include "Commands.h"
#include "threads.h"

List* readyQueues[3];
char command;
PCB* init;

int main() {

    pthread_t input, os;

    for (int i = 0; i < 3; i ++) {
        readyQueues[i] = List_create();
    }

    init = (PCB*)malloc(sizeof(PCB));
    init->id = 0;
    init->state = READY;

    pthread_create(&input, NULL, inputFunction, NULL);
    pthread_create(&os, NULL, osFunction, NULL);

    while (true);

    return 0;
}