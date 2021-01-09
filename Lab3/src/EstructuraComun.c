#include "../incl/Estructuras.h"
#include "../incl/shared.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
----------------inicializarDisco----------------
Entradas: No posee entradas.
Salidas: Una estructura de tipo Disco.
Descripción: Inicializa una estructura de tipo disco, reservando la memoria necesaria e iniciando en 0 los datos.
*/
Disco* inicializarDisco(){
    Disco* disco = (Disco*)malloc(sizeof(Disco));
    disco->cantidadVisibilidades = 0;
    disco->mediaImaginaria = 0;
    disco->mediaReal = 0;
    disco->potencia = 0;
    disco->ruidoTotal = 0;
    return disco;
}

/*
----------------calcularDisco----------------
Entradas: Una estrutura de tipo Visibilidad.
Salidas: Un entero que representa a qué disco le corresponde la visibilidad.
Descripción: Obtiene el número de disco que le corresponde a la visibilidad ingresada como parámetro.
*/
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

/*
----------------imprimirDisco----------------
Entradas: Una estructura de tipo Disco.
Salidas: No tiene.
Descripción: Imprime el contenido de una estructura de tipo Disco.
*/
void imprimirDisco(Disco* disco){
    printf("Cantidad visibilidades: %d\n",disco->cantidadVisibilidades);
    printf("Media imaginaria: %f\n",disco->mediaImaginaria);
    printf("Media real: %f\n",disco->mediaReal);
    printf("Potencia: %f\n",disco->potencia);
    printf("Ruido total: %f\n",disco->ruidoTotal);
    return;
}

//FUNCIONES ESTRUCTURA
/*
----------------inicializarEstructura----------------
Entradas: No tiene.
Salidas: No tiene.
Descripción: Inicializa la estructura común, reservando la memoria necesaria para esta y para cada disco dentro de la estructura. 
*/
void inicializarEstructura(){
    estructuraComun.discos = (Disco**)malloc(sizeof(Disco*)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        estructuraComun.discos[i] = inicializarDisco();
    }
    pthread_mutex_init(&estructuraComun.mutexEstructura,NULL);
    return;
}


/*
----------------imprimirEstructura----------------
Entradas: Una Estructura Común.
Salidas: No tiene.
Descripción: Muestra por pantalla el contenido de la estructura común, que corresponde al contenido de todos los discos.
*/
void imprimirEstructura(EstructuraComun estructura){
    for(int i = 0; i < cantidadDiscos;i++){
        printf("Disco %d:\n",i);
        imprimirDisco(estructura.discos[i]);
        printf("\n");
    }
    return;
}

/*
----------------imprimirVisualizaciones----------------
Entradas: Una estructura común.
Salidas: No tiene.
Descripción: Imprime el número de hebra y la cantidad de visibildiades que procesó.
*/
void imprimirVisualizaciones(EstructuraComun estructura){
    for(int i = 0; i < cantidadDiscos;i++){
        printf("Soy la hebra %d, y procese %d visibilidades\n",i,estructuraComun.discos[i]->cantidadVisibilidades);
    }
    return;
}


/*
----------------actualizarValorDisco----------------
Entradas: Un disco y un buffer.
Salidas: No tiene.
Descripción: Actualiza los valores del disco según el contenido del buffer. Estos son: la media real ,media imaginaria, la potencia y el ruido total.
*/
void actualizarValorDisco(Disco* disco, Buffer* valores){
    disco->mediaReal = disco->mediaReal * disco->cantidadVisibilidades;
    disco->mediaImaginaria = disco->mediaImaginaria * disco->cantidadVisibilidades;
    for(int i = 0; i < valores->elementosActuales;i++){
        //printf("%d disco 1;",i);
        disco->mediaReal = (disco->mediaReal + valores->listaVisibilidad[i]->r);
        //printf("%d disco 2;",i);
        disco->mediaImaginaria = (disco->mediaImaginaria + valores->listaVisibilidad[i]->i);
        //printf("%d disco 3;",i);
        disco->potencia = disco->potencia + sqrt(pow(valores->listaVisibilidad[i]->r,2) + pow(valores->listaVisibilidad[i]->i,2));
        //printf("%d disco 4;",i);
        disco->ruidoTotal = disco->ruidoTotal + valores->listaVisibilidad[i]->w;
        //printf("%d disco 5\n",i);
        
    }
    disco->cantidadVisibilidades = disco->cantidadVisibilidades + valores->elementosActuales;
    for(int i=0;i<valores->elementosActuales;i++){
        valores->listaVisibilidad[i]->i=0;
        valores->listaVisibilidad[i]->r=0;
        valores->listaVisibilidad[i]->w=0;
        valores->listaVisibilidad[i]->u=0;
        valores->listaVisibilidad[i]->v=0;

    }
    valores->elementosActuales = 0;
    
    disco->mediaReal = disco->mediaReal/disco->cantidadVisibilidades;
    disco->mediaImaginaria = disco->mediaImaginaria/disco->cantidadVisibilidades;
    return;
}