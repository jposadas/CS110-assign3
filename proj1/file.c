#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "file.h"
#include "inode.h"
#include "diskimg.h"

#define FILE_BLOCK_SIZE 512

/*
 * Fetch the specified file block from the specified inode.
 * Return the number of valid bytes in the block, -1 on error.
 */
int
file_getblock(struct unixfilesystem *fs, int inumber, int blockNum, void *buf)
{
	struct inode ino;
	int rv = inode_iget(fs, inumber, &ino);
	if(rv == 0) {
		int diskBlockNum = inode_indexlookup(fs, &ino, blockNum);
		printf("fdf: %d, inumber: %d, blockNum: %d, diskBlockNum: %d\n", fs->dfd, inumber, blockNum, diskBlockNum);
		int numBytesRead = diskimg_readsector(fs->dfd, diskBlockNum, buf);
		
		if(numBytesRead == -1) return -1;
		int inodeSize = inode_getsize(&ino);
		int lastBlock = inodeSize / FILE_BLOCK_SIZE;
		if(blockNum == lastBlock) {
			int lastBlockBytes = inodeSize % FILE_BLOCK_SIZE;
			if (lastBlockBytes > 0)
				return lastBlockBytes;
			else
				return FILE_BLOCK_SIZE;
		} else {
			return FILE_BLOCK_SIZE;
		}		
	} else {
		return -1;
	}
  //fprintf(stderr, "file_getblock(inumber = %d, blockNum = %d) unimplemented. returning -1\n", inumber, blockNum);
  //return -1;  
}
