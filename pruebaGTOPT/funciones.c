#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include "Signal.h"
//#include "Shared.h"

//Entradas: Recibe 
//argc: que consiste en el contador de argumentos ingresados en la linea de comandos. El nombre del programa que se ejecuta se cuenta como un argumento
//argv: arreglo con las entradas ingresadas por linea de comandos
//*nombreArchivo, *numeroProcesos, *cantidadLineas, *cadena y *flag: punteros a variables que se utilizaran para pasar valores por referencia
//Funcionamiento:
//Salida: No posee retorno, ya que es una función void
void recibirArgumentos(int argc, char *argv[], char *nombreArchivo, int *numeroProcesos, int *cantidadLineas, char *cadena, int *flag ){
	int flags, opt;
	char *aux3;
	aux3 = malloc(10*sizeof(char));
	if(argc <6){//si se ingresa un numero de argumentos menor a 6, se finaliza la ejecucion del programa
		//Debe ser 6, porque el nombre del programa se considera como un argumento
		printf("Se ingreso un numero incorrecto de argumentos\n");
		fprintf(stderr, "Uso correcto: %s [-i string] [-n numero entero] [-c numero entero] [-p string] [-d]\n",
				   argv[0]);
		   exit(EXIT_FAILURE);
	}
	int N = -1;
    int N2 = -1;
	//char *nombre;
	//char *cad;


	flags = 0;
	//Se inicia un ciclo while hasta que se verifiquen todos los argumentos ingresados como entradas, los cuales se reciben con getopt()
	//int getopt (int argc, char *const *argv, const char *options): Siempre se le debe entregar argc y argv. El tercer argumento consiste en el formato de las entradas, ya sea
	//el caracter correspondiente a la opcion/entrada que se quiere recibir y además se debe indicar si dicha entrada estará acompañada de algún valor
	//Esto se indica con ":". Por lo tanto, "i:" quiere decir que se espera recibir la opcion -i y esta si o si debe estar acompañada de un valor
	//En cambio, la opcion "-d" no necesita estar acompañada de un valor
	while((opt = getopt(argc, argv, "di:n:c:p:")) != -1) { 
	   //opt recibe el argumento leido (se hace de forma secuencial) y se ingresa a un switch
	   //En caso de que opt sea -1, se dejaran de buscar entradas y por lo tanto se podrá salir del while
	   switch(opt) {
	   case 'd'://se busca la entrada -d, en caso de ser encontrado se setea el valor flags = 1, no se considera lo que se ingrese despues del flag -d
		   flags = 1;
		   break;
	   case 'i': //se busca la entrada -i
			nombreArchivo= optarg;
			printf("parametro i: %s\n",nombreArchivo);
		   if(optarg!=0 && strcmp(nombreArchivo,"")==0){//si no se ingresa un argumento junto a -i o si no se logra parsear el argumento ingresado, se considera como invalido
				fprintf(stderr, "Uso correcto: %s [-i string] [-n numero entero] [-c numero entero] [-p string] [-d]\n",
				   argv[0]);
				exit(EXIT_FAILURE);
		   }
		   break;
       case 'n': //se busca la entrada -n
		   N = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -n a entero
		   if(optarg!=0 && N<=0){//si no se ingresa un argumento junto a -h o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("caso n\n");
				fprintf(stderr, "Uso correcto: %s [-i string] [-n numero entero] [-c numero entero] [-p string] [-d]\n",
				   argv[0]);
				exit(EXIT_FAILURE);
			   }
		   break;
       case 'c': //se busca la entrada -c
		   N2 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -h a entero
		   if(optarg!=0 && N2 <=0){//si no se ingresa un argumento junto a -c o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("caso c\n");
				fprintf(stderr, "Uso correcto: %s [-i string] [-n numero entero] [-c numero entero] [-p string] [-d]\n",
				   argv[0]);
				exit(EXIT_FAILURE);
			   }
		   break;
       case 'p': //se busca la entrada -h
			cadena=optarg;
			printf("parametro p (cadena): %s\n",cadena);
		   if(optarg!=0 && strcmp(cadena,"")==0){//si no se ingresa un argumento junto a -p o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("caso p\n");

				fprintf(stderr, "Uso correcto: %s [-i string] [-n numero entero] [-c numero entero] [-p string] [-d]\n",
				   argv[0]);
				exit(EXIT_FAILURE);
			   }
		   break;
	   default: /* '?' */
		   printf("otro caso\n");
		   fprintf(stderr, "Uso correcto: %s [-i string] [-n numero entero] [-c numero entero] [-p string] [-d]\n",
				   argv[0]);
		   exit(EXIT_FAILURE);
	   }
	}

	if(flags==1){//si se encontro un flag -d, se setea la variable global flag = 1, respecto al scope del proceso principal
		(*flag) = 1;
	}
	(*numeroProcesos) = N; //se iguala la variable numeroProcesos a N, para poder acceder al valor en el main
    (*cantidadLineas) = N2; //se iguala la variable cantidadLineas a N2, para poder acceder al valor en el main
	printf("valor del nombre: %s\n",nombreArchivo);
	printf("valor del cad: %s\n",cadena);
	//nombreArchivo=nombre;
	//cadena=cad;
	if(N<=0){
		printf("El valor que acompaña a -n debe ser un mayor a 0\n");
		//fprintf(stderr, "Uso correcto: %s [-h nchild] [-m]\n", argv[0]); //si el valor que acompaña a -n es negativo o cero, se retorna un error
		exit(EXIT_FAILURE);
		}
    if(N2<=0){
		printf("El valor que acompaña a -c debe ser un mayor a 0\n");
		//fprintf(stderr, "Uso correcto: %s [-h nchild] [-m]\n", argv[0]); //si el valor que acompaña a -c es negativo o cero, se retorna un error
		exit(EXIT_FAILURE);
		}

}
