#include "ficheros_basico.h"

int tamMB(unsigned int nbloques){ 

	if((nbloques/8)%BLOCKSIZE==0){
		return (nbloques/8)/BLOCKSIZE;
	}else{
		return ((nbloques/8)/BLOCKSIZE)+1;
	}
}


int tamAI(unsigned int ninodos){ 

	if(((ninodos * INODOSIZE) % BLOCKSIZE)==0){
		return (ninodos * INODOSIZE)/BLOCKSIZE;
	}else{
		return ((ninodos * INODOSIZE)/BLOCKSIZE)+1;
	}
}
 
 
int initSB(unsigned int nbloques, unsigned int ninodos){
    
	struct superbloque SB;
	SB.posPrimerBloqueMB = posSB + tamSB; 
	SB.posUltimoBloqueMB = SB.posPrimerBloqueMB + tamMB(nbloques) - 1;
	SB.posPrimerBloqueAI = SB.posUltimoBloqueMB + 1; 
	SB.posUltimoBloqueAI = SB.posPrimerBloqueAI  + tamAI(ninodos)- 1;
	SB.posPrimerBloqueDatos = SB.posUltimoBloqueAI + 1;
	SB.posUltimoBloqueDatos = nbloques - 1;
	SB.posInodoRaiz = 0;
	SB.posPrimerInodoLibre = 0;
	SB.cantBloquesLibres = nbloques;
	SB.cantInodosLibres = ninodos; 
	SB.totBloques=nbloques;
	SB.totInodos=nbloques/4;

	int w=bwrite(posSB , &SB);
		if(w){
			return 0;
		}
		return w;
}

int initMB(){ 

	struct superbloque SB;
	bread(posSB,&SB);

	unsigned char buffer[BLOCKSIZE];
	memset(buffer,0,BLOCKSIZE);

	for(int j=SB.posPrimerBloqueMB; j<=SB.posUltimoBloqueMB;j++){
		if((bwrite(j, buffer)==-1)){
			return -1; 
		}
	}  
	
	for(int i=posSB;i<=SB.posUltimoBloqueAI;i++){
		escribir_bit(i,1);
		SB.cantBloquesLibres--;
	}
	
	bwrite(posSB,&SB);
	return 0;    
}

int initAI(){

	struct superbloque SB;
	struct inodo ainodos[BLOCKSIZE/INODOSIZE];

	bread(posSB,&SB);
	int contadorInodos=SB.posPrimerInodoLibre+1;

	for(int i= SB.posPrimerBloqueAI;i<=SB.posUltimoBloqueAI;i++){
		for(int j=0; j<(BLOCKSIZE/INODOSIZE); j++){
			ainodos[j].tipo='l';

			if(contadorInodos<SB.totInodos){
				ainodos[j].punterosDirectos[0]=contadorInodos;
				contadorInodos++;
			}else{
				ainodos[j].punterosDirectos[0]=UINT_MAX;
			}
		}
		bwrite(i,ainodos);
	}
	return 0;
}

 

int escribir_bit(unsigned int nbloque, unsigned int bit){ 

	struct superbloque SB;
	bread(posSB,&SB);

	int posbyte = nbloque / 8;
	int posbit = nbloque % 8;
	int nbloqueMB = posbyte/BLOCKSIZE;
	int nbloqueabs = nbloqueMB + SB.posPrimerBloqueMB;

	unsigned char  bufferMB[BLOCKSIZE];
	bread(nbloqueabs,bufferMB);

	posbyte=posbyte%BLOCKSIZE;

	unsigned char mascara = 128; // 10000000
	mascara >>= posbit; // desplazam. bits drcha

	if(bit == 1){
		bufferMB[posbyte] |= mascara; // OR para bits
	}else{
		bufferMB[posbyte] &= ~mascara; //AND y NOT bits
	}
	bwrite(nbloqueabs,bufferMB);
	
	return 0;
}


unsigned char leer_bit(unsigned int nbloque){
     
	struct superbloque SB;
	bread(posSB,&SB);
	
	int posbyte = nbloque / 8;
	int posbit = nbloque % 8;
	int nbloqueMB = posbyte/BLOCKSIZE;
	int nbloqueabs = nbloqueMB + SB.posPrimerBloqueMB;
	
	unsigned char  bufferMB[BLOCKSIZE];
	bread(nbloqueabs,bufferMB);
	
	posbyte=posbyte%BLOCKSIZE;

	unsigned char mascara = 128; // 10000000
	mascara >>= posbit; // desplazamiento de bits a la derecha
	mascara &= bufferMB[posbyte]; // operador AND para bits
	mascara >>= (7-posbit); // desplazamiento de bits a la derecha
	return mascara;
	
}


int reservar_bloque(){ 

		
	struct superbloque SB;
	bread(posSB,&SB);
	
	unsigned char bufferAux[BLOCKSIZE];
	memset(bufferAux,255,BLOCKSIZE);

	unsigned char  bufferMB[BLOCKSIZE];

	if(SB.cantBloquesLibres==0) return -1;
	
	int posBloqueMB=SB.posPrimerBloqueMB;
	
	bread(posBloqueMB,bufferMB);
	
	while(memcmp(bufferMB,bufferAux,BLOCKSIZE)==0){
		posBloqueMB++;
		if(bread(posBloqueMB,bufferMB)==-1){
			return -1;
		}
	}

	int trobat=0;
	int posbyte=0;
	while((posbyte<BLOCKSIZE)&&(trobat==0)){      
		if(bufferMB[posbyte]<255){
			trobat=1;
		}else{
			posbyte++;
		}
	}

	int posbit=0;
	unsigned char mascara = 128; // 10000000
	while(bufferMB[posbyte]& mascara){
		posbit++;	
		bufferMB[posbyte]<<=1;
	}

	int nbloque= ((posBloqueMB -SB.posPrimerBloqueMB) * BLOCKSIZE + posbyte)*8 + posbit;
	escribir_bit(nbloque,1);
	SB.cantBloquesLibres--;

	bwrite(posSB,&SB);	
	return nbloque;        
}

int liberar_bloque(unsigned int nbloque){ 

	struct superbloque SB;
	bread(posSB,&SB);
	escribir_bit(nbloque,0);
    	SB.cantBloquesLibres++;
	bwrite(posSB,&SB);
	return nbloque;
}

int escribir_inodo(unsigned int ninodo, struct inodo inodo){ 

	struct superbloque SB;
	bread(posSB,&SB);
	struct inodo inodos[BLOCKSIZE/INODOSIZE];
	int nbloque=SB.posPrimerBloqueAI + ninodo*(INODOSIZE/BLOCKSIZE); 	
	if(bread(nbloque,inodos)==-1) return -1;   
	inodos[ninodo%(BLOCKSIZE/INODOSIZE)]= inodo;   
	if(bwrite(nbloque, inodos)==-1) return -1;	
	return 0;  
}

int leer_inodo(unsigned int ninodo, struct inodo *inodo){ 

	struct superbloque SB;
     bread(posSB,&SB);
	struct inodo inodos[BLOCKSIZE/INODOSIZE];
	int nbloque=SB.posPrimerBloqueAI + ninodo/(BLOCKSIZE/INODOSIZE); 
	if(bread(nbloque,&inodos)==-1) return -1;
	*inodo= inodos[ninodo%(BLOCKSIZE/INODOSIZE)];
	return 0;
}

int reservar_inodo(unsigned char tipo, unsigned char permisos){ 

	struct superbloque SB;
	if(bread(posSB,&SB)==-1) return -1;
	if(SB.cantInodosLibres==0){
		printf("No quedan inodos libres \n");
		return -1;
	}
	
	struct inodo inodo_libre;
	leer_inodo(SB.posPrimerInodoLibre,&inodo_libre);
	unsigned int posInodoReservado= inodo_libre.punterosDirectos[0];
	

	inodo_libre.tipo=tipo;
	inodo_libre.permisos=permisos;
	inodo_libre.nlinks=1;
	inodo_libre.tamEnBytesLog=0;
	inodo_libre.atime=time(NULL);
	inodo_libre.ctime=time(NULL);
	inodo_libre.mtime=time(NULL);
	inodo_libre.numBloquesOcupados=0;
	for(int i=0;i<12;i++){
	inodo_libre.punterosDirectos[i]=0;
	}
	for(int j=0;j<3;j++){
	inodo_libre.punterosDirectos[j]=0;
	}
	
	escribir_inodo(SB.posPrimerInodoLibre,inodo_libre);
	SB.cantInodosLibres--;

	SB.posPrimerInodoLibre=posInodoReservado;
	if(bwrite(posSB,&SB)==-1) return -1;
	return posInodoReservado;
}

int traducir_bloque_inodo(int ninodo, unsigned int nblogico,char reservar){

	struct inodo inodo;
	unsigned  ptr,ptr_ant,salvar_inodo,nRangoBL,nivel_punteros,indice;
	int buffer[NPUNTEROS];

	leer_inodo(ninodo,&inodo);
	ptr=0; ptr_ant=0; salvar_inodo=0;
	nRangoBL=obtener_nRangoBL(inodo, nblogico, &ptr);
	nivel_punteros=nRangoBL;

	while(nivel_punteros>0){
		if(ptr==0){
			if(reservar==0){
				return -1;
			}else{
				salvar_inodo=1;
				ptr=reservar_bloque();
				inodo.numBloquesOcupados++;
				inodo.ctime=time(NULL);

				if(nivel_punteros==nRangoBL){ //el bloque cuelga directamente del inodo
					inodo.punterosIndirectos[nRangoBL-1]=ptr;
					printf("Puntero: %i\n",inodo.punterosIndirectos[nRangoBL]);
				}else{
					buffer[indice]=ptr;
					printf("buffer[indice]= %i\n",buffer[indice]);
					bwrite(ptr_ant,buffer);
				}
			}
		}
		bread(ptr,buffer);
		indice=obtener_indice(nblogico,nivel_punteros);
		ptr_ant=ptr;
		ptr=buffer[indice];
		nivel_punteros--;
	}

	if(ptr==0){
		if(reservar==0){
			return -1;
		}else{
			salvar_inodo=1;
			ptr=reservar_bloque();
			inodo.numBloquesOcupados++;
			inodo.ctime=time(NULL);

			if(nRangoBL==0){
				inodo.punterosDirectos[nblogico]=ptr;
				printf("punterosDirectos[nblogico]= %i\n",ptr);
			}else{
				buffer[indice]=ptr;
				printf("buffer[indice]= %i\n",ptr);
 				if(bwrite(ptr_ant,buffer)==-1){
					return -1;
				}
			}
		}
	}
	
	if(salvar_inodo==1){
		escribir_inodo(ninodo,inodo);
	}
	return ptr;
}


int obtener_nRangoBL(struct inodo  inodo, unsigned int nblogico,unsigned int *ptr){

	if (nblogico<DIRECTOS){
		return 0;
	}else if (nblogico<INDIRECTOS0){
		*ptr = inodo.punterosIndirectos[0];        
		return 1;
	}else if (nblogico<INDIRECTOS1){
		*ptr = inodo.punterosIndirectos[1];             
		return 2;
	}else if (nblogico<INDIRECTOS2){
		*ptr = inodo.punterosIndirectos[2];                
		return 3; 
	}else{
		*ptr=0;              
		printf("Bloque lógico fuera de rango\n");         
		return -1;  
	}
}

int obtener_indice(int nblogico ,int nivel_punteros ){ 

	if(nblogico<DIRECTOS){
		return nblogico;
	}else if (nblogico<INDIRECTOS0){
		return nblogico-DIRECTOS;
	}else if(nblogico<INDIRECTOS1){
		if(nivel_punteros==2){
			return (nblogico-INDIRECTOS0)/ NPUNTEROS;
		}else if(nivel_punteros==1){
			return (nblogico-INDIRECTOS0)% NPUNTEROS;
		}
	}else if(nblogico<INDIRECTOS2){
		if(nivel_punteros==3){
			return (nblogico-INDIRECTOS1)/(NPUNTEROS*NPUNTEROS);
		}else if(nivel_punteros==2){
			return ((nblogico-INDIRECTOS1)%(NPUNTEROS*NPUNTEROS))/NPUNTEROS;
		}else if(nivel_punteros==1){
			return((nblogico-INDIRECTOS1)%(NPUNTEROS*NPUNTEROS))%NPUNTEROS;
		}
	}  
	return 0;
}          
   
int liberar_bloques_inodo(unsigned int ninodo,unsigned int nblogico){
    
    struct inodo inodo;
    unsigned int nRangoBL,nivel_punteros,indice,ptr,nblog,ultimoBL;
    int bloques_punteros[3][NPUNTEROS];
    int ptr_nivel[3];
    int indices[3];
    int liberados;

    liberados=0;
    leer_inodo(ninodo,&inodo);
    if(inodo.tamEnBytesLog==0){
        return 0;
    }

    if(inodo.tamEnBytesLog%BLOCKSIZE==0){
        ultimoBL=inodo.tamEnBytesLog/BLOCKSIZE-1;
    }
    else{
        ultimoBL=inodo.tamEnBytesLog/BLOCKSIZE;
    }
    ptr=0;
    
    for(nblog=nblogico;nblog==ultimoBL;nblog++){
        nRangoBL=obtener_nRangoBL(inodo,nblog,&ptr);
        if(nRangoBL<0){
            return -1;
        }
        nivel_punteros=nRangoBL;

    while((ptr>0)&&(nivel_punteros>0)){
        bread(ptr,bloques_punteros[nivel_punteros-1]);
        indice=obtener_indice(nblog,nivel_punteros);
        ptr_nivel[nivel_punteros-1]=ptr;
        indices[nivel_punteros-1]=indice;
        ptr=bloques_punteros[nivel_punteros-1][indice];
        nivel_punteros--;
    }

    if(ptr>0){
        liberar_bloque(ptr);
        liberados++;
        if(nRangoBL==0){
            inodo.punterosDirectos[nblog]=0;
        }
        else{
            while(nivel_punteros<nRangoBL){
                indice=indices[nivel_punteros];
                bloques_punteros[nivel_punteros][indice]=0;
                ptr=ptr_nivel[nivel_punteros];

                if(bloques_punteros[nivel_punteros]==0){
                    liberar_bloque(ptr);
                    liberados++;
                    nivel_punteros++;

                    if(nivel_punteros==nRangoBL){
                        inodo.punterosIndirectos[nRangoBL-1]=0;
                    }
                }
                else{
                    bwrite(ptr,bloques_punteros[nivel_punteros]);
                    nivel_punteros=nRangoBL;
                }

            }
        }
    }
    }
	printf("Liberar_bloque_inodo. Total bloques liberados %i\n",liberados);
    return liberados;

}

int liberar_inodo(unsigned int ninodo){
    int liberados=liberar_bloques_inodo(ninodo,0);
    if(liberados==-1){
        perror("error al liberar los bloques\n");
        return -1;
    }

    struct inodo inodo;
    leer_inodo(ninodo, &inodo);
    inodo.tipo='l';

//Actualizar lista enlazada inodos libres
    struct superbloque SB;
    if(bread(posSB,&SB)==-1){
        return -1;
    }

    inodo.punterosDirectos[0]=SB.posPrimerInodoLibre;
    SB.posPrimerInodoLibre=ninodo;
    SB.cantInodosLibres++;
    if(escribir_inodo(ninodo,inodo)==-1){
        perror("error al escribir inodo\n");
        return -1;
    }
    if(bwrite(posSB, &SB)==-1){
        perror("error de escritura del SB\n");
        return -1;
    }

    return ninodo;
}

 
