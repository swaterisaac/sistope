#include "../incl/Estructuras.h"
#include "../incl/EstructuraComun.h"
#include "../incl/shared.h"
#include <stdlib.h>
#include <stdio.h>

/*
----------------agregar----------------
Entradas: un monitor y una visibilidad.
Salidas: no tiene.
Descripción: Agrega una visibilidad al monitor, o bien, la agrega al buffer del monitor.
*/
void agregar(Monitor* monitor, Visibilidad* filaAgregar){

    //printf("\nElementos actuales del buffer: %d y valor del largo máximo: %d",monitor->buffer->elementosActuales,largoMaximo);
    if(monitor->buffer->elementosActuales == largoMaximo){

        //printf("Llego al max\n");

        pthread_cond_wait(&monitor->noLleno,&monitor->mutexProduccion);
    }

    //printf("\n%d\n",monitor->buffer->elementosActuales);

    monitor->buffer->elementosActuales = monitor->buffer->elementosActuales + 1;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->i = filaAgregar->i;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->r = filaAgregar->r;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->w = filaAgregar->w;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->u = filaAgregar->u;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->v = filaAgregar->v;

    //printf("NElementos del buffer %d: %d\n",monitor->index ,monitor->buffer->elementosActuales);

    pthread_cond_signal(&monitor->noVacio);
    return;
}

/*
----------------eliminar----------------
Entradas: un monitor.
Salidas: no tiene.
Descripción: elimina una visiblidad del buffer del monitor.
*/
void eliminar(Monitor* monitor){
    if(monitor->buffer->elementosActuales == 0){
        pthread_cond_wait(&monitor->noVacio,&monitor->mutexRelleno);
    }
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->i = 0;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->w = 0;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->r = 0;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->v = 0;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->u = 0;
    monitor->buffer->elementosActuales = monitor->buffer->elementosActuales - 1;
    pthread_cond_signal(&monitor->noLleno);
    return;
}


/*
----------------inicializarVisibilidad----------------
Entradas: no tiene.
Salidas: una visibilidad.
Descripción: inicializa una visibilidad, reservando la memoria necesario e iniciando en 0 todos los valores de esta.
*/
Visibilidad* inicializarVisibilidad(){
    Visibilidad* visibilidad = (Visibilidad*)malloc(sizeof(Visibilidad));
    visibilidad->i = 0;
    visibilidad->r = 0;
    visibilidad->u = 0;
    visibilidad->v = 0;
    visibilidad->w = 0;
    return visibilidad;
}


/*
----------------inicializarVisibilidades----------------
Entradas: no tiene.
Salidas: un arreglo de visibilidades.
Descripción: inicializa un arreglo de tipo Visibilidad, reservando la memoria necesaria e inicializando cada visibilidad.
*/
Visibilidad** inicializarVisibilidades(){
    Visibilidad** visibilidades = (Visibilidad**)malloc(sizeof(Visibilidad*)*largoMaximo);
    for(int i = 0; i < largoMaximo;i++){
        visibilidades[i] = inicializarVisibilidad();
    }
    return visibilidades;
}


/*
----------------imprimirVisibilidad----------------
Entradas: una visibilidad.
Salidas: no tiene.
Descripción: imprime por pantalla el contenido de una visibilidad.
*/
void imprimirVisibilidad(Visibilidad* visibilidad){
    printf("%lf;%lf;%lf;%lf;%lf\n",visibilidad->u,visibilidad->v,visibilidad->r,visibilidad->i,visibilidad->w);
    return;
}


/*
----------------inicializarBuffer----------------
Entradas: no tiene.
Salidas: un buffer.
Descripción: inicializa un buffer, reservando la memoria necesaria e inicilizando el arreglo de visibilidades dentro de este.
*/
Buffer* inicializarBuffer(){
    Buffer* buffer = (Buffer*)malloc(sizeof(Buffer));
    buffer->elementosActuales = 0;
    buffer->listaVisibilidad = inicializarVisibilidades();
    return buffer;
}
/*
----------------imprimirBuffer----------------
Entradas: un buffer.
Salidas: no tiene.
Descripción: imprime el contenido de un buffer, es decir, imprime cada visibilidad.
*/
void imprimirBuffer(Buffer* buffer){
    for(int i = 0;i < buffer->elementosActuales;i++){
        imprimirVisibilidad(buffer->listaVisibilidad[i]);
    }
    return;
}


/*
----------------inicializarMonitor----------------
Entradas: un entero que representa el índice del monitor.
Salidas: un monitor.
Descripción: inicializa un monitor, reservando la memoria necesaria, inicializando el buffer y 
asignando los valores correspondientes a los datos de la estructura. Además se inicializan las variables de condición y los mutex.
*/
Monitor* inicializarMonitor(int index){
    Monitor* monitor = (Monitor*)malloc(sizeof(Monitor));
    monitor->buffer = inicializarBuffer();
    monitor->agregar = agregar;
    monitor->eliminar = eliminar;
    monitor->index = index;
    monitor->trabajando = 1;
    pthread_cond_init(&monitor->noLleno, NULL);
    pthread_cond_init(&monitor->noVacio, NULL);
    pthread_cond_init(&monitor->bufferLleno, NULL);
    pthread_mutex_init(&monitor->mutexProduccion,NULL);
    pthread_mutex_init(&monitor->mutexRelleno,NULL);
    return monitor;
}

/*
----------------inicializarMonitores----------------
Entradas: no tiene.
Salidas: un arreglo de tipo Monitor.
Descripción: iniciliza los monitores, reservando la memoria necesaria para el arreglo e iniciando cada monitor dentro de dicho arreglo.
*/
Monitor** inicializarMonitores(){
    Monitor** monitores = (Monitor**)malloc(sizeof(Monitor*)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        monitores[i] = inicializarMonitor(i);
    }
    return monitores;
}


/*
----------------vaciarBuffer----------------
Entradas: un monitor y un buffer auxiliar.
Salidas: no tiene.
Descripción: elimina el contenido del buffer pero los copia en un buffer auxiliar, para poder hacer los cálculos.
*/
void vaciarBuffer(Monitor* monitor, Buffer* bufferAux){
    int cantidadElementosMonitor = monitor->buffer->elementosActuales;
    int cantidadElementosMonitorEst = monitor->buffer->elementosActuales;
    for(int i = 0; i < cantidadElementosMonitorEst;i++){

        /*
        printf("soy el iterador %d de la funcion vaciarBuffer\n",i);
        printf("Cantidad de elementos estaticos %d\n",cantidadElementosMonitorEst);
        printf("cantidad de elementos del bufferAux %d\n",bufferAux->elementosActuales);
        */

        bufferAux->elementosActuales = bufferAux->elementosActuales + 1;
        bufferAux->listaVisibilidad[i]->i = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->i;
        bufferAux->listaVisibilidad[i]->r = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->r;
        bufferAux->listaVisibilidad[i]->u = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->u;
        bufferAux->listaVisibilidad[i]->v = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->v;
        bufferAux->listaVisibilidad[i]->w = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->w;
        
        //printf("-----------------Entrando al lock de eliminar\n");

        pthread_mutex_lock(&monitor->mutexProduccion);

        //printf("-----------------Eliminando\n");

        monitor->eliminar(monitor);
        pthread_mutex_unlock(&monitor->mutexProduccion);

        //printf("-----------------Saliendo del lock de eliminar\n");
        
        cantidadElementosMonitor = monitor->buffer->elementosActuales;        
    }

    //printf("Saliendo del ciclo for en vaciarBuffer, el aux tiene %d\n",bufferAux->elementosActuales);

    //DEBERIA SER EN ELIMINAR PERO HAY PROBLEMAS
    

    //imprimirBuffer(bufferAux);
    return;
}


/*
----------------calcularVisibilidad----------------
Entradas: un monitor.
Salidas: no tiene.
Descripción: las hebras vacían su buffer y hacen los cálculos una vez que el buffer esté lleno, 
*/
void* calcularVisibilidad(void* monitor){
    Buffer* bufferAux = inicializarBuffer();
    Monitor* monitorR = (Monitor*)monitor;
    int index = monitorR->index;
    //Quede esperando hasta que el buffer del monitor esté lleno.
    while(monitorR->trabajando != 0){

        //printf("Soy la hebra %d y estoy esperando...\n",monitorR->index);

        pthread_cond_wait(&monitorR->bufferLleno,&monitorR->mutexRelleno);

        //printf("Soy la hebra %d y deje de esperar! (vaciando buffer...)\n",monitorR->index);

        //Vaciar buffer
        vaciarBuffer(monitorR,bufferAux);

        //printf("Soy la hebra %d y deje de vaciar el buffer, ahora escribire en la estructura\n",monitorR->index);
        
        pthread_mutex_lock(&estructuraComun.mutexEstructura);
        actualizarValorDisco(estructuraComun.discos[index],bufferAux);
        pthread_mutex_unlock(&estructuraComun.mutexEstructura);

    }
    return NULL;
}


/*----------------inicializarHebras----------------
Entradas: un arreglo de tipo Monitor.
Salidas: un arreglo de hebras.
Descripción: crea las hebras para que calculen la visibilidad del monitor que les corresponda.
*/

pthread_t* inicializarHebras(Monitor** monitores){
    pthread_t* arregloHebras = (pthread_t*)malloc(sizeof(pthread_t)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        //pthread_t aux;
        //arregloHebras[i] = aux;
        pthread_create(&arregloHebras[i],NULL,calcularVisibilidad,(void*)monitores[i]);
    }
    /*for(int i = 0; i < cantidadDiscos;i++){
        pthread_create(&arregloHebras[i],NULL,calcularVisibilidad,(void*)monitores[i]);
    }*/
    return arregloHebras;
}

/*
----------------terminoLectura----------------
Entradas: un arreglo de monitores y un arreglo de hebras.
Salidas: no tiene.
Descripción: función para indicar que se terminó de leer el archivo, por ende se envía una señal a cada buffer indicando que están llenos y no hay
más datos para que deje de trabajar.
*/

void terminoLectura(Monitor** monitores,pthread_t* hebras){
    for(int i = 0; i < cantidadDiscos;i++){
        pthread_cond_signal(&monitores[i]->bufferLleno);
        monitores[i]->trabajando = 0;
    }
    for(int i = 0; i < cantidadDiscos;i++){
        pthread_join(hebras[i],NULL);
    }
    //Despues de esperar, se deben liberar los monitores y las hebras.
    for(int i = 0 ; i < cantidadDiscos;i++){
        free(monitores[i]->buffer);
        free(monitores[i]);
    }
    free(monitores);
    free(hebras);
    return;
}