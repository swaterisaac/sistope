//Importación de bibliotecas
#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include "Estructuras.h"
#include "funciones.h"

#define LECTURA 0
#define ESCRITURA 1



void imprimirPaquete(empaquetado paquete){
	printf("Nombre del archivo: %s\n",paquete.nombreArchivo);
	printf("Cantidad de lineas a leer: %d\n",paquete.cantidadLineasLeer);
	printf("Linea de donde empieza: %d\n",paquete.cursorY);
	printf("Cadena a comparar: %s\n",paquete.cadenaComparar);
	printf("Bandera: %d\n",paquete.flag);
	return;
}

//Todos los procesos van a ser equitativos excepto el último, quien se va a llevar ese trabajo más lo que sobre por hacer.
//En el caso de que sea perfectamente divisible, todos los procesos harán el mismo trabajo. (ya que cantLineas % numeroDeProcesos = 0)
//descripción: permite separar el trabajo que se le asigna a cada proceso creado.
//entradas: la cantidad de procesos a crear, y la cantidad de líneas que posee el archivo.
//salida: un arreglo que contiene el trabajo que se le asiganará a cada proceso.
int* separarTrabajo(int numeroDeProcesos, int cantLineas){
	int* resultado = (int*)malloc(sizeof(int)*2);
	resultado[0] = cantLineas / numeroDeProcesos;
	//si la división no es entera, aquí queda almacenado la cantidad de líneas que debe trabajar el último proceso
	resultado[1] = (cantLineas / numeroDeProcesos) + (cantLineas % numeroDeProcesos);
	return resultado;
}


//descripción: crea los n procesos para que cada uno realice el trabajo que le corresponde.
//entradas: la cantidad de procesos a crear, la cantidad de líneas totales del archivo de entrada,
// el nombre del archivo de entrada,y la cadena a buscar.
//salida: entrega un arreglo de enteros, con los PID de los procesos.
int* crearNProcesos(int numeroDeProcesos,int cantLineas,int flag,char* nombreArchivo,char* cadenaBuscar){
	int* arregloPID = (int*)malloc(sizeof(int)* numeroDeProcesos);
	int* trabajoSeparado = separarTrabajo(numeroDeProcesos,cantLineas);
	//Si el numero de procesos es mayor a la cantidad de lineas
	if(trabajoSeparado[0] == 0){
		trabajoSeparado[0] = 1;
		trabajoSeparado[1] = 1;
		numeroDeProcesos = cantLineas;
		printf("El numero de procesos es mayor a la cantidad de lineas, así que se bajan a %d cantidad de procesos.\n",cantLineas);
	}

	int pidRel = 1;
	int posInicio = 0;
	int bkPoint = 0;
	int status;
	empaquetado paquete1;

	char* argumentos[2] = {"./Comp", NULL};

	
	int *pipesGeneral = (int*)malloc(sizeof(int)*2);

	for(int i = 0; i < numeroDeProcesos && bkPoint == 0; i++){
		//Creamos el proceso hijo
		//int *pipesGeneral = (int*)malloc(sizeof(int)*2);
		pipe(pipesGeneral);
		pidRel = fork();
		if(pidRel > 0){ //Proceso padre
			//Armamos el paquete para el hijo		

			strcpy(paquete1.nombreArchivo,nombreArchivo);
			strcpy(paquete1.cadenaComparar,cadenaBuscar);

			if(i == numeroDeProcesos - 1){
				paquete1.cantidadLineasLeer = trabajoSeparado[1];	
			}
			else{
				paquete1.cantidadLineasLeer = trabajoSeparado[0];	
			}
			paquete1.cursorY = posInicio;
			paquete1.flag = flag;


			//Se lo comunica a través de pipe

			close(pipesGeneral[LECTURA]);
			write(pipesGeneral[ESCRITURA], &paquete1, sizeof(empaquetado));
			

			//El próximo hijo va a partir de más adelante, entonces le aumentamos la posicion de inicio.
			posInicio += trabajoSeparado[0];
			arregloPID[i] = pidRel;
			waitpid(pidRel, &status,0);

		}
		else if(pidRel == 0){ //Proceso hijo
			close(pipesGeneral[ESCRITURA]);
			dup2(pipesGeneral[LECTURA], STDIN_FILENO);
			bkPoint = 1; //Esto es redundante si se hace el exit, pero es un break del for para el proceso hijo.
			execv(argumentos[0], argumentos);
			exit(1);
		}
		else{ //Error
			printf("Ha habido un error con la creación de procesos hijos. Interrumpiendo...");
			exit(1);
		}
	}
	//printf("termina funcion crear proceso");
	return arregloPID;
}


//descripción: permite codificar el nombre de los archivos de salida de cada proceso, según lo solicitado por enunciado.
//entrada: el identificador del proceso, y la cadena a buscar.
//salida: un string que representa el nombre del archivo de salida de un determinado proceso.
char* codificarNombreLeer(int PID,char* cadenaBuscar){
	char nombreArchivo[40];
	char aux[6];
	sprintf(aux,"%d",PID);

	strcpy(nombreArchivo,"rp_");
	strcat(nombreArchivo,cadenaBuscar);
	strcat(nombreArchivo,"_");
	strcat(nombreArchivo,aux);
	strcat(nombreArchivo,".txt");

	char* nombreFinal = (char*)malloc(sizeof(char)* 40);
	strcpy(nombreFinal,nombreArchivo);
	return nombreFinal;
}

//descripción: permite unir todos los archivos generados por cada proceso.
//entradas: el arreglo que contiene los PID de los procesos, la cadena a buscar, y la cantidad de procesos.
//salida: como es void, sólo se escribe en el archivo.
void juntarArchivo(int* arregloPID,char* cadenaBuscar,int cantidadProcesos){
	char nombreArchivoSalida[40];
	strcpy(nombreArchivoSalida,"rc_");
	strcat(nombreArchivoSalida,cadenaBuscar);
	strcat(nombreArchivoSalida,".txt");
	FILE* arch = fopen(nombreArchivoSalida,"w");
	char auxiliar = '0';
	for(int i = 0;i < cantidadProcesos;i++){
		FILE* archAux = fopen(codificarNombreLeer(arregloPID[i],cadenaBuscar),"r");
		auxiliar = fgetc(archAux);
		while(auxiliar != EOF){
			fprintf(arch,"%c",auxiliar);
			auxiliar = fgetc(archAux);
		}
		fclose(archAux);

	}
	fclose(arch);

	return;
}

/*
Descripción: Función que detiene la ejecución del programa en el caso que el nombre del archivo o la cantidad de lineas ingresadas
sea incorrecto.
Entradas: String indicando el nombre del archivo, entero indicando la cantidad de lineas a leer.
Salidas: void
*/

void validarArchivo(char* nombreArchivo, int cantidadLineas){
	FILE * arch = fopen(nombreArchivo,"r");
    if(arch == NULL){
    	printf("No existe el archivo %s en esta carpeta.\n",nombreArchivo);
    	exit(1);
    }
    int cont = 0;
    char relativo[100];
    while(!feof(arch)){
    	fgets(relativo,100,arch);
    	cont++;
    }
    if(cantidadLineas > cont){
    	printf("Has ingresado una cantidad de lineas mayor a las que tiene el archivo.\n");
    	exit(1);
    }
    fclose(arch);
    return;
}

//int argc, char* argv[]
int main(int argc, char *argv[]){
	int numeroProcesos = 0, cantidadLineas = 0, flag = 0;
    char *nombreArchivo, *cadena;
    //Se reciben atributos por el getOPT
	recibirArgumentos(argc, argv, &nombreArchivo, &numeroProcesos, &cantidadLineas, &cadena, &flag );

	if(flag == 1){
		printf("Se utilizo flag -d\n");
	}

    printf("El argumento de flag -i es: %s\n", nombreArchivo);
    printf("El argumento de flag -n es: %d\n", numeroProcesos);
    printf("El argumento de flag -c es: %d\n", cantidadLineas);
    printf("El argumento de flag -p es: %s\n", cadena);

    validarArchivo(nombreArchivo,cantidadLineas);

    int* arregloPID = (int*)malloc(sizeof(int)*numeroProcesos);
	arregloPID = crearNProcesos(numeroProcesos,cantidadLineas,flag,nombreArchivo,cadena);

	juntarArchivo(arregloPID,cadena,numeroProcesos);
    return 0;
}