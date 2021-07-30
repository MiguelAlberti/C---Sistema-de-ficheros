#include "ficheros.h"


int mi_write_f(unsigned int ninodo, const void *buf_original,  unsigned int offset, unsigned int nbytes){
 
 //Comprobaremos que los permisos del inodo permiten la escritura
 struct inodo inodo;
 leer_inodo(ninodo,&inodo);

 if((inodo.permisos & 2)!=2){
     printf("Error: violación de privilegios \n");
     return -1;
 }
//Primer y último bloque lógico a escribir
 unsigned int PrimerBL=offset/BLOCKSIZE;
 unsigned int desp1=offset%BLOCKSIZE;

 unsigned int UltimoBL=(offset+nbytes-1)/BLOCKSIZE;
 unsigned int desp2=(offset+nbytes-1)%BLOCKSIZE;

//Comprobar que PrimerBL no coincide con UltimoBL ---Revisar

unsigned char buf_bloque_leido[BLOCKSIZE];
int bfisico;

if(PrimerBL==UltimoBL){
   bfisico= traducir_bloque_inodo(ninodo,PrimerBL,1);

    if(bread(bfisico,buf_bloque_leido)==-1){
        return -1;
    }
    memcpy(buf_bloque_leido+desp1, buf_original, nbytes);

    if(bwrite(bfisico,buf_bloque_leido)==-1){
        return -1;
    }
}else{

//Caso normal , PBL != UBL
bfisico=traducir_bloque_inodo(ninodo,PrimerBL,1);
if(bread(bfisico,buf_bloque_leido)==-1){
    return -1;
}
memcpy(buf_bloque_leido+desp1, buf_original,BLOCKSIZE-desp1);

if(bwrite(PrimerBL,buf_bloque_leido)==-1){
    return -1;
}

//Volcamos bloques intermedios

for(int i=(offset/BLOCKSIZE)+1; i<UltimoBL;i++){
    bfisico=traducir_bloque_inodo(ninodo,i,1);
    if(bwrite(bfisico, buf_original+(BLOCKSIZE-desp1)+(i-PrimerBL-1)*BLOCKSIZE)==-1){
        return -1;
    }
}

//El último bloque está parcialmente ocupado
bfisico=traducir_bloque_inodo(ninodo, UltimoBL ,1);

if(bread(bfisico, buf_bloque_leido)==-1){
    return -1;
}
memcpy(buf_bloque_leido, buf_original+ (nbytes-desp2-1),desp2+1);
if(bwrite(bfisico,buf_bloque_leido)==-1){
    return -1;
    }

}
//Modificar info inodo, ya que hemos escrito datos
leer_inodo(ninodo,&inodo);
if((offset+nbytes)>inodo.tamEnBytesLog){
    inodo.tamEnBytesLog=offset+nbytes;
    inodo.ctime=time(NULL);
}
inodo.mtime=time(NULL);
escribir_inodo(ninodo, inodo);

return nbytes;

}


int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes){

//Comprobamos los permisos de lectura 
    struct inodo inodo;
    leer_inodo(ninodo,&inodo);
    if((inodo.permisos & 4)!= 4){
    printf("Error: violación de privilegios \n");
        return -1;
    }
    
    int leidos;

    if(offset >= inodo.tamEnBytesLog){
        leidos=0;
        return leidos;
    }

    if((offset + nbytes) >= inodo.tamEnBytesLog){
        nbytes=inodo.tamEnBytesLog-offset;
        //leemos sólo los bytes desde el offset hasta EOF
    }

    //Primer y último bloque lógico a escribir
    unsigned int PrimerBL=offset/BLOCKSIZE;
    unsigned int desp1=offset%BLOCKSIZE;

    unsigned int UltimoBL=(offset+nbytes-1)/BLOCKSIZE;
    unsigned int desp2=(offset+nbytes-1)%BLOCKSIZE;

    //PBL=UBL
    unsigned char buf_bloque_leido[BLOCKSIZE];
    int bfisico;

    if(PrimerBL==UltimoBL){
    bfisico= traducir_bloque_inodo(ninodo,PrimerBL,0);

    if(bread(bfisico,buf_bloque_leido)==-1){
        return -1;
    }
    memcpy(buf_original, buf_bloque_leido+desp1, nbytes); 
        leidos=nbytes;
    }else{

//Caso PrimerBL != UltimoBL
    bfisico= traducir_bloque_inodo(ninodo,PrimerBL,0);
    if(bread(bfisico, buf_bloque_leido)==-1){
        return -1;
    }
    memcpy(buf_original, buf_bloque_leido+desp1,BLOCKSIZE-desp1);
    leidos=BLOCKSIZE-desp1;

    //lectura de bloques intermedios
    for(int i=(offset/BLOCKSIZE)+1; i<UltimoBL;i++){
        bfisico=traducir_bloque_inodo(ninodo,i,0);
        if(bfisico!=-1){
        if(bread(bfisico, buf_bloque_leido)==-1){
            return -1;
        }
        memcpy(buf_original + (BLOCKSIZE-desp1) + (i-PrimerBL-1)*BLOCKSIZE,buf_bloque_leido,BLOCKSIZE);
        
    }
    leidos=leidos + BLOCKSIZE;
    

    //lectura del ultimo bloque, puede estar parcialmente ocupado
    bfisico=traducir_bloque_inodo(ninodo, UltimoBL,0);
    if(bread(bfisico, buf_bloque_leido)==-1){
        return -1;
    }

    memcpy(buf_bloque_leido, buf_original+ (nbytes-desp2-1),desp2+1);
    leidos+=desp2+1;
    }

}
inodo.atime=time(NULL);
escribir_inodo(ninodo, inodo);

    return leidos;
}

int mi_stat_f(unsigned int ninodo, struct STAT *p_stat){
    struct inodo inodo;
    leer_inodo(ninodo,&inodo);

    p_stat->tipo=inodo.tipo;
    p_stat->permisos=inodo.permisos;
    p_stat->atime=inodo.atime;
    p_stat->ctime=inodo.ctime;
    p_stat->mtime=inodo.mtime;
    p_stat->nlinks=inodo.nlinks;
    p_stat->tamEnBytesLog=inodo.tamEnBytesLog;
    p_stat->numBloquesOcupados=inodo.numBloquesOcupados;

    return 0;
}

int mi_chmod_f(unsigned int ninodo, unsigned char permisos){
    struct inodo inodo;
    leer_inodo(ninodo,&inodo);

    inodo.permisos=permisos;
    inodo.ctime=time(NULL);

    escribir_inodo(ninodo,inodo);
    return 0;
}

int mi_truncar_f(unsigned int ninodo, unsigned int nbytes){
    struct inodo inodo;
    leer_inodo(ninodo,&inodo);
    if((inodo.permisos & 2)!=2){
        printf("Privilege error al truncar. \n");
        return -1;
    }

    if(inodo.tamEnBytesLog<nbytes){
        return -1;
    }
    
    //nº bloque logico a liberar
    int nblogico;
    if(nbytes%BLOCKSIZE==0) nblogico=nbytes/BLOCKSIZE;
    else nblogico= (nbytes/BLOCKSIZE)+1;

    int liberados= liberar_bloques_inodo(ninodo,nblogico);
	leer_inodo(ninodo,&inodo);
    inodo.mtime=time(NULL);
    inodo.ctime=time(NULL);
    inodo.tamEnBytesLog=nbytes;
    escribir_inodo(ninodo,inodo);

    return liberados;
}
