/*=============================================================================
* includes
=============================================================================*/
#include "customAllocator.h"
Block* blockList;

#define dbg 1

/*=============================================================================
* declarations
=============================================================================*/
Block* requestSpace(size_t size);
Block* findFreeBlock(size_t size);
void mergeBlocks(void);

/*=============================================================================
* memory management
=============================================================================*/
void initBlock(Block* block, size_t size)
{
    block->free = false;
    block->next = NULL;
    block->size = size;
}
#include "stdlib.h"
Block* requestSpace(size_t size)
{
    Block* block;
    void* programBreak = sbrk(0);
    if(sbrk(size + sizeof(Block)) == SBRK_FAIL) 
        return NULL;
    if(dbg) printf("<requestSpace>: allocated %d bytes\n", size);
    block = (Block*)programBreak;
    initBlock(block, size);
    return block;
}

bool areBuddies(Block* b1, Block* b2)
{
    return b1->size == b2->size && (((char*)b1 - (char*)b2) % (2 * b1->size) == 0);
}

void mergeBlocks()
{
    Block* currentBlock = blockList;
    Block* previousBlock = NULL;
    while (currentBlock)
    {
        if (currentBlock->free)
        {
            if (dbg) printf("<mergeBlocks>: found free block, trying to merge\n");
            Block* nextBlock = currentBlock->next;
            while (nextBlock && nextBlock->free && areBuddies(currentBlock, nextBlock))
            {
                if (dbg) printf("<mergeBlocks>: successfully merged with buddy\n");
                currentBlock->size *= 2;
                currentBlock->next = nextBlock->next;
                nextBlock = currentBlock->next;
            }
        }
        previousBlock = currentBlock;
        currentBlock = currentBlock->next;
    }
}

void mergeBlocksWithoutBuddySystem()
{
    Block* currentBlock = blockList;
    Block* previousBlock = NULL;
    while(currentBlock)
    {
        if(currentBlock->free) //try and merge block with neigbours
        {
            if(dbg) printf("<mergeBlocks>: found free block, trying to merge\n");
            if(previousBlock && previousBlock->free) //can merge with previous block
            {
                if(dbg) printf("<mergeBlocks>: successfully merged\n");
                previousBlock->size += sizeof(Block) + currentBlock->size;
                previousBlock->next = currentBlock->next;
            }
        }
        previousBlock = currentBlock;
        currentBlock = currentBlock->next;
    }
}

Block* findFreeBlock(size_t size)
{
    Block* currentBlock = blockList;
    while(currentBlock)
    {
        if(currentBlock->free && currentBlock->size >= size)
            return currentBlock;
        currentBlock = currentBlock->next;
    }
    return NULL;
}

/*=============================================================================
* required functions
=============================================================================*/
void* customMalloc(size_t size)
{
    if(dbg) printf("<malloc>: trying to malloc %d bytes\n", size);
    if(size <= 0)
    {
        perror("<malloc error>: passed nonpositive size\n");
        return NULL;
    }
    size_t alignedSize = ALIGN_TO_8BIT(size);
    Block* block = findFreeBlock(alignedSize);
    if(block)
    {
        if(dbg) printf("<malloc>: allocated free block\n");
        block->free = false;
        return (block + 1);
    }
    if(dbg) printf("<malloc>: couldn't find free block, requesting space\n");
    block = requestSpace(alignedSize);
    if(!block)
        return NULL;
    
    if(blockList == NULL)
        blockList = block;
    else  //traverse list to end
    {
        Block* currentBlock = blockList;
        while(currentBlock && currentBlock->next)
        {
            currentBlock = currentBlock->next;
        }
        currentBlock->next = block;
    }
    return (block + 1);
}

void customFree(void* ptr)
{
    if(!ptr)
        perror("<free error>: passed null pointer\n");
    Block* block = (Block*)ptr - 1;
    block->free = true;
    if(dbg) printf("<free>: freed block\n");
    mergeBlocks();
}

void* customCalloc(size_t nmemb, size_t size)
{
    size_t totalSize = nmemb * size;
    void* ptr = customMalloc(totalSize);
    if(ptr)
        memset(ptr, 0, totalSize);
    return ptr;
}

void* customRealloc(void* ptr, size_t size)
{
    if (!ptr)
        return customMalloc(size);
    Block* block = (Block*)((char*)ptr - sizeof(Block));
    size_t oldSize = block->size - sizeof(Block);
    if (size <= oldSize)
        return ptr;
    void* newPtr = customMalloc(size);
    if (newPtr)
    {
        memcpy(newPtr, ptr, oldSize);
        customFree(ptr);
    }
    return newPtr;
}
