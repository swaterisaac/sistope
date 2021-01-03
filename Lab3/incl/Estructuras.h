#include <pthread.h>
#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

struct Visibilidad{
    double u;
    double v;
    double r;
    double i;
    double w;
}typedef Visibilidad;


struct Disco{
    double mediaReal;
    double mediaImaginaria;
    double potencia;
    double ruidoTotal;
    int cantidadVisibilidades;
}typedef Disco;

struct EstructuraComun{
    Disco** discos;
    pthread_mutex_t mutexEstructura;
}typedef EstructuraComun;

//largoMaximo será una variable global
struct Buffer{
    int elementosActuales;
    Visibilidad** listaVisibilidad;
}typedef Buffer;

struct Monitor{
    int index;
    int trabajando;
    Buffer* buffer;
    pthread_cond_t noVacio;
    pthread_cond_t noLleno;
    pthread_cond_t bufferLleno;
    pthread_mutex_t mutexProduccion;
    pthread_mutex_t mutexRelleno;
    void (*agregar)(struct Monitor*,Visibilidad*);
    void (*eliminar)(struct Monitor*);
}typedef Monitor;

#endif