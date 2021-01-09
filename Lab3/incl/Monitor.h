#ifndef MONITOR_H
#define MONITOR_H
void agregar(Monitor* monitor, Visibilidad* filaAgregar);
void eliminar(Monitor* monitor);
Visibilidad* inicializarVisibilidad();
Visibilidad** inicializarVisibilidades();
void imprimirVisibilidad(Visibilidad* visibilidad);
Buffer* inicializarBuffer();
void imprimirBuffer(Buffer* buffer);
Monitor* inicializarMonitor(int index);
Monitor** inicializarMonitores();
void vaciarBuffer(Monitor* monitor, Buffer* bufferAux);
void* calcularVisibilidad(void* monitor);
pthread_t* inicializarHebras(Monitor** monitores);
void terminoLectura(Monitor** monitores,pthread_t* hebras);
#endif