salida_out = lab2

salida_headers =
salida_source  = $(salida_headers:.h=.c) hist.c
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
