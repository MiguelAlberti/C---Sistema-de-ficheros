CC=gcc
CFLAGS=-c -g -Wall -std=gnu99
#LDFLAGS=-pthread

SOURCES=bloques.c  ficheros_basico.c mi_mkfs.c leer_sf.c ficheros.c leer.c escribir.c truncar.c permitir.c
LIBRARIES=bloques.o ficheros_basico.o ficheros.o 
INCLUDES=bloques.h ficheros_basico.h ficheros.h
PROGRAMS= mi_mkfs leer_sf leer escribir truncar permitir
OBJS=$(SOURCES:.c=.o)

all: $(OBJS) $(PROGRAMS)

$(PROGRAMS): $(LIBRARIES) $(INCLUDES)
	$(CC) $(LDFLAGS) $(LIBRARIES) $@.o -o $@
	

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf *.o *~ $(PROGRAMS)
