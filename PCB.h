int counter = 0;

typedef struct Process_Control_Block PCB;
struct Process_Control_Block {
    int id = counter ++;
    int priority;
    State state = READY;
};

enum State {
    RUNNING,
    READY,
    BLOCKED
};