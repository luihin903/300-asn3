#ifndef PCB_H
#define PCB_H

#include "list.h"

typedef enum Process_State State;
typedef enum Blocking_Source Source;
typedef struct Process_Message Message;
typedef struct Process_Control_Block PCB;
typedef struct Semaphore Semaphore;

extern List* readyQueues[3];
extern List* sendingQueue;
extern List* receivingQueue;
extern Semaphore* semaphores[5];
extern PCB* running;
extern PCB* init;

enum Process_State {
    RUNNING,
    READY,
    BLOCKED
};

enum Blocking_Source {
    SENDING,
    RECEIVING,
    SEMAPHORE,
    NONE
};

struct Process_Message {
    char* content;
    PCB* sender;
};

struct Process_Control_Block {
    int id;
    int priority;
    State state;
    Message* msg;
    Source source;
};

struct Semaphore {
    int id;
    int value;
    List* plist;
};

char* State_toString(State state) {
    switch (state) {
        case RUNNING: return "RUNNING";
        case READY: return "READY";
        case BLOCKED: return "BLOCKED";
    }
}

bool PCB_comparator(void* process, void* pid) {
    return ((PCB*) process)->id == (uintptr_t) pid;
}

// sets current to target
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
    if (process == NULL) {
        List_first(sendingQueue);
        process = List_search(sendingQueue, PCB_comparator, (void*) ptr);
    }
    if (process == NULL) {
        List_first(receivingQueue);
        process = List_search(receivingQueue, PCB_comparator, (void*) ptr);
    }
    for (int i = 0; i < 5; i ++) {
        if (process == NULL) {
            List_first(semaphores[i]->plist);
            process = List_search(semaphores[i]->plist, PCB_comparator, (void*) ptr);
        }
    }

    return process;
}

#endif