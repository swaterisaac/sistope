#ifndef ESTRUCTURACOMUN_H
#define ESTRUCTURACOMUN_H
Disco* inicializarDisco();
int calcularDisco(Visibilidad* visibilidad);
void imprimirDisco(Disco* disco);
void inicializarEstructura();
void imprimirEstructura(EstructuraComun estructura);
void imprimirVisualizaciones(EstructuraComun estructura);
void actualizarValorDisco(Disco* disco, Buffer* valores);
#endif