#include "customAllocator.h"

int main()
{
    for(int i = 1; i < 1024; i++)
    {
        void* ptr = customMalloc(i);
        customFree(ptr);
        ptr = customCalloc(2, i);;
        customRealloc(ptr, i * 4);
        customFree(ptr);
    }
}