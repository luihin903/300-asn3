#ifndef THREADS_H
#define THREADS_H

extern List* readyQueues[3];
extern char command;

void* inputFunction(void* t) {
    
    while (true) {

        printf("Simulation$ ");
        scanf(" %c", &command);

        switch (command) {
            case 'C':
                int priority = -1;
                while (priority != 0 && priority != 1 && priority != 2) {
                    printf("Priority(0, 1, 2): ");
                    scanf("%d", &priority);
                }
                Create(priority);
                break;
            case 'I':
                int pid;
                printf("pid: ");
                scanf("%d", &pid);
                Procinfo(pid);
                break;
            case 'T':
                Totalinfo();
                break;
            default:
                printf("%s \"%c\"\n", "Invalid Command", command);
        }
    }

}

void* osFunction(void* t) {
    PCB* running = init;
    running->state = RUNNING;

    while (true) {

    }
}

#endif