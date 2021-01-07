#include "../incl/Estructuras.h"
#include "../incl/shared.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

Disco* inicializarDisco(){
    Disco* disco = (Disco*)malloc(sizeof(Disco));
    disco->cantidadVisibilidades = 0;
    disco->mediaImaginaria = 0;
    disco->mediaReal = 0;
    disco->potencia = 0;
    disco->ruidoTotal = 0;
    return disco;
}

int calcularDisco(Visibilidad* visibilidad){
    double resultado = sqrt(pow(visibilidad->u,2) + pow(visibilidad->v,2));
    int ite = 0;
    for(int i = 0; i < cantidadDiscos - 1;i++){
        if(resultado >= ite && resultado < anchoDisco*(i+1)){
            return i;
        }
        ite += anchoDisco;
    }
    return cantidadDiscos-1;
}

void imprimirDisco(Disco* disco){
    printf("Cantidad visibilidades: %d\n",disco->cantidadVisibilidades);
    printf("Media imaginaria: %f\n",disco->mediaImaginaria);
    printf("Media real: %f\n",disco->mediaReal);
    printf("Potencia: %f\n",disco->potencia);
    printf("Ruido total: %f\n",disco->ruidoTotal);
    return;
}

//FUNCIONES ESTRUCTURA
void inicializarEstructura(){
    estructuraComun.discos = (Disco**)malloc(sizeof(Disco*)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        estructuraComun.discos[i] = inicializarDisco();
    }
    pthread_mutex_init(&estructuraComun.mutexEstructura,NULL);
    return;
}

void imprimirEstructura(EstructuraComun estructura){
    for(int i = 0; i < cantidadDiscos;i++){
        printf("Disco %d:\n",i);
        imprimirDisco(estructura.discos[i]);
        printf("\n");
    }
    return;
}

void imprimirVisualizaciones(EstructuraComun estructura){
    for(int i = 0; i < cantidadDiscos;i++){
        printf("Soy la hebra %d, y procese %d visibilidades\n",i,estructuraComun.discos[i]->cantidadVisibilidades);
    }
    return;
}

void actualizarValorDisco(Disco* disco, Buffer* valores){
    disco->mediaReal = disco->mediaReal * disco->cantidadVisibilidades;
    disco->mediaImaginaria = disco->mediaImaginaria * disco->cantidadVisibilidades;
    for(int i = 0; i < valores->elementosActuales;i++){
        //printf("%d disco 1;",i);
        disco->mediaReal = (disco->mediaReal + valores->listaVisibilidad[i]->r);
        //printf("%d disco 2;",i);
        disco->mediaImaginaria = (disco->mediaImaginaria + valores->listaVisibilidad[i]->i);
        //printf("%d disco 3;",i);
        disco->potencia = disco->potencia + pow(valores->listaVisibilidad[i]->r,2) + sqrt(pow(valores->listaVisibilidad[i]->i,2));
        //printf("%d disco 4;",i);
        disco->ruidoTotal = disco->ruidoTotal + valores->listaVisibilidad[i]->w;
        //printf("%d disco 5\n",i);
        
    }
    disco->cantidadVisibilidades = disco->cantidadVisibilidades + valores->elementosActuales;
    valores->elementosActuales = 0;
    disco->mediaReal = disco->mediaReal/disco->cantidadVisibilidades;
    disco->mediaImaginaria = disco->mediaImaginaria/disco->cantidadVisibilidades;
    return;
}