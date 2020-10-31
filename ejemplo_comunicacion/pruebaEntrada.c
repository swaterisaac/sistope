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

int main(){
	char* argumentos[2] = {"./Rl", NULL};
	int xd = 15;
	cosas paquete1;
	strcpy(paquete1.nombreArchivo,"ola.txt");
	paquete1.cantidadLineasLeer = 3;
	paquete1.cursorY = 0;
	strcpy(paquete1.cadenaComparar, "XDDD");
	paquete1.flag = 0;


	int *pipes = (int*)malloc(sizeof(int)*2);
	pipe(pipes);

	int pid = fork();
	printf("%d\n",pid);

	int status;
	if(pid > 0){//Padre
		close(pipes[LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor
		

		write(pipes[ESCRITURA],&paquete1 , sizeof(cosas));
		
		waitpid(pid, &status,0);
	}
	else{ //Hijo
		printf("Llegue aqui\n");
		close(pipes[ESCRITURA]);

		dup2(pipes[LECTURA], STDIN_FILENO);

		execv(argumentos[0], argumentos);
	}	

	return 0;
}
