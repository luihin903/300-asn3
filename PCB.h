typedef struct Process_Control_Block PCB;
struct Process_Control_Block {
    int id;
    int priority;
    enum State state;
};

enum State {
    RUNNING,
    READY,
    BLOCKED
};