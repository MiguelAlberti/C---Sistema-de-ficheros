#include "ficheros_basico.h"

int main(int argc, char **argv){

	bmount(argv[1]);

	struct superbloque SB;
	bread(posSB,&SB);
	
	printf ("\n********Datos del superbloque*********\n");
	printf ("posPrimerBloqueMB = %d\n",SB.posPrimerBloqueMB);
	printf ("posUltimoBloqueMB = %d\n",SB.posUltimoBloqueMB);
	printf ("posPrimerBloqueAI = %d\n",SB.posPrimerBloqueAI);
	printf ("posUltimoBloqueAI = %d\n",SB.posUltimoBloqueAI);
	printf ("posPrimerBloqueDatos = %d\n",SB.posPrimerBloqueDatos);
	printf ("posUltimoBloqueDatos = %d\n",SB.posUltimoBloqueDatos);
	printf ("posInodoRaiz = %d\n",SB.posInodoRaiz);
	printf ("posPrimerInodoLibre = %d\n",SB.posPrimerInodoLibre);
	printf ("cantBloquesLibres = %d\n",SB.cantBloquesLibres);
	printf ("cantInodosLibres = %d\n",SB.cantInodosLibres);
	printf ("totBloques = %d\n",SB.totBloques);
	printf ("totInodos = %d\n",SB.totInodos );

/*	printf ("\nsizeof struct superbloque is: %lu\n", sizeof(struct superbloque));
	printf ("sizeof struct inodo is: %lu\n", sizeof(struct inodo));
	
	printf ("\n********ETAPA 3*********\n");
	
	printf ("\n********Reservar y liberar bloque*********\n");
	printf ("SB.cantBloquesLibres = %d\n",SB.cantBloquesLibres);
	int nbloque=reservar_bloque();
	printf ("El nbloque reservado es %i\n",nbloque);
	bread(posSB,&SB);
	printf ("SB.cantBloquesLibres = %d\n",SB.cantBloquesLibres);
	nbloque=liberar_bloque(nbloque);
	printf ("El nbloque liberado es %i\n",nbloque);
	bread(posSB,&SB);
	printf ("Liberamos el bloque y SB.cantBloquesLibres = %d\n",SB.cantBloquesLibres);
	
	
	printf ("\n********Mapa de bits con bloques de metadatos ocupados********\n");
	
	
	int res=leer_bit(posSB);
	printf("[leer_bit(posSB)→ nbloque:%i, posbyte:%i, posbit:%i, nbloqueMB:%i, nbloqueabs:%i)] valor del bit correspondiente = %i\n"
	,posSB,(posSB/8),(posSB%8),((posSB/8)/BLOCKSIZE),(((posSB/8)/BLOCKSIZE)+SB.posPrimerBloqueMB),res);

	res=leer_bit(SB.posPrimerBloqueMB);
	printf("[leer_bit(SB.posPrimerBloqueMB)→ nbloque:%i, posbyte:%i, posbit:%i, nbloqueMB:%i, nbloqueabs:%i)] valor del bit correspondiente = %i\n",SB.posPrimerBloqueMB,(SB.posPrimerBloqueMB/8),(SB.posPrimerBloqueMB%8),((SB.posPrimerBloqueMB/8)/BLOCKSIZE),(((SB.posPrimerBloqueMB/8)/BLOCKSIZE)+SB.posPrimerBloqueMB),res);
	
	res=leer_bit(SB.posUltimoBloqueMB);
	printf("[leer_bit(SB.posUltimoBloqueMB)→ nbloque:%i, posbyte:%i, posbit:%i, nbloqueMB:%i, nbloqueabs:%i)] valor del bit correspondiente = %i\n",SB.posUltimoBloqueMB,(SB.posUltimoBloqueMB/8),(SB.posUltimoBloqueMB%8),((SB.posUltimoBloqueMB/8)/BLOCKSIZE),(((SB.posUltimoBloqueMB/8)/BLOCKSIZE)+SB.posPrimerBloqueMB),res);
	
	res=leer_bit(SB.posPrimerBloqueAI);
	printf("[leer_bit(SB.posPrimerBloqueAI)→ nbloque:%i, posbyte:%i, posbit:%i, nbloqueMB:%i, nbloqueabs:%i)] valor del bit correspondiente = %i\n",SB.posPrimerBloqueAI,(SB.posPrimerBloqueAI/8),(SB.posPrimerBloqueAI%8),((SB.posPrimerBloqueAI/8)/BLOCKSIZE),(((SB.posPrimerBloqueAI/8)/BLOCKSIZE)+SB.posPrimerBloqueMB),res);
	
	res=leer_bit(SB.posUltimoBloqueAI);
	printf("[leer_bit(SB.posUltimoBloqueAI)→ nbloque:%i, posbyte:%i, posbit:%i, nbloqueMB:%i, nbloqueabs:%i)] valor del bit correspondiente = %i\n",SB.posUltimoBloqueAI,(SB.posUltimoBloqueAI/8),(SB.posUltimoBloqueAI%8),((SB.posUltimoBloqueAI/8)/BLOCKSIZE),(((SB.posUltimoBloqueAI/8)/BLOCKSIZE)+SB.posPrimerBloqueMB),res);
	
	res=leer_bit(SB.posPrimerBloqueDatos);
	printf("[leer_bit(SB.posPrimerBloqueDatos)→ nbloque:%i, posbyte:%i, posbit:%i, nbloqueMB:%i, nbloqueabs:%i)] valor del bit correspondiente = %i\n",SB.posPrimerBloqueDatos,(SB.posPrimerBloqueDatos/8),(SB.posPrimerBloqueDatos%8),((SB.posPrimerBloqueDatos/8)/BLOCKSIZE),(((SB.posPrimerBloqueDatos/8)/BLOCKSIZE)+SB.posPrimerBloqueMB),res);
	
	res=leer_bit(SB.posUltimoBloqueDatos);
	printf("[leer_bit(SB.posUltimoBloqueDatos)→ nbloque:%i, posbyte:%i, posbit:%i, nbloqueMB:%i, nbloqueabs:%i)] valor del bit correspondiente = %i\n",SB.posUltimoBloqueDatos,(SB.posUltimoBloqueDatos/8),(SB.posUltimoBloqueDatos%8),((SB.posUltimoBloqueDatos/8)/BLOCKSIZE),(((SB.posUltimoBloqueDatos/8)/BLOCKSIZE)+SB.posPrimerBloqueMB),res);
	
	

	printf ("\n********Datos del superbloque*********\n");
	struct tm *ts;
	char atime[80];
	char mtime[80];
	char ctime[80];
	struct inodo inodo;
	int ninodo;

	leer_inodo(ninodo, &inodo);
	ts = localtime(&inodo.atime);
	strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
	ts = localtime(&inodo.mtime);
	strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
	ts = localtime(&inodo.ctime);
	strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
	printf ("tipo = %c\n",inodo.tipo);
	printf ("permisos = %d\n",inodo.permisos);
	printf("ID: %d \n",ninodo);
	printf("ATIME: %s \n",atime);
	printf("MTIME: %s \n",mtime);
	printf("CTIME: %s\n",ctime);
	printf ("nlinks = %d\n",inodo.nlinks);
	printf ("tamEnBytesLog; = %d\n",inodo.tamEnBytesLog);
	printf ("numBloquesOcupados = %d\n",inodo.numBloquesOcupados);


	printf ("\n********ETAPA 4*********\n");
	
	int pinodo=reservar_inodo('f',6);
	traducir_bloque_inodo(pinodo,8,'1');
	traducir_bloque_inodo(pinodo,204,'1');
	traducir_bloque_inodo(pinodo,30004,'1');
	traducir_bloque_inodo(pinodo,400004,'1');
	traducir_bloque_inodo(pinodo,16843019,'1');
	
*/
	bumount();
}
