#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t

#define LECTURA 0
#define ESCRITURA 1
struct cosas{
	char nombreArchivo[40];
	int cantidadLineasLeer;
	int cursorY;
	char cadenaComparar[5];
	int flag;
}typedef cosas;

void imprimirStruct(cosas impresion){
	printf("%s\n",impresion.nombreArchivo);
	printf("%d\n",impresion.cantidadLineasLeer);
	printf("%d\n",impresion.cursorY);
	printf("%s\n",impresion.cadenaComparar);
	printf("%d\n",impresion.flag);
	return;
}

int main(){
	
	cosas paqueteRel;
	
	read(STDIN_FILENO, &paqueteRel, sizeof(cosas));
	imprimirStruct(paqueteRel);
	return 0;
}