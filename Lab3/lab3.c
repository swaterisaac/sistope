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

struct Tabla{
    Visibilidad* listaVisibilidad;
    int cantidadFilas;
}typedef Tabla;

struct Disco{
    double mediaReal;
    double mediaImaginaria;
    double potencia;
    double ruidoTotal;
    int cantidadVisibilidades;
}typedef Disco;

struct EstructuraComun{
    Disco** discos;
}typedef EstructuraComun;

//largoMaximo será una variable global
struct Buffer{
    int elementosActuales;
    Visibilidad* listaVisibilidad;
}typedef Buffer;

struct Monitor{
    Buffer* buffer;
    pthread_cond_t noVacio;
    pthread_cond_t noLleno;
    void (*agregar)(struct Monitor*,Visibilidad);
    void (*eliminar)(struct Monitor*);
}typedef Monitor;

//Variables globales
int largoMaximo, cantidadDiscos,anchoDisco;
EstructuraComun estructuraComun;

//Debug funciones:

/////////////////////////////////////////////////////
/*
FUNCIONES DE MONITOR
*/
void agregar(Monitor* monitor, Visibilidad filaAgregar){

    return;
}

void eliminar(Monitor* monitor){
    
    return;
}

Tabla* inicializarTabla(int cantidadFilas){
    Tabla* tablaRet = (Tabla*)malloc(sizeof(Tabla));
    tablaRet->listaVisibilidad = (Visibilidad*)malloc(sizeof(Visibilidad)*cantidadFilas);
    tablaRet->cantidadFilas = cantidadFilas;
    return tablaRet;
}

Tabla** inicializarTablas(int cantidadFilas){
    Tabla** tablas = (Tabla**)malloc(sizeof(Tabla*)* cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        tablas[i] = inicializarTabla(0);
    }
    return tablas;
}
Buffer* inicializarBuffer(){
    Buffer* buffer = (Buffer*)malloc(sizeof(Buffer));
    buffer->elementosActuales = 0;
    buffer->listaVisibilidad = (Visibilidad*)malloc(sizeof(Visibilidad)*largoMaximo);
    return buffer;
}

Monitor* inicializarMonitor(){
    Monitor* monitor = (Monitor*)malloc(sizeof(Monitor));
    monitor->buffer = inicializarBuffer();
    monitor->agregar = agregar;
    monitor->eliminar = eliminar;
    pthread_cond_init(&monitor->noLleno, NULL);
    pthread_cond_init(&monitor->noVacio, NULL);
    return monitor;
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

void aumentarValorDisco(Disco* disco, Visibilidad valores){
    disco->mediaReal = disco->mediaReal * disco->cantidadVisibilidades;
    disco->mediaReal = (disco->mediaReal + valores.r)/(disco->cantidadVisibilidades + 1);
    
    disco->mediaImaginaria = disco->mediaImaginaria * disco->cantidadVisibilidades;
    disco->mediaImaginaria = (disco->mediaImaginaria + valores.i)/(disco->cantidadVisibilidades + 1);

    disco->cantidadVisibilidades = disco->cantidadVisibilidades + 1;

    disco->potencia = disco->potencia + pow(valores.r,2) + sqrt(pow(valores.i,2));

    disco->ruidoTotal = disco->ruidoTotal + valores.w;
    return;
}

void aumentarFila(Tabla* tablaRel){
    tablaRel->listaVisibilidad = (Visibilidad*)realloc(tablaRel->listaVisibilidad,sizeof(Visibilidad)*(tablaRel->cantidadFilas+1));
    tablaRel->cantidadFilas = tablaRel->cantidadFilas + 1;
    return;
}

void imprimirTabla(Tabla* tabla){
    for(int i = 0; i < tabla->cantidadFilas;i++){
        printf("%5lf;%5lf;%5lf;%5lf;%5lf\n",tabla->listaVisibilidad[i].u,tabla->listaVisibilidad[i].v,tabla->listaVisibilidad[i].w,tabla->listaVisibilidad[i].i,tabla->listaVisibilidad[i].w);
    }
    return;
}
void imprimirTablas(Tabla** tablas){
    for(int i = 0; i < cantidadDiscos;i++){
        printf("\nDisco %d:\n\n",i+1);
        imprimirTabla(tablas[i]);
    }
    return;
}

void inicializarEstructuraComun(){
    estructuraComun.discos = (Disco**)malloc(sizeof(Disco*)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos; i++){
        estructuraComun.discos[i] = (Disco*)malloc(sizeof(Disco));
    }
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
int calcularDisco(Visibilidad visibilidad){
    double resultado = sqrt(pow(visibilidad.u,2) + pow(visibilidad.v,2));
    int ite = 0;
    for(int i = 0; i < cantidadDiscos - 1;i++){
        if(resultado >= ite && resultado < anchoDisco*(i+1)){
            return i;
        }
        ite += anchoDisco;
    }
    return cantidadDiscos-1;
}

Tabla** leerArchivo(char* nombreArchivo){
    
    Tabla** tablas = inicializarTablas(0);
    //Inicializamos un arreglo de monitores de largo Ndiscos
    //Monitor** monitores = inicializarMonitores(cantidadDiscos);
    //Inicializamos un arreglo de hebras de largo Nhebras

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
    Visibilidad* aux = (Visibilidad*)malloc(sizeof(Visibilidad)*lineas);
    //Se vuelve a abrir el archivo
    archivo = fopen(nombreArchivo,"r");

    for (int i=0; i<lineas;i++){
        fscanf(archivo,"%lf,%lf,%lf,%lf,%lf,",&aux[i].u,&aux[i].v,&aux[i].r,&aux[i].i,&aux[i].w);
        //Se calcula el disco al que va la fila con la función d(u,v) = (u**2 + v**2)**0.5 y el ancho del radio.
        index = calcularDisco(aux[i]);

        //printf("INDEX: %d\n",index);
        //Se agrega al monitor correspondiente con el index y la funcion agregar los valores de esa Visibilidad (fila).
        //Hay que asegurarse de que se hace un lock antes de agregar y unlock después de agregar.
        //Si la cantidad de elementos del monitor es igual al tamaño maximo del buffer, la hebra se desbloquea para hacer
        //los cálculos.
        
        aumentarFila(tablas[index]);
        subIndex = tablas[index]->cantidadFilas - 1;
        //printf("Soy la tabla %d y mi cantidad de filas son %d\n\n",index,tablas[index]->cantidadFilas);
        tablas[index]->listaVisibilidad[subIndex].u = aux[i].u;
        tablas[index]->listaVisibilidad[subIndex].v = aux[i].v;
        tablas[index]->listaVisibilidad[subIndex].r = aux[i].r;
        tablas[index]->listaVisibilidad[subIndex].i = aux[i].i;
        tablas[index]->listaVisibilidad[subIndex].w = aux[i].w;
        
        //printf("%lf,%lf,%lf,%lf,%lf\n",aux[i].u,aux[i].v,aux[i].r,aux[i].i,aux[i].w);
    }
    //Cuando termine de leer el archivo, se manda una señal para desbloquear la hebra
    //y que calcule del buffer sin necesidad que esté lleno.
    free(aux);
    fclose(archivo);
    for(int i = 0; i < cantidadDiscos;i++){
        printf("Disco %d: %d\n",i+1,tablas[i]->cantidadFilas);
    }
    
    return tablas;
}


int main(){
    cantidadDiscos = 4;
    anchoDisco = 50;

    //imprimirTablas(leerArchivo("prueba2.csv"));
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

