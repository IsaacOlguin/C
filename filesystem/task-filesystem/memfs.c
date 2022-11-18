
#define FUSE_USE_VERSION 30

#include <fuse.h> 		
#include <stdio.h>
//#include <fuse/fuse_lowlevel.h>
#include "OwnFuseOperations.h"
#include "StoreData.h"

int dirIndex = -1;
int fileIndex = -1;
int fileContentIndex = -1;

char dirList[TAMANIO][TAMANIO];
char fileList[TAMANIO][TAMANIO];
char fileContent[TAMANIO][TAMANIO];

praktFile arrayFiles[TAMANIO];
praktFile arrayDir[TAMANIO];

static struct fuse_operations operations = {
    .getattr	= prakt_getattr,
    .readdir	= prakt_readdir, //int (*readdir) (const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info_compat *);
    .read		= prakt_read,
	.write		= prakt_write,
	.open		= prakt_open,
	.release	= prakt_release,  
	.flush		= prakt_flush,  
	.truncate 	= prakt_truncate,
	.mkdir		= prakt_mkdir,
	.mknod		= prakt_mknod,
};

// gcc FUSE.c -o myFUSE `pkg-config fuse --cflags --libs`
int main( int argc, char *argv[] ) {
    int ret = fuse_main( argc, argv, &operations, NULL);
    printf("Ret <<%i>>\n", ret);
	return ret;
}