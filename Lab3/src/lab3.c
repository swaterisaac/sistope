#define MAIN_FILE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include "../incl/Estructuras.h"
#include "../incl/Monitor.h"
#include "../incl/EstructuraComun.h"
#include "../incl/Getopt.h"
#include "../incl/shared.h"

//Ya que los archivos csv siempre van a tener una cantidad fija de columnas.
#define COLUMNAS 5

/*
//Variables globales que están en shared.h
int largoMaximo, cantidadDiscos,anchoDisco;
EstructuraComun estructuraComun;
*/
/*
----------------inicializarHebras----------------
Entradas:
Salidas:
Descripción:
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
----------------obtenerLineas----------------
Entradas:
Salidas:
Descripción:
*/
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

/*
----------------terminoLectura----------------
Entradas:
Salidas:
Descripción:
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


/*
----------------escribirArchivo----------------
Entradas:
Salidas:
Descripción:
*/
void escribirArchivo(char* nombreArchivo){
    FILE* archivo = fopen(nombreArchivo,"w");

    for(int i = 0; i < cantidadDiscos;i++){
        fprintf(archivo,"Disco %d:\n",i+1);
        fprintf(archivo,"Media real: %f\n",estructuraComun.discos[i]->mediaReal);
        fprintf(archivo,"Media imaginaria: %f\n",estructuraComun.discos[i]->mediaImaginaria);
        fprintf(archivo,"Potencia: %f\n",estructuraComun.discos[i]->potencia);
        fprintf(archivo,"Ruido total: %f\n",estructuraComun.discos[i]->ruidoTotal);
    }
    fclose(archivo);
    return;
}


/*
----------------leerArchivo----------------
Entradas:
Salidas:
Descripción:
*/
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

        /*
        Se agrega al monitor correspondiente con el index y la funcion agregar los valores de esa Visibilidad (fila).
        Hay que asegurarse de que se hace un lock antes de agregar y unlock después de agregar.
        Si la cantidad de elementos del monitor es igual al tamaño maximo del buffer, la hebra se desbloquea para hacer
        los cálculos.
        */
        //printf("Elementos en buffer: %d\n",monitores[index]->buffer->elementosActuales);

        if(monitores[index]->buffer->elementosActuales == largoMaximo){

            //printf("Desbloqueando a la hebra %d...\n",index);

            pthread_cond_signal(&monitores[index]->bufferLleno);
        }
        pthread_mutex_lock(&monitores[index]->mutexProduccion);
        
        monitores[index]->agregar(monitores[index],aux);

        //Mutex unlock
        pthread_mutex_unlock(&monitores[index]->mutexProduccion);
        
        /*
        printf("\n%lf,%lf,%lf,%lf,%lf\n",monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->u,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->v,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->r,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->i,
        monitores[index]->buffer->listaVisibilidad[monitores[index]->buffer->elementosActuales - 1]->w);
        */
        
        free(aux);
    }

    //Cuando termine de leer el archivo, se manda una señal para desbloquear la hebra
    //y que calcule del buffer sin necesidad que esté lleno.
    fclose(archivo);
    terminoLectura(monitores,arregloHebras);
    return;
}



int main(int argc, char *argv[]){
    //Argumentos a recibir por línea de comandos (esto será parte del getopt)
    char* archivoEntrada; 
    char* archivoSalida;
    int flag;

    recibirArgumentos(argc, argv, &archivoEntrada, &archivoSalida, &anchoDisco, &cantidadDiscos, &largoMaximo, &flag);
    printf("El argumento de flag -i es: %s\n", archivoEntrada);
    printf("El argumento de flag -o es: %s\n", archivoSalida);
    printf("El argumento de flag -d es: %d\n", anchoDisco);
    printf("El argumento de flag -n es: %d\n", cantidadDiscos);
    printf("El argumento de flag -s es: %d\n", largoMaximo);

    inicializarEstructura();
    
    /*cantidadDiscos = 4;
    anchoDisco = 100;
    largoMaximo = 100;
    */
    
    leerArchivo(archivoEntrada);
    escribirArchivo(archivoSalida);
    
    if(flag == 1){
        printf("\nMOSTRANDO RESULTADOS\n");
        imprimirVisualizaciones(estructuraComun);
    }
    
    return 0;
}

