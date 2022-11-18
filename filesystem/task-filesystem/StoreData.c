#include "StoreData.h"

void getNameAndPath(const char *fullpath, char *filename, char *dirPath) {
	int length = strlen(fullpath);
	strcpy(filename, "");
	strcpy(dirPath, "");

	//printf("getNameAndPath <<%s>> <<%s>> <<%s>>\n", fullpath, filename, dirPath);

	int indexDiv = -1;
	for(int i = length-1; i >= 0; i--){
		if(fullpath[i] == '/') {
			indexDiv = i; 
			break;
		}
	}

	//printf("Indice en donde encontre ultimo / <<%i>>\n", indexDiv);

	for(int i = 0; i < indexDiv; i++) {
		//dirPath[i] = fullpath[i];
		sprintf(dirPath, "%s%c", dirPath, fullpath[i]);
	}
	if(!strlen(dirPath)) strcpy(dirPath, "/");
	//printf("Longitud de dirPath <<%li>> <<%s>>\n", strlen(dirPath), dirPath);


	for(int i = indexDiv+1, j = 0; i < length; i++, j++) {
		//filename[j] = fullpath[i];
		sprintf(filename, "%s%c", filename, fullpath[i]);
	}
}

void addDirectory( const char *dirName ) {
	char filename[TAMANIO];
	char dirPath[TAMANIO];

	getNameAndPath(dirName, filename, dirPath);
	//printf("DirName[%s] -- FileName[%s] -- DirPath[%s] --\n", dirName, filename, dirPath);

	dirIndex++;
	/*
	dirName++;
	printf("NewDirName <<%s>>\n", dirName);
	strcpy( dirList[ dirIndex ], dirName );
	*/
	strcpy(arrayDir[dirIndex].fullpath, dirName);
	strcpy(arrayDir[dirIndex].path, dirPath);
	strcpy(arrayDir[dirIndex].name, filename);
}

int isDirectory( const char *path) {
	//path++; // Eliminating "/" in the path
	
	printf("path [%s] DirIndex[%i]\n", path, dirIndex);
	for ( int curr_idx = 0; curr_idx <= dirIndex; curr_idx++ ) {
		printf("Contenido[%i]=>[%s]\n", curr_idx, dirList[curr_idx]);
		/*
		if ( strcmp( path, dirList[ curr_idx ] ) == 0 )
			return 1;
		*/
		if ( strcmp( path, arrayDir[ curr_idx ].fullpath ) == 0 )
			return 1;
    }
	
	return 0;
}

void addFile( const char *filename) {
	/*
	fileIndex++;
	strcpy( fileList[ fileIndex ], filename );
	
	fileContentIndex++;
	strcpy( fileContent[ fileContentIndex ], "" );
	*/
	char file[TAMANIO];
	char dirPath[TAMANIO];

	getNameAndPath(filename, file, dirPath);
	//printf("DirName[%s] -- FileName[%s] -- DirPath[%s] --\n", filename, file, dirPath);

	fileIndex++;

	strcpy(arrayFiles[fileIndex].fullpath, filename);
	strcpy(arrayFiles[fileIndex].path, dirPath);
	strcpy(arrayFiles[fileIndex].name, file);
}

int isFile( const char *path) {
	/*path++; // Eliminating "/" in the path
	
	for ( int curr_idx = 0; curr_idx <= fileIndex; curr_idx++ ) {
		if ( strcmp( path, fileList[ curr_idx ] ) == 0 )
			return 1;
	}
	*/
	//printf("path [%s] FileIndex[%i]\n", path, fileIndex);
	for ( int curr_idx = 0; curr_idx <= fileIndex; curr_idx++ ) {
		//printf("Contenido[%i]=>[%s]\n", curr_idx, fileList[curr_idx]);
		/*
		if ( strcmp( path, dirList[ curr_idx ] ) == 0 )
			return 1;
		*/
		if ( strcmp( path, arrayFiles[ curr_idx ].fullpath ) == 0 )
			return 1;
    }
	return 0;
}

int getFileIndex( const char *path) {
	//path++; // Eliminating "/" in the path
	
	for ( int curr_idx = 0; curr_idx <= fileIndex; curr_idx++ ) {
		printf("Path[%s] -- ArrayFile[%i][%s]\n", path, curr_idx, arrayFiles[ curr_idx ].path);
		if ( strcmp( path, arrayFiles[ curr_idx ].fullpath ) == 0 )
			return curr_idx;
    }
	
	return -1;
}

void writeToFile( const char *path, const char *newContent, off_t offset ) {
	int fileInd = getFileIndex( path );
	
	if ( fileInd == -1 ) // No such file
		return;
		
	if (!offset)
		strcpy( fileContent[ fileInd ], newContent ); 
	else
		sprintf( fileContent[ fileInd ], "%s%s", fileContent[ fileInd ], newContent);
}

