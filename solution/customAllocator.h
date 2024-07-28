#ifndef __CUSTOM_ALLOCATOR__
#define __CUSTOM_ALLOCATOR__

/*=============================================================================
* do no edit lines below!
=============================================================================*/
#include <stddef.h> //for size_t

void* customMalloc(size_t size);
void customFree(void* ptr);
void* customCalloc(size_t nmemb, size_t size);
void* customRealloc(void* ptr, size_t size);
/*=============================================================================
* do no edit lines above!
=============================================================================*/
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/*=============================================================================
* defines
=============================================================================*/
#define SBRK_FAIL (void*)(-1)
#define ALIGN_TO_8BIT(x) (((((x) - 1) >> 2) << 2) + 4)

/*=============================================================================
* Block
=============================================================================*/
typedef struct Block
{
    size_t size;
    struct Block* next;
    bool free;
} Block;
extern Block* blockList;

#endif // CUSTOM_ALLOCATOR
