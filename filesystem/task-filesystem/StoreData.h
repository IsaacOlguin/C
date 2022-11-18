
#ifndef STORE_DATA_H
#define STORE_DATA_H

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define TAMANIO 256

typedef struct str_Prakt_File {
    char path[TAMANIO];
    char name[TAMANIO];
    char fullpath[TAMANIO];
} praktFile;

extern praktFile arrayFiles[TAMANIO];
extern praktFile arrayDir[TAMANIO];

extern int dirIndex;
extern int fileIndex;
extern int fileContentIndex;

extern char dirList[TAMANIO][TAMANIO];
extern char fileList[TAMANIO][TAMANIO];
extern char fileContent[TAMANIO][TAMANIO];

void addDirectory( const char *dirName ); 
int isDirectory( const char *path );
void addFile( const char *filename );
int isFile( const char *path );
int getFileIndex( const char *path );
void writeToFile( const char *path, const char *newContent, off_t offset );


#endif // STORE_DATA_H

