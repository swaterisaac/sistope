#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <stdint.h>
//#include <unistd.h>
#include <string.h>
#include <math.h>

//Ya que los archivos csv siempre van a tener una cantidad fija de columnas.
#define COLUMNAS 5
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

//Variables globales
int largoMaximo, cantidadDiscos,anchoDisco;
EstructuraComun estructuraComun;




//Cabeceras
void* calcularVisibilidad(void* monitor);


//Debug funciones:

/////////////////////////////////////////////////////
/*
FUNCIONES DE MONITOR
*/
void agregar(Monitor* monitor, Visibilidad* filaAgregar){
    printf("\na");
    printf("\nEntrando a la función agregar");
    //printf("\nElementos actuales del buffer: %d y valor del largo máximo: %d",monitor->buffer->elementosActuales,largoMaximo);
    printf("HOLA?");
    if(monitor->buffer->elementosActuales == largoMaximo){
        printf("HOLA?");
        printf("Llego al max");
        
        
        //pthread_cond_signal(&monitor->bufferLleno);
    
        //pthread_cond_wait(&monitor->noLleno,&monitor->mutexRelleno);
        
    }
    monitor->buffer->elementosActuales = monitor->buffer->elementosActuales + 1;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->i = filaAgregar->i;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->r = filaAgregar->r;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->w = filaAgregar->w;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->u = filaAgregar->u;
    monitor->buffer->listaVisibilidad[monitor->buffer->elementosActuales-1]->v = filaAgregar->v;
    printf("\nElementos actuales del buffer: %d",monitor->buffer->elementosActuales);
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
    pthread_cond_signal(&monitor->noLleno);
    return;
}


Visibilidad* inicializarVisibilidad(){
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
Buffer* inicializarBuffer(){
    Buffer* buffer = (Buffer*)malloc(sizeof(Buffer));
    buffer->elementosActuales = 0;
    buffer->listaVisibilidad = inicializarVisibilidades();
    return buffer;
}

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

Monitor** inicializarMonitores(){
    Monitor** monitores = (Monitor**)malloc(sizeof(Monitor*)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        monitores[i] = inicializarMonitor(i);
    }
    return monitores;
}

pthread_t* inicializarHebras(Monitor** monitores){
    pthread_t* arregloHebras = (pthread_t*)malloc(sizeof(pthread_t)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        pthread_t aux;
        arregloHebras[i] = aux;
    }
    for(int i = 0; i < cantidadDiscos;i++){
        pthread_create(&arregloHebras[i],NULL,calcularVisibilidad,(void*)monitores[i]);
    }
    return arregloHebras;
}

Disco* inicializarDisco(){
    Disco* disco = (Disco*)malloc(sizeof(Disco*));
    disco->cantidadVisibilidades = 0;
    disco->mediaImaginaria = 0;
    disco->mediaReal = 0;
    disco->potencia = 0;
    disco->ruidoTotal = 0;
    return disco;
}

void inicializarEstructura(){
    estructuraComun.discos = (Disco**)malloc(sizeof(Disco*)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        estructuraComun.discos[i] = inicializarDisco();
    }
    pthread_mutex_init(&estructuraComun.mutexEstructura,NULL);
}

void vaciarBuffer(Monitor* monitor, Buffer* bufferAux){
    int cantidadElementosMonitor = monitor->buffer->elementosActuales;
    int cantidadElementosMonitorEst = monitor->buffer->elementosActuales;
    for(int i = 0; i < cantidadElementosMonitorEst;i++){
        bufferAux->listaVisibilidad[i]->i = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->i;
        bufferAux->listaVisibilidad[i]->r = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->r;
        bufferAux->listaVisibilidad[i]->u = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->u;
        bufferAux->listaVisibilidad[i]->v = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->v;
        bufferAux->listaVisibilidad[i]->w = monitor->buffer->listaVisibilidad[cantidadElementosMonitor-1]->w;
        bufferAux->elementosActuales = bufferAux->elementosActuales + 1;

        pthread_mutex_lock(&monitor->mutexProduccion);
        monitor->eliminar(monitor);
        pthread_mutex_unlock(&monitor->mutexProduccion);
        
        cantidadElementosMonitor = monitor->buffer->elementosActuales;        
    }
    return;
}

void actualizarValorDisco(Disco* disco, Buffer* valores){
    disco->mediaReal = disco->mediaReal * disco->cantidadVisibilidades;
    disco->mediaImaginaria = disco->mediaImaginaria * disco->cantidadVisibilidades;
    for(int i = 0; i < valores->elementosActuales;i++){
        disco->mediaReal = (disco->mediaReal + valores->listaVisibilidad[i]->r);
        disco->mediaImaginaria = (disco->mediaImaginaria + valores->listaVisibilidad[i]->i);
        disco->potencia = disco->potencia + pow(valores->listaVisibilidad[i]->r,2) + sqrt(pow(valores->listaVisibilidad[i]->i,2));
        disco->ruidoTotal = disco->ruidoTotal + valores->listaVisibilidad[i]->w;
    }
    disco->cantidadVisibilidades = disco->cantidadVisibilidades + valores->elementosActuales;
    disco->mediaReal = disco->mediaReal/disco->cantidadVisibilidades;
    disco->mediaImaginaria = disco->mediaImaginaria/disco->cantidadVisibilidades;
    return;
}


int obtenerLineas(FILE *archivo){
    int contador = 0;
    double temp;
    while(fscanf(archivo,"%lf,", &temp)!=EOF){
        contador++;
    }
    int lineas = contador/COLUMNAS;
    printf("Cantidad lineas archivo: %d\n",lineas);
    fclose(archivo);
    return lineas;
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

void* calcularVisibilidad(void* monitor){
    Buffer* bufferAux = inicializarBuffer();
    Monitor* monitorR = (Monitor*)monitor;
    int index = monitorR->index;    
    //Quede esperando hasta que el buffer del monitor esté lleno.
    while(monitorR->trabajando != 0){
        pthread_cond_wait(&monitorR->bufferLleno,&monitorR->mutexRelleno);
        //Vaciar buffer
        vaciarBuffer(monitorR,bufferAux);

        pthread_mutex_lock(&estructuraComun.mutexEstructura);
        actualizarValorDisco(estructuraComun.discos[index],bufferAux);
        pthread_mutex_unlock(&estructuraComun.mutexEstructura);

    }
    return NULL;
}

void terminoLectura(Monitor** monitores){
    for(int i = 0; i < cantidadDiscos;i++){
        pthread_cond_signal(&monitores[i]->bufferLleno);
        monitores[i]->trabajando = 0;
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
    int subIndex = 0;

    //Creamos un arreglo de Visibilidad (fila del csv) para ir guardando
    Visibilidad* aux;
    //Se vuelve a abrir el archivo
    archivo = fopen(nombreArchivo,"r");

    for (int i=0; i<lineas;i++){
        aux = (Visibilidad*)malloc(sizeof(Visibilidad));
        fscanf(archivo,"%lf,%lf,%lf,%lf,%lf,",&aux->u,&aux->v,&aux->r,&aux->i,&aux->w);

        //Se calcula el disco al que va la fila con la función d(u,v) = (u**2 + v**2)**0.5 y el ancho del radio.
        index = calcularDisco(aux);
        printf("\nINDEX: %d\n",index);


        //Se agrega al monitor correspondiente con el index y la funcion agregar los valores de esa Visibilidad (fila).
        //Hay que asegurarse de que se hace un lock antes de agregar y unlock después de agregar.
        //Si la cantidad de elementos del monitor es igual al tamaño maximo del buffer, la hebra se desbloquea para hacer
        //los cálculos.
        //Mutex lock
        printf("\nantes lock");
        printf("\nElementos en buffer: %d",monitores[index]->buffer->elementosActuales);
        pthread_mutex_lock(&monitores[index]->mutexProduccion);
        //pthread_mutex_lock(&monitores[index]->mutexProduccion);
        printf("\ndespues lock");
        monitores[index]->agregar(monitores[index],aux);

        //Mutex unlock
        pthread_mutex_unlock(&monitores[index]->mutexProduccion);
        //pthread_mutex_unlock(&monitores[index]->mutexProduccion);
        
        printf("\n%lf,%lf,%lf,%lf,%lf\n",monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->u,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->v,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->r,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->i,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->w);
        free(aux);
    }

    //Cuando termine de leer el archivo, se manda una señal para desbloquear la hebra
    //y que calcule del buffer sin necesidad que esté lleno.
    free(aux);
    fclose(archivo);
    terminoLectura(monitores);

    return;
}


int main(){
    inicializarEstructura();
    cantidadDiscos = 4;
    anchoDisco = 50;
    largoMaximo = 100;

    leerArchivo("prueba2.csv");

    //Argumentos a recibir por línea de comandos (esto será parte del getopt)
    /*
    int radios; //número de radios
    int anchoIntervalo; //ancho de cada intervalo
    char *archivoEntrada; 
    char *archivoSalida;
    int tamBuffer; //tamaño del buffer de cada monitor
    */
    /*
    El proceso principal debe crear tantos monitores y hebras como n ́umero de discos hayan sido especifi-
    cados (una hebra por cada disco). Además de mutex, variables de condici ́on y cualquier herramienta
    de pthreads que sea necesaria para proveer exclusi ́on mutua y sincronizaci ́on.
    */

    //Aquí supongo que habría que crear la hebra principal y generar las hebras y monitores
    //pthread_t h0; //se crea la hebra principal (hebra padre)
    //pthread_create(&h0, NULL, generarHebras_Monitores, algo como un struct );


    return 0;
}

