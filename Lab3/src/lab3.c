#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include "../incl/Estructuras.h"
#include "../incl/Monitor.h"
#include "../incl/EstructuraComun.h"
#include "../incl/Getopt.h"

//Ya que los archivos csv siempre van a tener una cantidad fija de columnas.
#define COLUMNAS 5
/*struct Visibilidad{
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
*/
//Variables globales
int largoMaximo, cantidadDiscos,anchoDisco;
EstructuraComun estructuraComun;




//Cabeceras
//void* calcularVisibilidad(void* monitor);


//Debug funciones:

/////////////////////////////////////////////////////
/*
FUNCIONES DE MONITOR
*/
/*void agregar(Monitor* monitor, Visibilidad* filaAgregar){

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

    return;
}
*/

/*Visibilidad* inicializarVisibilidad(){
    Visibilidad* visibilidad = (Visibilidad*)malloc(sizeof(Visibilidad));
    visibilidad->i = 0;
    visibilidad->r = 0;
    visibilidad->u = 0;
    visibilidad->v = 0;
    visibilidad->w = 0;
    return visibilidad;
}
Visibilidad** inicializarVisibilidades(){
    Visibilidad** visibilidades = (Visibilidad**)malloc(sizeof(Visibilidad*)*largoMaximo);
    for(int i = 0; i < largoMaximo;i++){
        visibilidades[i] = inicializarVisibilidad();
    }
    return visibilidades;
}

void imprimirVisibilidad(Visibilidad* visibilidad){
    printf("%lf;%lf;%lf;%lf;%lf\n",visibilidad->u,visibilidad->v,visibilidad->r,visibilidad->i,visibilidad->w);
    return;
}

Buffer* inicializarBuffer(){
    Buffer* buffer = (Buffer*)malloc(sizeof(Buffer));
    buffer->elementosActuales = 0;
    buffer->listaVisibilidad = inicializarVisibilidades();
    return buffer;
}

void imprimirBuffer(Buffer* buffer){
    for(int i = 0;i < buffer->elementosActuales;i++){
        imprimirVisibilidad(buffer->listaVisibilidad[i]);
    }
    return;
}
*/
/*Monitor* inicializarMonitor(int index){
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

Monitor** inicializarMonitores(){
    Monitor** monitores = (Monitor**)malloc(sizeof(Monitor*)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        monitores[i] = inicializarMonitor(i);
    }
    return monitores;
}
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
//FUNCIONES DISCO
/*Disco* inicializarDisco(){
    Disco* disco = (Disco*)malloc(sizeof(Disco));
    disco->cantidadVisibilidades = 0;
    disco->mediaImaginaria = 0;
    disco->mediaReal = 0;
    disco->potencia = 0;
    disco->ruidoTotal = 0;
    return disco;
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
*/
/*void vaciarBuffer(Monitor* monitor, Buffer* bufferAux){
    int cantidadElementosMonitor = monitor->buffer->elementosActuales;
    int cantidadElementosMonitorEst = monitor->buffer->elementosActuales;
    for(int i = 0; i < cantidadElementosMonitorEst;i++){

        
        printf("soy el iterador %d de la funcion vaciarBuffer\n",i);
        printf("Cantidad de elementos estaticos %d\n",cantidadElementosMonitorEst);
        printf("cantidad de elementos del bufferAux %d\n",bufferAux->elementosActuales);
        

        bufferAux->elementosActuales = bufferAux->elementosActuales + 1;
        bufferAux->listaVisibilidad[i]->i = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->i;
        bufferAux->listaVisibilidad[i]->r = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->r;
        bufferAux->listaVisibilidad[i]->u = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->u;
        bufferAux->listaVisibilidad[i]->v = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->v;
        bufferAux->listaVisibilidad[i]->w = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->w;
        
        printf("-----------------Entrando al lock de eliminar\n");

        pthread_mutex_lock(&monitor->mutexProduccion);

        printf("-----------------Eliminando\n");

        monitor->eliminar(monitor);
        pthread_mutex_unlock(&monitor->mutexProduccion);

        printf("-----------------Saliendo del lock de eliminar\n");
        
        cantidadElementosMonitor = monitor->buffer->elementosActuales;        
    }

    printf("Saliendo del ciclo for en vaciarBuffer, el aux tiene %d\n",bufferAux->elementosActuales);

    //DEBERIA SER EN ELIMINAR PERO HAY PROBLEMAS
    pthread_cond_signal(&monitor->noLleno);

    //imprimirBuffer(bufferAux);
    return;
}*/

/*void actualizarValorDisco(Disco* disco, Buffer* valores){
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
}*/


int obtenerLineas(FILE *archivo){
    int contador = 0;
    double temp;
    while(fscanf(archivo,"%lf,", &temp)!=EOF){
        contador++;
    }
    int lineas = contador/COLUMNAS;
    //printf("Cantidad lineas archivo: %d\n",lineas);
    fclose(archivo);
    return lineas;
}



/*int calcularDisco(Visibilidad* visibilidad){
    double resultado = sqrt(pow(visibilidad->u,2) + pow(visibilidad->v,2));
    int ite = 0;
    for(int i = 0; i < cantidadDiscos - 1;i++){
        if(resultado >= ite && resultado < anchoDisco*(i+1)){
            return i;
        }
        ite += anchoDisco;
    }
    return cantidadDiscos-1;
}*/

/*void* calcularVisibilidad(void* monitor){
    Buffer* bufferAux = inicializarBuffer();
    Monitor* monitorR = (Monitor*)monitor;
    int index = monitorR->index;
    //Quede esperando hasta que el buffer del monitor esté lleno.
    while(monitorR->trabajando != 0){

        printf("Soy la hebra %d y estoy esperando...\n",monitorR->index);

        pthread_cond_wait(&monitorR->bufferLleno,&monitorR->mutexRelleno);

        printf("Soy la hebra %d y deje de esperar! (vaciando buffer...)\n",monitorR->index);

        //Vaciar buffer
        vaciarBuffer(monitorR,bufferAux);

        printf("Soy la hebra %d y deje de vaciar el buffer, ahora escribire en la estructura\n",monitorR->index);

        pthread_mutex_lock(&estructuraComun.mutexEstructura);
        actualizarValorDisco(estructuraComun.discos[index],bufferAux);
        pthread_mutex_unlock(&estructuraComun.mutexEstructura);

    }
    return NULL;
}
*/

void terminoLectura(Monitor** monitores,pthread_t* hebras){
    for(int i = 0; i < cantidadDiscos;i++){
        pthread_cond_signal(&monitores[i]->bufferLleno);
        monitores[i]->trabajando = 0;
    }
    for(int i = 0; i < cantidadDiscos;i++){
        pthread_join(hebras[i],NULL);
    }
    return;
}

void escribirArchivo(char* nombreArchivo){

    return;
}

void leerArchivo(char* nombreArchivo){
    
    //Inicializamos un arreglo de monitores de largo Ndiscos
    Monitor** monitores = inicializarMonitores();
    //Inicializamos un arreglo de hebras de largo Ndiscos
    pthread_t* arregloHebras = inicializarHebras(monitores);

    //Creamos las hebras con pthread_create a una función que calcule lo que haya en los monitores, solo cuando el buffer de estos
    //esté lleno o ya no haya más datos que leer. El parámetro de la función serían los monitores declarados arriba.

    //Se empieza a leer el archivo
    FILE* archivo = fopen(nombreArchivo,"r");
    if(archivo == NULL){
        printf("No existe tal archivo.\n");
    }
    //Se obtienen las lineas del archivo
    int lineas = obtenerLineas(archivo);
    //Este index sirve para saber a qué disco se le asigna la fila del csv.
    int index = 0;
    //Este subindex es una variable auxiliar para obtener el largoVisibilidad - 1.
    //int subIndex = 0;

    //Creamos un arreglo de Visibilidad (fila del csv) para ir guardando
    Visibilidad* aux;
    //Se vuelve a abrir el archivo
    archivo = fopen(nombreArchivo,"r");

    for (int i=0; i<lineas;i++){
        aux = (Visibilidad*)malloc(sizeof(Visibilidad));
        fscanf(archivo,"%lf,%lf,%lf,%lf,%lf,",&aux->u,&aux->v,&aux->r,&aux->i,&aux->w);

        //Se calcula el disco al que va la fila con la función d(u,v) = (u**2 + v**2)**0.5 y el ancho del radio.
        index = calcularDisco(aux);

        //Se agrega al monitor correspondiente con el index y la funcion agregar los valores de esa Visibilidad (fila).
        //Hay que asegurarse de que se hace un lock antes de agregar y unlock después de agregar.
        //Si la cantidad de elementos del monitor es igual al tamaño maximo del buffer, la hebra se desbloquea para hacer
        //los cálculos.
        //Mutex lock
        printf("Elementos en buffer: %d\n",monitores[index]->buffer->elementosActuales);

        if(monitores[index]->buffer->elementosActuales == largoMaximo){

            printf("Desbloqueando a la hebra %d...\n",index);

            pthread_cond_signal(&monitores[index]->bufferLleno);
        }
        pthread_mutex_lock(&monitores[index]->mutexProduccion);
        
        monitores[index]->agregar(monitores[index],aux);

        //Mutex unlock
        pthread_mutex_unlock(&monitores[index]->mutexProduccion);
        
        /*printf("\n%lf,%lf,%lf,%lf,%lf\n",monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->u,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->v,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->r,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->i,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->w);*/
        
        free(aux);
    }

    //Cuando termine de leer el archivo, se manda una señal para desbloquear la hebra
    //y que calcule del buffer sin necesidad que esté lleno.
    fclose(archivo);
    printf("Terminé de leer el archivoooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n");
    terminoLectura(monitores,arregloHebras);
    return;
}



int main(int argc, char *argv[]){
    
    //Argumentos a recibir por línea de comandos (esto será parte del getopt)

    char *archivoEntrada; 
    char *archivoSalida;
    int flag;
    recibirArgumentos(argc, argv, &archivoEntrada, &archivoSalida, &anchoDisco, &cantidadDiscos, &largoMaximo, &flag);
    printf("El argumento de flag -i es: %s\n", archivoEntrada);
    printf("El argumento de flag -o es: %s\n", archivoSalida);
    printf("El argumento de flag -d es: %d\n", anchoDisco);
    printf("El argumento de flag -n es: %d\n", cantidadDiscos);
    printf("El argumento de flag -s es: %d\n", largoMaximo);
    if(flag == 1){
        printf("\nMOSTRANDO RESULTADOS\n");
    }
    
    /*cantidadDiscos = 4;
    anchoDisco = 100;
    largoMaximo = 100;
    inicializarEstructura();
    leerArchivo("prueba2.csv");
    imprimirEstructura(estructuraComun);*/

    return 0;
}

