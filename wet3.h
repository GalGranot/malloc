#ifndef __WET3__
#define __WET3__

/*=============================================================================
* do no edit lines below!
=============================================================================*/
#include <stddef.h> //for size_t

void* __malloc(size_t size);
void __free(void* ptr);
void* __calloc(size_t nmemb, size_t size);
void* __realloc(void* ptr, size_t size);
/*=============================================================================
* do no edit lines above!
=============================================================================*/
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

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

Block* requestSpace(size_t size);
Block* findFreeBlock(size_t size);
void mergeBlocks(void);


#endif // __WET3__
