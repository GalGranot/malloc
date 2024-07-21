/*=============================================================================
* includes
=============================================================================*/
#include "wet3.h"

#define dbg 1

/*=============================================================================
* memory management
=============================================================================*/
void initBlock(Block* block, size_t size)
{
    block->free = false;
    block->next = NULL;
    block->size = size;
}

Block* requestSpace(size_t size)
{
    Block* block;
    void* curr_program_break = sbrk(0);
    if(sbrk(size + sizeof(Block)) == SBRK_FAIL)
        return NULL;
    if(dbg) printf("<requestSpace>: allocated %d bytes\n", size);
    initBlock(block, size);
    return block;
}

void mergeBlocks()
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
void* __malloc(size_t size)
{
    if(dbg) printf("<malloc>: trying to malloc %d bytes\n", size);
    if(size <= 0)
    {
        perror("<malloc error>: passed nonpositive size\n");
        return NULL;
    }
    size_t alignedSize = ALIGN_TO_8BIT(size);
    Block* block = findFreeBlock();
    if(block)
    {
        if(dbg) printf("<malloc>: allocated free block\n");
        block->Free = false;
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
        while(currentBlock)
            currentBlock = curr_block->next;
        currentBlock->next = block;
    }
    return (block + 1);
}

void __free(void* ptr)
{
    if(!ptr)
        perror("<free error>: passed null pointer\n");
    Block* block = (Block*)ptr - 1;
    block->free = true;
    mergeBlocks();
}

void* __calloc(size_t nmemb, size_t size)
{
    //TODO
}

void* __realloc(void* ptr, size_t size)
{
    //TODO
}