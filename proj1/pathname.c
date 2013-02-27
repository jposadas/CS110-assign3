
#include "pathname.h"
#include "directory.h"
#include "inode.h"
#include "diskimg.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define NAME_MAX_LENGTH 14
#define MAX_PATH

/*
 * Return the inumber associated with the specified pathname. This need only
 * handle absolute paths. Return a negative number if an error is encountered.
 */
int
pathname_lookup(struct unixfilesystem *fs, const char *pathname)
{
	if(strcmp(pathname, "/") == 0) return ROOT_INUMBER;	//check if it's root
	struct direntv6 entry;
	
	int dirinumber = ROOT_INUMBER;
	char *path = malloc(strlen(pathname) + 1);
	memcpy(path, pathname, strlen(pathname) + 1); //copy pathname into path because strtok cannot be used on constant strings

	char *tokens = strtok(path, "/"); //tokenizes the string path
	
	while(tokens != NULL) {
		
		if(directory_findname(fs, tokens, dirinumber, &entry) <  0) return -1; //return negative value on failure
		dirinumber = entry.d_inumber;
		tokens = strtok(NULL, "/"); //read next token
	}

	free(path);
	return dirinumber;
}
