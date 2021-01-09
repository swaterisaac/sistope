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
#include "../incl/FuncionesArchivo.h"
#include "../incl/shared.h"



/*
//Variables globales que están en shared.h
int largoMaximo, cantidadDiscos,anchoDisco;
EstructuraComun estructuraComun;
*/


int main(int argc, char *argv[]){
    //Argumentos a recibir por línea de comandos (esto será parte del getopt)
    char* archivoEntrada; 
    char* archivoSalida;
    int flag;

    recibirArgumentos(argc, argv, &archivoEntrada, &archivoSalida, &anchoDisco, &cantidadDiscos, &largoMaximo, &flag);
    /*printf("El argumento de flag -i es: %s\n", archivoEntrada);
    printf("El argumento de flag -o es: %s\n", archivoSalida);
    printf("El argumento de flag -d es: %d\n", anchoDisco);
    printf("El argumento de flag -n es: %d\n", cantidadDiscos);
    printf("El argumento de flag -s es: %d\n", largoMaximo);*/

    inicializarEstructura();
    
    /*cantidadDiscos = 4;
    anchoDisco = 100;
    largoMaximo = 100;
    */
    
    leerArchivo(archivoEntrada);
    escribirArchivo(archivoSalida);
    
    if(flag == 1){
        //printf("\nMOSTRANDO RESULTADOS\n");
        imprimirVisualizaciones(estructuraComun);
    }
    
    return 0;
}

