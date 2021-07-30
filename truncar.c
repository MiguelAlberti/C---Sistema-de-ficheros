#include "ficheros.h"

int main(int argc, char **argv){
 
	if(argc!=4){ printf("Error de sintaxis\nSintaxis: truncar <nombre_dispositivo> <ninodo> <nbytes>\n"); return -1;}

	bmount(argv[1]);

	int ninodo = atoi(argv[2]);
	int nbytes = atoi(argv[3]);
	
	mi_truncar_f(ninodo,nbytes);
	
	bumount();

	return 0;   
}
