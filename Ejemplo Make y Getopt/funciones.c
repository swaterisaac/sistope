#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "Signal.h"
#include "Shared.h"

void printGlobal(){
	printf("Valor de variable global: %d\n", valor);

	}
//Entradas: Recibe 
//argc: que consiste en el contador de argumentos ingresados en la linea de comandos. El nombre del programa que se ejecuta se cuenta como un argumento
//argv: arreglo con las entradas ingresadas por linea de comandos
//*n y *flag: punteros a variables que se utilizaran para pasar valores por referencia, ambos son enteros
//Funcionamiento:
//Salida: No posee retorno, ya que es una función void
void recibirArgumentos(int argc, char *argv[], int *n, int *flag){
	int flags, opt;
	char *aux3;
	aux3 = malloc(10*sizeof(char));
	if(argc <3){//si se ingresa un numero de argumentos menor a 3, se finaliza la ejecucion del programa
		//Debe ser 3, porque el nombre del programa se considera como un argumento, siendo -h y el valor que acompañe a -h los dos argumentos faltantes. No se considera -m (que seria el cuarto argumento) porque es un flag que puede ser especificado por el usuario o no
		printf("Se ingreso un numero incorrecto de argumentos\n");
		fprintf(stderr, "Uso correcto: %s [-h numero entero] [-m]\n",
				   argv[0]);
		   exit(EXIT_FAILURE);
		}
	int N = -1;
	flags = 0;
	//Se inicia un ciclo while hasta que se verifiquen todos los argumentos ingresados como entradas, los cuales se reciben con getopt()
	//int getopt (int argc, char *const *argv, const char *options): Siempre se le debe entregar argc y argv. El tercer argumento consiste en el formato de las entradas, ya sea
	//el caracter correspondiente a la opcion/entrada que se quiere recibir y además se debe indicar si dicha entrada estará acompañada de algún valor
	//Esto se indica con ":". Por lo tanto, "h:" quiere decir que se espera recibir la opcion -h y esta si o si debe estar acompañada de un valor
	//En cambio, la opcion "-m" no necesita estar acompañada de un valor
	while((opt = getopt(argc, argv, "mh:")) != -1) { 
	   //opt recibe el argumento leido (se hace de forma secuencial) y se ingresa a un switch
	   //En caso de que opt sea -1, se dejaran de buscar entradas y por lo tanto se podrá salir del while
	   switch(opt) {
	   case 'm'://se busca la entrada -m, en caso de ser encontrado se setea el valor flags = 1, no se considera lo que se ingrese despues del flag -m
		   flags = 1;
		   break;
	   case 'h': //se busca la entrada -h
		   N = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -h a entero
		   if(optarg!=0 && N==0){//si no se ingresa un argumento junto a -h o si no se logra parsear el argumento ingresado, se considera como invalido
				fprintf(stderr, "Uso correcto: %s [-h numero entero] [-m]\n", argv[0]); 
				exit(EXIT_FAILURE);
			   }
		   break;
	   default: /* '?' */
		   fprintf(stderr, "Uso correcto: %s [-h numero entero] [-m]\n",
				   argv[0]);
		   exit(EXIT_FAILURE);
	   }
	}

	if(flags==1){//si se encontro un flag -m, se setea la variable global flag = 1, respecto al scope del proceso principal
		(*flag) = 1;
		}
	(*n) = N; //se iguala la variable n a N, para poder acceder al valor en el main
	if(N<=0){
		printf("El valor que acompaña a -h debe ser mayor a 0\n");
		fprintf(stderr, "Uso correcto: %s [-h nchild] [-m]\n", argv[0]); //si el valor que acompaña a -h es negativo, se retorna un error
		exit(EXIT_FAILURE);
		}


}
