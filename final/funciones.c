#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include "Signal.h"
//#include "Shared.h"


void imprimirError(char *argv[]){
	fprintf(stderr, "Uso correcto: %s \n[-i string: Nombre del archivo]\n[-n numero entero: Cantidad de procesos]"
	"\n[-c numero entero: Cantidad de lineas a leer]\n[-p string: Cadena a buscar]\n[-d: Bandera para mostrar resultados en pantalla]\n",
	argv[0]);
	return;
}

int verificarADN(char* string){
	int max = strlen(string);
	for(int i = 0; i < max;i++){
		if(!(string[i] == 'A' || string[i] == 'C' || string[i] == 'G' || string[i] == 'T')){
			return 0;
		}
	}
	return 1;
}

//Entradas: Recibe 
//argc: que consiste en el contador de argumentos ingresados en la linea de comandos. El nombre del programa que se ejecuta se cuenta como un argumento
//argv: arreglo con las entradas ingresadas por linea de comandos
//*nombreArchivo, *numeroProcesos, *cantidadLineas, *cadena y *flag: punteros a variables que se utilizaran para pasar valores por referencia
//Funcionamiento:
//Salida: No posee retorno, ya que es una función void
void recibirArgumentos(int argc, char *argv[], char **nombreArchivo, int *numeroProcesos, int *cantidadLineas, char **cadena, int *flag ){
	int flags, opt;
	char *aux3;
	aux3 = malloc(10*sizeof(char));
	if(argc < 9 || argc > 10){//si se ingresa un numero de argumentos menor a 6, se finaliza la ejecucion del programa
		//Debe ser 6, porque el nombre del programa se considera como un argumento
		printf("Se ingreso un numero incorrecto de argumentos.\n");
		imprimirError(argv);
		exit(EXIT_FAILURE);
	}
	int N = -1;
    int N2 = -1;
	flags = 0;
	//Se inicia un ciclo while hasta que se verifiquen todos los argumentos ingresados como entradas, los cuales se reciben con getopt()
	//int getopt (int argc, char *const *argv, const char *options): Siempre se le debe entregar argc y argv. El tercer argumento consiste en el formato de las entradas, ya sea
	//el caracter correspondiente a la opcion/entrada que se quiere recibir y además se debe indicar si dicha entrada estará acompañada de algún valor
	//Esto se indica con ":". Por lo tanto, "i:" quiere decir que se espera recibir la opcion -i y esta si o si debe estar acompañada de un valor
	//En cambio, la opcion "-d" no necesita estar acompañada de un valor
	while((opt = getopt(argc, argv, "di:n:c:p:")) != -1 ){ 
	   //opt recibe el argumento leido (se hace de forma secuencial) y se ingresa a un switch
	   //En caso de que opt sea -1, se dejaran de buscar entradas y por lo tanto se podrá salir del while
	   switch(opt) {
	   case 'd'://se busca la entrada -d, en caso de ser encontrado se setea el valor flags = 1, no se considera lo que se ingrese despues del flag -d
		   flags = 1;
		   break;
	   case 'i': //se busca la entrada -i
			*nombreArchivo= optarg;
		   if(optarg!=0 && strcmp(*nombreArchivo,"")==0){//si no se ingresa un argumento junto a -i o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("Error de formato\n");
				imprimirError(argv);
				exit(EXIT_FAILURE);
		   }
		   break;
       case 'n': //se busca la entrada -n
		   N = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -n a entero
		   if(optarg!=0 && N<=0){//si no se ingresa un argumento junto a -h o si no se logra parsear el argumento ingresado, se considera como invalido
				imprimirError(argv);
				exit(EXIT_FAILURE);
			   }
		   break;
       case 'c': //se busca la entrada -c
		   N2 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -h a entero
		   if(optarg!=0 && N2 <=0){//si no se ingresa un argumento junto a -c o si no se logra parsear el argumento ingresado, se considera como invalido
				imprimirError(argv);
				exit(EXIT_FAILURE);
			   }
		   break;
       case 'p': //se busca la entrada -P
			*cadena=optarg;
			
			if(optarg != 0 && (strlen(*cadena) != 4)){//si no se ingresa un argumento junto a -p o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("El largo de la cadena debe ser de 4.\n");
				imprimirError(argv);
				exit(EXIT_FAILURE);
			}
			else if(verificarADN(*cadena) == 0){
				printf("La cadena debe contener solo A,C,G o T (mayúsculas)\n");
				imprimirError(argv);
				exit(EXIT_FAILURE);
			}
		   break;
	   default: /* '?' */
		   imprimirError(argv);
		   exit(EXIT_FAILURE);
	   }
	}

	if(flags==1){//si se encontro un flag -d, se setea la variable global flag = 1, respecto al scope del proceso principal
		(*flag) = 1;
	}
	(*numeroProcesos) = N; //se iguala la variable numeroProcesos a N, para poder acceder al valor en el main
    (*cantidadLineas) = N2; //se iguala la variable cantidadLineas a N2, para poder acceder al valor en el main
	if(N<=0){
		printf("El valor que acompaña a -n debe ser un mayor a 0\n: %d",N);
		imprimirError(argv);
		exit(EXIT_FAILURE);
		}
    if(N2<=0){
		printf("El valor que acompaña a -c debe ser un mayor a 0\n");
		imprimirError(argv);
		exit(EXIT_FAILURE);
		}
}