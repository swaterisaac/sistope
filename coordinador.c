#include <unistd.h> //Para utilizar fork(), pipes(), entre otros
#include <stdio.h> //Funciones de entrada y salida como printf
#include <stdlib.h> //Asignación de memoria, atoi, etc.
#include <string.h> 
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t

#define LECTURA 0
#define ESCRITURA 1

//Empaquetado de la información que se manda a través del PIPE
struct empaquetado{
	char nombreArchivo[40];
	int cantidadLineasLeer;
	int cursorY;
	char cadenaComparar[5];
	int flag;
}typedef empaquetado;


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

			//imprimirPaquete(paquete1);

			//Se lo comunica a través de pipe

			close(pipesGeneral[LECTURA]);
			//printf("XD %d\n",i);
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

void juntarArchivo(int* arregloPID,char* cadenaBuscar,int cantidadProcesos){
	char nombreArchivoSalida[40];
	strcpy(nombreArchivoSalida,"rc_");
	strcat(nombreArchivoSalida,cadenaBuscar);
	strcat(nombreArchivoSalida,".txt");
	FILE* arch = fopen(nombreArchivoSalida,"w");
	char auxiliar = '0';
	for(int i = 0;i < cantidadProcesos;i++){
		FILE* archAux = fopen(codificarNombreLeer(arregloPID[i],cadenaBuscar),"r");

		while(auxiliar != EOF){
			auxiliar = fgetc(archAux);
			fprintf(arch,"%c",auxiliar);
		}
		printf("-------->%c<-------------\n",auxiliar);
		auxiliar = '0';
		fgetc(archAux);
		//fflush(archAux);
		fclose(archAux);

	}
	fclose(arch);

	return;
}
//int argc, char* argv[]
int main(){
	//Atributos a recibir por el getOpt:
	char nombreArchivo[40];
	char cadenaBuscar[5];
	int numeroDeProcesos;
	int cantLineas;
	int flag;

	numeroDeProcesos = 5;
	cantLineas = 20;
	flag = 1;
	strcpy(nombreArchivo,"ejemploGenerado.txt");
	strcpy(cadenaBuscar,"AAAA");

	int* arregloPID = (int*)malloc(sizeof(int)*numeroDeProcesos);

	arregloPID = crearNProcesos(numeroDeProcesos,cantLineas,flag,nombreArchivo,cadenaBuscar);
	juntarArchivo(arregloPID,cadenaBuscar,numeroDeProcesos);
	return 0;
}