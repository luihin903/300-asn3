#ifndef PCB_H
#define PCB_H

enum State {
    RUNNING,
    READY,
    BLOCKED
};

typedef struct Process_Control_Block PCB;
struct Process_Control_Block {
    int id;
    int priority;
    enum State state;
};

void PCB_run(PCB process) {
    
}

bool PCB_comparator(void* process, void* pid) {
    return ((PCB*) process)->id == (uintptr_t) pid;
}

#endif