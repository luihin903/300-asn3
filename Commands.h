#ifndef COMMANDS_H
#define COMMANDS_H

#include "list.h"
#include "PCB.h"

extern List* readyQueues[3];
extern char command;
extern PCB* init;
extern PCB* running;
extern int cpuClock;

int counter = 0;

void Create(int priority);
void Fork();
void Kill(int pid);
void Exit();
void Quantum();
void Send(int pid, char* msg);
void Receive();
void Reply(int pid, char* msg);
void NewSemaphore(int sid, int value);
void SemaphoreP(int sid);
void SemaphoreV(int sid);

void Create(int priority) {
    if (priority != 0 && priority != 1 && priority != 2) {
        printf("Invalid priority.\n");
        return;
    }

    PCB* process = (PCB*)malloc(sizeof(PCB));
    process->id = ++ counter;
    process->priority = priority;
    process->state = READY;
    if (List_append(readyQueues[process->priority], process)) {
        printf("Process create failed");
        counter --;
    }
    else {
        printf("Process (id: %d) created successfully with priority = %d.\n", process->id, process->priority);
    }

    if (running == init) {
        Quantum();
    }
}

void Fork() {
    if (running == init) {
        printf("Failed to fork process \"init\"\n");
    }
    else {
        Create(running->priority);
    }
}

void Kill(int pid) {

    if (pid == init->id) {
        if (running == init) {
            printf("Process \"init\" killed successfully. Simulation ended.\n");
            exit(0);
        }
        else {
            printf("Unable to kill process \"init\".\n");
            return;
        }
    }

    PCB* process = PCB_find(pid);

    if (process == NULL) {
        printf("Process pid = %d Does Not Exist.\n", pid);
    }
    else {
        if (process == running) {
            running = init;
            running->state = RUNNING;
        }
        else {
            process = List_remove(readyQueues[process->priority]);
        }
        printf("Process pid = %d killed successfully\n", process->id);
        Quantum();
    }
}

void Exit() {

}

void Quantum() {
    cpuClock ++;

    if (running->id != 0)
        List_append(readyQueues[running->priority], running);

    if (running->state == RUNNING)
        running->state = READY;

    switch (cpuClock % 6) {
        case 0:
            running = List_first(readyQueues[0]);
            if (running != NULL) {
                running = List_remove(readyQueues[0]);
                break;
            }
        case 1:
            running = List_first(readyQueues[1]);
            if (running != NULL) {
                running = List_remove(readyQueues[1]);
                break;
            }
        case 2:
            running = List_first(readyQueues[0]);
            if (running != NULL) {
                running = List_remove(readyQueues[0]);
                break;
            }
        case 3:
            running = List_first(readyQueues[1]);
            if (running != NULL) {
                running = List_remove(readyQueues[1]);
                break;
            }
        case 4:
            running = List_first(readyQueues[0]);
            if (running != NULL) {
                running = List_remove(readyQueues[0]);
                break;
            }
        case 5:
            running = List_first(readyQueues[2]);
            if (running != NULL) {
                running = List_remove(readyQueues[2]);
                break;
            }
        // fall from case 5
        case 6:
            running = List_first(readyQueues[0]);
            if (running != NULL) {
                running = List_remove(readyQueues[0]);
                break;
            }
        case 7:
            running = List_first(readyQueues[1]);
            if (running != NULL) {
                running = List_remove(readyQueues[1]);
                break;
            }
    }

    if (running == NULL) {
        running = init;
    }

    running->state = RUNNING;
    printf("Process pid = %d is now running.\n", running->id);
}

void Procinfo(int pid) {
    PCB* process = PCB_find(pid);

    if (process == NULL) {
        printf("Process pid = %d Does Not Exist.\n", pid);
    }
    else {
        printf("Priority: %d\n", process->priority);
        printf("State: %s\n", State_toString(process->state));
    }    
}

void Totalinfo() {
    printf("Process \"init\": { pid : %d }\n", init->id);

    for (int i = 0; i < 3; i ++) {
        List_first(readyQueues[i]);
        List_prev(readyQueues[i]);
        printf("Ready Queue (Priority = %d): [", i);

        for (int j = 0; j < List_count(readyQueues[i]); j ++) {
            PCB* process = List_next(readyQueues[i]);
            printf("{ pid : %d }, ", process->id);
        }

        if (List_count(readyQueues[i]) != 0) printf("\b\b");
        printf("] (%d)\n", List_count(readyQueues[i]));
    }
}

#endif