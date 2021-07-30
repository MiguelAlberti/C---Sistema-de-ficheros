#include "bloques.h"

static int descriptor=0;

int bmount(const char *camino){

	descriptor=open(camino, O_RDWR|O_CREAT, 0666);
	if(descriptor==-1){
		return -1;
	}
	return descriptor;
}

int bumount(){

	if( close(descriptor)==-1){
		return -1;
	}
	return 0;
}

int bwrite(unsigned int nbloque, const void *buf){

	lseek(descriptor, nbloque*BLOCKSIZE,SEEK_SET);
	int w=write(descriptor,buf,BLOCKSIZE);
	if(w==-1){
		fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
		return -1;
	}
	return w;
}

int bread(unsigned int nbloque, void *buf){

	lseek(descriptor, nbloque*BLOCKSIZE,SEEK_SET);
	int r=read(descriptor,buf,BLOCKSIZE);
	if(r==-1){
		fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
		return -1;
	}
	return r;
}

