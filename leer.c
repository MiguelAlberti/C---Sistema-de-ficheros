#include "ficheros.h"


int main(int argc, char **argv){
 
if(bmount(argv[1])< 0 ){
    perror("Error bmount");
    return -1;
}

//struct STAT stat;
int tambuffer = BLOCKSIZE;
unsigned char buffer_texto[tambuffer];
//int offset = 0;
//int leidos;
//int totleidos;
int ninodo = atoi(argv[2]);

int i =0;
	memset(buffer_texto,0,tambuffer);

    while (mi_read_f(ninodo,buffer_texto,i*BLOCKSIZE,tambuffer) > 0){
    			i++;
                write(1, buffer_texto, tambuffer);
                //totleidos = totleidos + leidos; 
                memset(buffer_texto,0,tambuffer);
                //offset = offset + tambuffer;
    }

//    mi_stat_f(numeroinodo,&stat);
//    char string[1000];
//    printf(stderr,'bytes leidos %d\n',totleidos);
//    sprintf(tamEnBytesLog, "bytes logicos inodo %d\n", stat.tamEnBytesLog);
//    write(2, tamEnBytesLog, strlen(tamEnBytesLog));


    bumount();

return 0;  

}
