salida_out = salida
child_process = Comp


salida_headers = funciones.h
salida_source  = $(salida_headers:.h=.c) coordinador.c 
salida_objects = $(salida_source:.c=.o)

gray_headers = funciones.h
gray_process_source  = $(gray_headers:.h=.c) comparador.c 
gray_process_objects = $(gray_process_source:.c=.o)

CC     = gcc
CFLAGS = -Wall

depends = .depends

build : $(salida_out) 
build : $(child_process)

$(salida_out) : $(salida_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(child_process) : $(gray_process_objects)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(objects) :
	$(CC) $(CFLAGS) -c -o $@ $*.c

$(depends) : $(salida_source) $(salida_headers)
	@$(CC) -MM $(salida_source) > $@


clean :
	$(RM) $(salida_out) $(salida_objects) $(zipfile) $(depends) $(child_process) $(gray_process_objects)

.PHONY : build zip clean

sinclude $(depends)
