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
https://prod.liveshare.vsengsaas.visualstudio.com/join?C27AEFA88E70449051D9329C7D63291CCD22