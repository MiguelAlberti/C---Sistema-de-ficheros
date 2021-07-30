#include "ficheros.h"

int main(int argc, char *argv[]){

			

if(argc < 3){ printf("Sintaxis incorrecta\nSintaxis: escribir <nombre_dispositivo> <'texto o fichero'> <diferentes_inodos>\n"); return -1;}

if(bmount(argv[1])< 0 ){
    perror("Error bmount\n");
    return -1;
}

int longitud= strlen(argv[2]);
int diferentes_inodos = atoi(argv[3]);
printf("longitud %i\n",longitud);


struct STAT stat;
char buffer[BLOCKSIZE];
strcpy(buffer, "blablablablalabla......\n");
char BytesLong[100];
char BloquesOcuapados[100];

struct tm *ts;
	char atime[80];
	char mtime[80];
	char ctime[80];
struct inodo in;


int ninodo = reservar_inodo('f',6);
ninodo=ninodo-1;
printf("Nº inodo %i\n",ninodo);
mi_write_f(ninodo,buffer,0,longitud*sizeof(char));
mi_stat_f(ninodo,&stat);
sprintf(BytesLong,"Tamaño bytes lógicos: %d\n", stat.tamEnBytesLog);
sprintf( BloquesOcuapados,"Tamaño ocupa inodo: %d\n", stat.numBloquesOcupados);
fflush(stdout);
write(2,BytesLong,strlen(BytesLong));
write(2,BloquesOcuapados,strlen(BloquesOcuapados));
leer_inodo(ninodo,&in);
	printf("----> Numero inodo: %d\n",ninodo);
			printf("\tTipo de inodo: %c\n",in.tipo);
			printf("\tPermisos: %d\n",in.permisos);
			printf("\tNumero de links: %d\n",in.nlinks);
			printf("\tTamanño en bytes logicos: %d\n",in.tamEnBytesLog);
			printf("\tNumero de bloques que ocupa: %d\n",in.numBloquesOcupados);
			ts = localtime(&in.atime);
			strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.mtime);
			strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.ctime);
			strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
			printf("\tATIME: %s\n\tMTIME: %s\n\tCTIME: %s\n",atime,mtime,ctime);

//offset == 5120
if(diferentes_inodos==1){
	ninodo = reservar_inodo('f',6);
	ninodo=ninodo-1;
}
printf("Nº inodo %i\n",ninodo);
mi_write_f(ninodo,buffer,5120,longitud*sizeof(char));
memset(buffer,0,sizeof(buffer));
sprintf(BytesLong,"Tamaño bytes lógicos: %d\n", stat.tamEnBytesLog);
sprintf( BloquesOcuapados,"Tamaño ocupa inodo: %d\n", stat.numBloquesOcupados);
fflush(stdout);
write(2,BytesLong,strlen(BytesLong));
write(2,BloquesOcuapados,strlen(BloquesOcuapados));
leer_inodo(ninodo,&in);
	printf("----> Numero inodo: %d\n",ninodo);
			printf("\tTipo de inodo: %c\n",in.tipo);
			printf("\tPermisos: %d\n",in.permisos);
			printf("\tNumero de links: %d\n",in.nlinks);
			printf("\tTamaño en bytes logicos: %d\n",in.tamEnBytesLog);
			printf("\tNumero de bloques que ocupa: %d\n",in.numBloquesOcupados);
			ts = localtime(&in.atime);
			strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.mtime);
			strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.ctime);
			strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
			printf("\tATIME: %s\n\tMTIME: %s\n\tCTIME: %s\n",atime,mtime,ctime);

//offset == 256000
if(diferentes_inodos==1){
	ninodo = reservar_inodo('f',6);
	ninodo=ninodo-1;
}
printf("Nº inodo %i\n",ninodo);
mi_write_f(ninodo,buffer,256000,longitud*sizeof(char));
sprintf(BytesLong,"Tamaño bytes lógicos: %d\n", stat.tamEnBytesLog);
sprintf( BloquesOcuapados,"Tamaño ocupa inodo: %d\n", stat.numBloquesOcupados);
fflush(stdout);
write(2,BytesLong,strlen(BytesLong));
write(2,BloquesOcuapados,strlen(BloquesOcuapados));
leer_inodo(ninodo,&in);
	printf("----> Numero inodo: %d\n",ninodo);
			printf("\tTipo de inodo: %c\n",in.tipo);
			printf("\tPermisos: %d\n",in.permisos);
			printf("\tNumero de links: %d\n",in.nlinks);
			printf("\tTamaño en bytes logicos: %d\n",in.tamEnBytesLog);
			printf("\tNumero de bloques que ocupa: %d\n",in.numBloquesOcupados);
			ts = localtime(&in.atime);
			strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.mtime);
			strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.ctime);
			strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
			printf("\tATIME: %s\n\tMTIME: %s\n\tCTIME: %s\n",atime,mtime,ctime);
//offset == 30720000
if(diferentes_inodos==1){
	ninodo = reservar_inodo('f',6);
	ninodo=ninodo-1;
}
printf("Nº inodo %i\n",ninodo);
mi_write_f(ninodo,buffer,30720000,longitud*sizeof(char));
sprintf(BytesLong,"Tamaño bytes lógicos: %d\n", stat.tamEnBytesLog);
sprintf( BloquesOcuapados,"Tamaño ocupa inodo: %d\n", stat.numBloquesOcupados);
fflush(stdout);
write(2,BytesLong,strlen(BytesLong));
write(2,BloquesOcuapados,strlen(BloquesOcuapados));
leer_inodo(ninodo,&in);
	printf("----> Numero inodo: %d\n",ninodo);
			printf("\tTipo de inodo: %c\n",in.tipo);
			printf("\tPermisos: %d\n",in.permisos);
			printf("\tNumero de links: %d\n",in.nlinks);
			printf("\tTamaño en bytes logicos: %d\n",in.tamEnBytesLog);
			printf("\tNumero de bloques que ocupa: %d\n",in.numBloquesOcupados);
			ts = localtime(&in.atime);
			strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.mtime);
			strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.ctime);
			strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
			printf("\tATIME: %s\n\tMTIME: %s\n\tCTIME: %s\n",atime,mtime,ctime);

//offset == 71680000
if(diferentes_inodos==1){
	ninodo = reservar_inodo('f',6);
	ninodo=ninodo-1;
}
printf("Nº inodo %i\n",ninodo);
mi_write_f(ninodo,buffer,71680000,longitud*sizeof(char));
sprintf(BytesLong,"Tamaño bytes lógicos: %d\n", stat.tamEnBytesLog);
sprintf( BloquesOcuapados,"Tamaño ocupa inodo: %d\n", stat.numBloquesOcupados);
fflush(stdout);
write(2,BytesLong,strlen(BytesLong));
write(2,BloquesOcuapados,strlen(BloquesOcuapados));
leer_inodo(ninodo,&in);
	printf("----> Numero inodo: %d\n",ninodo);
			printf("\tTipo de inodo: %c\n",in.tipo);
			printf("\tPermisos: %d\n",in.permisos);
			printf("\tNumero de links: %d\n",in.nlinks);
			printf("\tTamaño en bytes logicos: %d\n",in.tamEnBytesLog);
			printf("\tNumero de bloques que ocupa: %d\n",in.numBloquesOcupados);
			ts = localtime(&in.atime);
			strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.mtime);
			strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
			ts = localtime(&in.ctime);
			strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
			printf("\tATIME: %s\n\tMTIME: %s\n\tCTIME: %s\n",atime,mtime,ctime);
return 0;
}

