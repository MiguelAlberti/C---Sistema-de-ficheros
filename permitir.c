#include "ficheros.h"

int main(int argc, char **argv){
 
	if(argc!=4){ printf("Error de sintaxis\nSintaxis: permitir <nombre_dispositivo> <ninodo> <permisos>\n"); return -1;}

	bmount(argv[1]);

	int ninodo = atoi(argv[2]);
	int permiso = atoi(argv[3]);
	mi_chmod_f(ninodo,permiso);
	
	bumount();

	return 0;   
}
