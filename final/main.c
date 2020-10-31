#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#include "Signal.h"
#include "funciones.h"
#include "coordinador.h"
#include "comparador.h"

int main(int argc, char *argv[]){

	int numeroProcesos = 0, cantidadLineas = 0, flag = 0;
    char *nombreArchivo, *cadena;
	recibirArgumentos(argc, argv, &nombreArchivo, &numeroProcesos, &cantidadLineas, &cadena, &flag );
	if(flag==1){
		printf("Se utilizo flag -d\n");
		}
    printf("El argumento de flag -i es: %s\n", nombreArchivo);
    printf("El argumento de flag -n es: %d\n", numeroProcesos);
    printf("El argumento de flag -c es: %d\n", cantidadLineas);
    printf("El argumento de flag -p es: %s\n", cadena);
    int* arregloPID = (int*)malloc(sizeof(int)*numeroProcesos);
    printf("llegue");
	arregloPID = crearNProcesos(numeroProcesos,cantidadLineas,flag,nombreArchivo,cadena);
    printf("llegue");
	juntarArchivo(arregloPID,cadena,numeroProcesos);
    return 0;
}