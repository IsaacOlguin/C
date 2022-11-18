#include "OwnFuseOperations.h"

int getIdInode (int fd) {
    struct stat buf;
    int ret;
 
    ret = fstat(fd, &buf);
    if ( ret <0 ) {
         perror ("fstat");
         return -1;
    }
    return buf.st_ino;
}

int getStatInode(char *filename, struct stat *buf) {
    int ret;
    int fd, inode;

    if ( filename == NULL || strcmp(filename, "")==0 ) {
       fprintf(stderr, "usage: %s <file>\n", filename) ;
       return EXIT_FAILURE;
    }

    fd = open(filename, O_RDONLY);
    if ( fd < 0 ) {
      perror("open");
      return EXIT_FAILURE;
    }
 
    ret = fstat(fd, &(*buf));
    if ( ret <0 ) {
         perror ("fstat");
         return -1;
    }
    return 0;
}

char *getUsername() {
	register struct passwd *pw;
  	register uid_t uid;
  	uid = geteuid();
  	pw = getpwuid(uid);
	return pw->pw_name;
}

// will be executed when the system asks for attributes of a file that was stored in the mount point
int prakt_getattr( const char *path, struct stat *st ) {
	printf("prakt_getattr path<<%s>>\n", path);
    st->st_uid = getuid(); //Owner of the file
	st->st_gid = getgid(); //Owner group 
	st->st_atime = time( NULL ); //Last access time
	st->st_mtime = time( NULL ); //Last modification time

/*
    int inodeNumber;

    // get the inode number from inode table
    inodeNumber = get_inodeNumber((char *)path);
    printf("\tinodeNumber <<%i>>\n", inodeNumber);
    */
	
	if ( strcmp( path, "/" ) == 0 || isDirectory( path ) == 1) {
        //printf("\t\tDirectory <<%s>>\n", path);
		st->st_mode = S_IFDIR | 0755; // If it is a regular file, directory or other. It also specifies permission bits
		st->st_nlink = 2; //Specifies the number of hard links
	} else if ( isFile( path ) == 1 ){
		st->st_mode = S_IFREG | 0644;
		st->st_nlink = 1;
		st->st_size = 1024;
	} else {
        return -ENOENT;
	}
    
	return 0;
}

//  will be executed when the system asks for a list of files that were stored in the mount point
int prakt_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi ) {
	printf("prakt_readdir\n");

    //printf( "\t--> Getting The List of Files of <<%s>> [DirInd:%i] [FileInd:%i]\n", path, dirIndex, fileIndex );
	
	filler( buffer, ".", NULL, 0 ); // Current Directory
	filler( buffer, "..", NULL, 0 ); // Parent Directory
	
    for ( int curr_idx = 0; curr_idx <= dirIndex; curr_idx++ ) {
        //printf("==>Path [%s] ArrayDir[%s] Comp[%i]\n", path, arrayDir[curr_idx].path, strcmp( path, arrayDir[curr_idx].path ));
        if ( strcmp( path, arrayDir[curr_idx].path ) == 0 )
            filler( buffer, arrayDir[ curr_idx ].name, NULL, 0 );
        //filler( buffer, dirList[ curr_idx ], NULL, 0 );
    }

    for ( int curr_idx = 0; curr_idx <= fileIndex; curr_idx++ ) {
        //if ( strcmp( path, arrayFiles[curr_idx].path ) == 0 )
        //filler( buffer, fileList[ curr_idx ], NULL, 0 );
        if ( strcmp( path, arrayFiles[curr_idx].path ) == 0 )
            filler( buffer, arrayFiles[ curr_idx ].name, NULL, 0 );
    }

	return 0;
}


// will be executed when the system is trying to read one of the files that were stored in the mount point
int prakt_read( const char *path, char *buff, size_t size, off_t offset, struct fuse_file_info *fi ) {
	printf("prakt_read <<%s>>\n", path);

	int file_idx = getFileIndex( path );
    printf("Index gotten is [%i]\n", file_idx);
	
	if ( file_idx == -1 )
		return -1;
	
	char *content = fileContent[ file_idx ];
    printf("content [%s]\n", content);
	
	memcpy( buff, content + offset, size );
		
	return strlen( content ) - offset;
}


int prakt_write(const char *path, const char *buff, size_t size, off_t offset, struct fuse_file_info *fi) {
	printf("prakt_write");

    writeToFile( path, buff, offset);
	
	return size;
}

int prakt_release(const char *path, struct fuse_file_info *fi) {
	printf("Release\n");
	return 0;
}

int prakt_flush(const char *path, struct fuse_file_info *fi) { 
	printf("Flush\n");
	return 0;
}

int prakt_open(const char *path, struct fuse_file_info *fi) {
	printf("prakt_open\n");
	return 0;
}

int prakt_truncate(const char *path, off_t newsize) {
	printf("prakt_truncate\n");
	return 0;
}

int prakt_ftruncate(const char *path, off_t offset, struct fuse_file_info *fi) {
	printf("prakt_ftruncate\n");
	return 0;
}

int prakt_mkdir( const char *path, mode_t mode ) {
    printf("prakt_mkdir <<%s>>\n", path);
	//path++;
	addDirectory( path );
    //mkdir(path, mode);
	
	return 0;
}

int prakt_mknod( const char *path, mode_t mode, dev_t rdev ) {
    printf("prakt_mknod\n");
    //path++;
	addFile( path );
	
	return 0;
}