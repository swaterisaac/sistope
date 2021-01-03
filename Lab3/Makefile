salida_out = lab3

salida_headers = ./incl/EstructuraComun.h ./incl/Getopt.h ./incl/Monitor.h
salida_source  = $(./incl/salida_headers:.h=.c) ./src/EstructuraComun.c ./src/Getopt.c ./src/Monitor.c ./src/lab3.c
salida_objects = $(salida_source:.c=.o)

CC     = gcc
CFLAGS = -Wall -pthread

depends = .depends

build : $(salida_out) 

$(salida_out) : $(salida_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm


$(objects) :
	$(CC) $(CFLAGS) -c -o $@ $*.c

$(depends) : $(salida_source) $(salida_headers)
	@$(CC) -MM $(salida_source) > $@


clean :
	$(RM) $(salida_out) $(salida_objects) $(zipfile) $(depends)

.PHONY : build zip clean

sinclude $(depends)
