#ifndef COMMANDS_H
#define COMMANDS_H

#include "list.h"
#include "PCB.h"

extern List* readyQueues[3];
extern char command;
extern PCB* init;

int counter = 0;

void Create(int priority) {
    PCB* process = (PCB*)malloc(sizeof(PCB));
    process->id = ++ counter;
    process->priority = priority;
    process->state = READY;
    if (List_append(readyQueues[process->priority], process)) {
        printf("Process create failed");
        counter --;
    }
    else {
        printf("Process (id: %d) created with priority = %d.\n", process->id, process->priority);
    }    
}

void Procinfo(int pid) {
    PCB* process = NULL;
    intptr_t ptr = pid;

    if (process == NULL) {
        List_first(readyQueues[0]);
        process = List_search(readyQueues[0], PCB_comparator, (void*) ptr);
    }
    if (process == NULL) {
        List_first(readyQueues[1]);
        process = List_search(readyQueues[1], PCB_comparator, (void*) ptr);
    }
    if (process == NULL) {
        List_first(readyQueues[2]);
        process = List_search(readyQueues[2], PCB_comparator, (void*) ptr);
    }
    if (process == NULL) {
        printf("Process pid = %d Does Not Exist.\n", pid);
        return;
    }

    printf("Priority: %d\n", process->priority);
}

void Totalinfo() {
    for (int i = 0; i < 3; i ++) {
        List_first(readyQueues[i]);
        List_prev(readyQueues[i]);
        printf("Ready Queue (Priority = %d): [", i);

        for (int j = 0; j < List_count(readyQueues[i]); j ++) {
            PCB* process = List_next(readyQueues[i]);
            printf("{ pid : %d }, ", process->id);
        }

        if (List_count(readyQueues[i]) != 0) printf("\b\b");
        printf("]\n");
    }
}

#endif