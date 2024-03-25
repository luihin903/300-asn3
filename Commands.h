#ifndef COMMANDS_H
#define COMMANDS_H

#include <string.h>
#include "list.h"
#include "PCB.h"

extern List* readyQueues[3];
extern List* sendingQueue;
extern List* receivingQueue;
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
        printf("error: Invalid priority.\n");
        return;
    }

    PCB* process = (PCB*)malloc(sizeof(PCB));
    process->id = ++ counter;
    process->priority = priority;
    process->state = READY;
    if (List_append(readyQueues[process->priority], process)) {
        printf("error: Process create failed");
        counter --;
    }
    else {
        printf("{ pid : %d } created successfully with priority = %d.\n", process->id, process->priority);
    }

    if (running == init) {
        Quantum();
    }
}

void Fork() {
    if (running == init) {
        printf("error: Failed to fork process \"init\"\n");
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
            printf("error: Unable to kill process \"init\".\n");
            return;
        }
    }

    PCB* process = PCB_find(pid);

    if (process == NULL) {
        printf("error: { pid : %d } Does Not Exist.\n", pid);
    }
    else {
        if (process == running) {
            running = init;
            running->state = RUNNING;
        }
        else if (process->state == READY) {
            process = List_remove(readyQueues[process->priority]);
        }
        else if (process->state == BLOCKED) {
            if (process->source == SENDING) {
                process = List_remove(sendingQueue);
            }
            else if (process->source == RECEIVING) {
                process = List_remove(receivingQueue);
            }
        }

        printf("{ pid : %d } killed successfully\n", process->id);
        Quantum();
    }
}

void Exit() {
    Kill(running->id);
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
    printf("{ pid : %d } is now running.\n", running->id);
}

void Send(int pid, char* msg) {
    if (strlen(msg) > 40) {
        printf("error: Message oversized.\n");
        return;
    }

    PCB* target = PCB_find(pid);
    
    if (target == NULL) {
        printf("error: { pid : %d } Does Not Exist", pid);
    }
    else if (target->msg != NULL) {
        printf("error: Unable to send second message to { pid : %d }.\n", target->id);
    }
    else {
        Message* message = (Message*)malloc(sizeof(Message));
        message->content = msg;
        message->sender = running;
        target->msg = message;

        if (message->sender != init) {
            message->sender->state = BLOCKED;
            List_append(sendingQueue, message->sender);
            printf("{ pid : %d } sent the message \"%s\" to { pid : %d }.\n", message->sender->id, message->content, target->id);
            running = init;
            Quantum();
        }

        if (target->state == BLOCKED) {
            if (target->source == RECEIVING) {
                target->source = NONE;
                target->state = READY;
                List_remove(receivingQueue);
                List_append(readyQueues[target->priority], target);
            }
        }

    }
}

void Receive() {
    
    if (running->msg != NULL) {
        printf("{ pid : %d } received a message from { pid : %d }: \"%s\".\n", running->id, running->msg->sender->id, running->msg->content);
        free(running->msg);
        running->msg = NULL;
    }
    else {
        running->state = BLOCKED;
        running->source = RECEIVING;
        List_append(receivingQueue, running);
        printf("{ pid : %d } started to receive message.\n", running->id);
        running = init;
        running->state = RUNNING;
        Quantum();
    }
}

void Reply(int pid, char* msg) {
    if (strlen(msg) > 40) {
        printf("error: Message oversized.\n");
        return;
    }

    PCB* target = PCB_find(pid);
    
    if (target == NULL) {
        printf("error: { pid : %d } Does Not Exist", pid);
    }
    else if (target->state != BLOCKED || target->source != SENDING) {
        printf("error: { pid : %d } is not waiting for reply.\n", target->id);
    }
    else {
        List_remove(sendingQueue);
        List_append(readyQueues[target->priority], target);
        printf("{ pid : %d } received reply from { pid : %d }: \"%s\".\n", target->id, running->id, msg);
    }
}

void Procinfo(int pid) {
    PCB* process = PCB_find(pid);

    if (process == NULL) {
        printf("{ pid : %d } Does Not Exist.\n", pid);
    }
    else {
        printf("Priority: %d\n", process->priority);
        printf("State: %s\n", State_toString(process->state));
        if (process->msg != NULL) {
            printf("Holding message: \"%s\"\n", process->msg->content);
        }
    }    
}

void Totalinfo() {
            
    void displayQueue(List* queue, char* name) {
        List_first(queue);
        List_prev(queue);
        printf("%s: [", name);
        for (int i = 0; i < List_count(queue); i ++) {
            PCB* process = List_next(queue);
            printf("{ pid : %d }, ", process->id);
        }
        if (List_count(queue) != 0) printf("\b\b");
        printf("] (%d)\n", List_count(queue));
    }

    printf("Process \"init\": { pid : %d }\n", init->id);
    printf("Running process: { pid : %d }\n", running->id);
    displayQueue(readyQueues[0], "Ready Queue (Priority = 0)");
    displayQueue(readyQueues[1], "Ready Queue (Priority = 1)");
    displayQueue(readyQueues[2], "Ready Queue (Priority = 2)");
    displayQueue(sendingQueue, "Sending Queue");
    displayQueue(receivingQueue, "Receiving Queue");
    
}

#endif