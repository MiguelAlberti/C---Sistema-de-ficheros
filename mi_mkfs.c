#include "ficheros_basico.h"

int main(int argc, char **argv){
 
	int numBloques = atoi(argv[2]);
	int inodos = numBloques/4;
	unsigned char buffer[BLOCKSIZE];
	memset(buffer,0,BLOCKSIZE);

	bmount(argv[1]);

	for(int i=0;i< numBloques;i++){
		bwrite(i,buffer);
	}
	
	initSB(numBloques,inodos);
	initMB();
	initAI();
	
	reservar_inodo('d',7);
	bumount();

	return 0;   
}
