#ifndef PCB_H
#define PCB_H

#include "list.h"

typedef struct Process_Message Message;
// struct Process_Message;
typedef struct Process_Control_Block PCB;
// struct Process_Control_Block;

enum State {
    RUNNING,
    READY,
    BLOCKED
};

struct Process_Message {
    char* content;
    PCB* sender;
};

typedef struct Process_Control_Block PCB;
struct Process_Control_Block {
    int id;
    int priority;
    enum State state;
    Message* msg;
};

extern List* readyQueues[3];
extern PCB* running;
extern PCB* init;

char* State_toString(enum State state) {
    switch (state) {
        case RUNNING: return "RUNNING";
        case READY: return "READY";
        case BLOCKED: return "BLOCKED";
    }
}

bool PCB_comparator(void* process, void* pid) {
    return ((PCB*) process)->id == (uintptr_t) pid;
}

PCB* PCB_find(int pid) {

    if (pid == running->id) return running;
    if (pid == init->id) return init;

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

    return process;
}

#endif