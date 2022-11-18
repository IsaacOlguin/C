
#ifndef OWN_FUSE_OPS_H
#define OWN_FUSE_OPS_H

#define FUSE_USE_VERSION 30
#include <stdio.h>	
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h> 
#include <fuse.h> 
#include "StoreData.h"	

int getIdInode (int fd);
int getStatInode(char *filename, struct stat *buf);
char *getUsername();
int prakt_getattr( const char *path, struct stat *st );
int prakt_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi );
int prakt_read( const char *path, char *buff, size_t size, off_t offset, struct fuse_file_info *fi );
int prakt_write(const char *path, const char *buff, size_t size, off_t offset, struct fuse_file_info *fi);
int prakt_release(const char *path, struct fuse_file_info *fi);
int prakt_flush(const char *path, struct fuse_file_info *fi);
int prakt_open(const char *path, struct fuse_file_info *fi);
int prakt_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi);
int prakt_truncate(const char *path, off_t newsize);
int prakt_mkdir( const char *path, mode_t mode );
int prakt_mknod( const char *path, mode_t mode, dev_t rdev );


#endif //OWN_FUSE_OPS_H