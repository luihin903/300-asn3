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

#endif