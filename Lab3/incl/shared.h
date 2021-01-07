#include "estructuras.h"
#ifndef SHAREFILE_INCLUDED
#define SHAREFILE_INCLUDED
#ifdef  MAIN_FILE
int largoMaximo, cantidadDiscos, anchoDisco;
EstructuraComun estructuraComun;
#else
extern int largoMaximo, cantidadDiscos, anchoDisco;
extern EstructuraComun estructuraComun;
#endif
#endif