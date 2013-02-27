
#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include "file.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
 

/*
 * Lookup the specified name (name) in the directory (dirinumber). If found, return the 
 * directory entry in dirEnt. Return 0 on success and something negative on failure. 
 */
int
directory_findname(struct unixfilesystem *fs, const char *name,
                   int dirinumber, struct direntv6 *dirEnt)
{
  
	struct inode ino;
	if(inode_iget(fs, dirinumber, &ino) == -1) return -1;
	if((ino.i_mode & IFMT) != IFDIR) return -1;

	//traverse the inodes datablocks forloop
	//compare the given file with directory entries
  	int size = inode_getsize(&ino);
	int numBlocks = (size + DISKIMG_SECTOR_SIZE - 1) / DISKIMG_SECTOR_SIZE;

	for(int block = 0; block < numBlocks; block++) {
		struct direntv6 buffer[DISKIMG_SECTOR_SIZE / sizeof(struct direntv6)]; //an array of direntv6s 
		int validBytes = file_getblock(fs, dirinumber, block, &buffer);
		if(validBytes < 0) return -1;
		
		int dirsPerBlock = validBytes / sizeof(struct direntv6); //calculate the number of dirsPerBlock based on validBytes of block

		for(int dir = 0; dir < dirsPerBlock; dir++) {
			if(strcmp(name, buffer[dir].d_name) == 0) { //copy memory into the direntv6 only if name is found in block
				memcpy(dirEnt, &buffer[dir], sizeof(struct direntv6));
				return 0;
			}
		}
		
	}

  	return -1;

}
