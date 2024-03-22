#include "list.h"
#include "PCB.h"

void Create(int priority, List* readyQueues[]) {
    PCB process;
    process.priority = priority;
    printf("Process (id: %d) created with priority = %d", process.id, process.priority);
}