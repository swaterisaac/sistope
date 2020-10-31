#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#include "Signal.h"
#include "funciones.h"

int main(int argc, char *argv[]){

	int numeroProcesos = 0, cantidadLineas = 0, flag = 0;
    char *nombreArchivo= NULL, *cadena= NULL;
	recibirArgumentos(argc, argv, nombreArchivo, &numeroProcesos, &cantidadLineas, cadena, &flag );
	if(flag==1){
		printf("Se utilizo flag -d\n");
		}
	printf("El argumento de flag -i es: %s\n", nombreArchivo);
    printf("El argumento de flag -n es: %d\n", numeroProcesos);
    printf("El argumento de flag -c es: %d\n", cantidadLineas);
    printf("El argumento de flag -p es: %s\n", cadena);
}