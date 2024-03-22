#include "list.h"
#include "PCB.h"

int counter = 0;

void Create(int priority, List* readyQueues[]) {
    PCB process;
    process.id = counter ++;
    process.priority = priority;
    process.state = READY;
    printf("Process (id: %d) created with priority = %d", process.id, process.priority);
}