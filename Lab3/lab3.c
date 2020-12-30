#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <stdint.h>
//#include <unistd.h>
#include <string.h>
#include <math.h>

//Ya que los archivos csv siempre van a tener una cantidad fija de columnas.
#define COLUMNAS 5
struct{
    double u;
    double v;
    double r;
    double i;
    double w;
}typedef Visibilidad;

struct{
    Visibilidad* listaVisibilidad;
    int cantidadFilas;
}typedef Tabla;

struct{
    double mediaReal;
    double mediaImaginaria;
    double potencia;
    double ruidoTotal;
}typedef Disco;

struct{
    Disco** discos;
}typedef EstructuraComun;

//largoMaximo será una variable global
struct{
    int largoBuffer;
    Visibilidad* listaVisibilidad;
}typedef Buffer;

//Variables globales
int largoMaximo, cantidadDiscos,anchoDisco;
EstructuraComun estructuraComun;

//Debug funciones:

/////////////////////////////////////////////////////

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
    FILE* archivo = fopen(nombreArchivo,"r");
    

    if(archivo == NULL){
        printf("No existe tal archivo.\n");
    }

    //Se obtienen las lineas del archivo
    int lineas = obtenerLineas(archivo);
    int index = 0;
    int subIndex = 0;
    Visibilidad* aux = (Visibilidad*)malloc(sizeof(Visibilidad)*lineas);
    //Se vuelve a abrir el archivo
    archivo = fopen(nombreArchivo,"r");

    for (int i=0; i<lineas;i++){
        fscanf(archivo,"%lf,%lf,%lf,%lf,%lf,",&aux[i].u,&aux[i].v,&aux[i].r,&aux[i].i,&aux[i].w);
        index = calcularDisco(aux[i]);
        printf("INDEX: %d\n",index);
        
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
    free(aux);
    fclose(archivo);
    for(int i = 0; i < cantidadDiscos;i++){
        printf("Disco %d: %d\n",i+1,tablas[i]->cantidadFilas);
    }
    
    return tablas;
}


/*void generarHebras_Monitores(){

    //Podríamos tener un arreglo con las hebras y monitores y así poder iterar con un ciclo
    pthread_t hilo[discos];
    while(i < numeroHebras){
        pthread_create(&hilo[i], NULL, alguna funcion, algo);
        i++;
    }
}
*/

int main(){
    cantidadDiscos = 4;
    anchoDisco = 50;

    //imprimirTablas(leerArchivo("prueba2.csv"));
    leerArchivo("prueba2.csv");

    //Argumentos a recibir por línea de comandos (esto será parte del getopt)
    int radios; //número de radios
    int anchoIntervalo; //ancho de cada intervalo
    char *archivoEntrada; 
    char *archivoSalida;
    int tamBuffer; //tamaño del buffer de cada monitor

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

