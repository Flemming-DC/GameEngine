#pragma once
#include <iostream>


namespace Memory
{
    bool trackMemory = false;
    bool isShuttingDown = false;
    int allocated = 0;
    int freed = 0;
    int lastAllocated = 0;
    int lastFreed = 0;

    void Update()
    {
        if (!trackMemory)
            return;
        int netAllocation = allocated - freed;
        int lastNetAllocation = lastAllocated - lastFreed;
        if (netAllocation - lastNetAllocation == 0)
            return;
        std::cout << "NetAllocation On Frame: " << (netAllocation - lastNetAllocation) / 1000.0f << " kB    "
            "Allocation On Frame: " << (allocated - lastAllocated) / 1000.0f << " kB" << std::endl;
        
        lastAllocated = allocated;
        lastFreed = freed;
    }
    
    void ShutDown() 
    {
        if (!trackMemory)
            return;
        std::cout << "Allocation On ShutDown: " << (allocated - freed) / 1000.0f << " kB" << std::endl;
        isShuttingDown = true;
    }

}

/*
Memory::trackMemory = true;

void* operator new(size_t size)
{
    Memory::allocated += size;
    return malloc(size);
}
void operator delete(void* ptr, size_t size)
{
    Memory::freed += size;

    if (Memory::isShuttingDown)
        std::cout << "Allocation On ShutDown: " << (Memory::allocated - Memory::freed) / 1000.0f << " kB" << std::endl;

    return free(ptr);
}
*/



