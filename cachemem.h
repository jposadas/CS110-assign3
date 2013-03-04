#ifndef _CACHEMEM_H
#define _CACHEMEM_H



/*
 * The main export of the cachemem module is the memory for the cache
 * pointed to by the following global variables:
 * cacheMemSizeInKB - The size of the cache memory in kiloytes. 
 * cacheMemPtr      - Starting address of the cache memory. 
 */ 
extern int cacheMemSizeInKB;
extern void *cacheMemPtr;

#define CACHEMEM_MAX_SIZE (64*1024*1024)

#define FILE_BLOCK_SIZE 512 

struct cacheBlock{
  int diskBlockNumber;
  unsigned char buf[FILE_BLOCK_SIZE];
};


int CacheMem_Init(int sizeInKB);

void putBlockInCache(int diskBlockNumber, void *buf, int bytesRead); // puts block in cache

int getBlockFromCache(void *buf, int index); // gets the specified stored block in the cache

int totalCacheSize(); // returns the total size in the cache

int isBlockInCache(int diskBlockNumber); // returns 1 if block is in cache, 0 otherwise

#endif /* _CACHEMEM_H */
