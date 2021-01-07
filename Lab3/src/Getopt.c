#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//descripción: imprime un mensaje de error por pantalla.
//entrada: arreglo con las entradas ingresadas.
//salida: imprime por pantalla.
void imprimirError(char *argv[]){
	fprintf(stderr, "Uso correcto: %s \n[-i string: Nombre del archivo CSV]\n[-o string: Nombre del archivo de salida]"
	"\n[-d número entero: Ancho de cada de disco]\n[-n número entero: Cantidad de discos]\n[-s número entero: Tamaño del buffer de cada monitor]\n"
	"[-b si se desean imprimir los resultados.]\n",
	argv[0]);
	return;
}

// ######################################### GETOPT #########################################
//descripción: permite ingresar argumentos desde la línea de comandos.
//Entradas: Recibe 
//argc: que consiste en el contador de argumentos ingresados en la linea de comandos. El nombre del programa que se ejecuta se cuenta como un argumento
//argv: arreglo con las entradas ingresadas por linea de comandos
//**nombreArchivo, **salida, *cantidadNiveles, *cantidadBins: punteros a variables que se utilizarán para pasar valores por referencia
//Salida: No posee retorno, ya que es una función void.
void recibirArgumentos(int argc, char *argv[], char **nombreArchivo, char **salida, int *ancho, int *cantDiscos, int *tamBuffer, int *flag){
    int opt;
	char *aux3;
	aux3 = malloc(10*sizeof(char));
	if(argc < 11 || argc > 12){//si se ingresa un numero de argumentos menor a 11 o mayor a 12, se finaliza la ejecución del programa
		printf("Se ingresó un número incorrecto de argumentos.\n");
		imprimirError(argv);
		exit(EXIT_FAILURE);
	}
	int N = -1;
    int N2 = -1;
    int N3 = -1;
    int flags = 0;
	//Se inicia un ciclo while hasta que se verifiquen todos los argumentos ingresados como entradas, los cuales se reciben con getopt()
	//int getopt (int argc, char *const *argv, const char *options): Siempre se le debe entregar argc y argv. El tercer argumento consiste en el formato de las entradas, ya sea
	//el caracter correspondiente a la opcion/entrada que se quiere recibir y además se debe indicar si dicha entrada estará acompañada de algún valor
	//Esto se indica con ":". Por lo tanto, "i:" quiere decir que se espera recibir la opcion -i y esta si o si debe estar acompañada de un valor
	while((opt = getopt(argc, argv, "bi:o:d:n:s:")) != -1 ){ 
	    //opt recibe el argumento leido (se hace de forma secuencial) y se ingresa a un switch
	    //En caso de que opt sea -1, se dejaran de buscar entradas y por lo tanto se podrá salir del while
	    switch(opt) {
        case 'b'://se busca la entrada -b, en caso de ser encontrado se setea el valor flags = 1, no se considera lo que se ingrese despues del flag -b
		    flags = 1;
		    break;
	    case 'i': //se busca la entrada -i
			*nombreArchivo= optarg;
            FILE * fp = fopen(*nombreArchivo, "rb");
		    if(optarg!=0 && strcmp(*nombreArchivo,"")==0){//si no se ingresa un argumento junto a -i o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("Error de formato.\n");
				imprimirError(argv);
				exit(EXIT_FAILURE);
		    }
            else if(fp==NULL){
               printf("No existe tal archivo.\n");
               imprimirError(argv);
			    exit(EXIT_FAILURE);
            }
		    break;

        case 'o': //se busca la entrada -o
			*salida= optarg;
		    if(optarg!=0 && strcmp(*salida,"")==0){//si no se ingresa un argumento junto a -o o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("Error de formato\n");
				imprimirError(argv);
				exit(EXIT_FAILURE);
		    }
		    break;
        case 'd': //se busca la entrada -d
		    N = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -d a entero
            if(optarg[0] < '0' || optarg[0] > '9'){
                printf("Ingrese un número entero válido para el parámetro d\n");
                imprimirError(argv);
                exit(EXIT_FAILURE);
            }
		    else if(optarg!=0 && N<0){//si no se ingresa un argumento junto a -n o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("Procure ingresar un número positivo menor a 8 para el parámetro d.\n");
                imprimirError(argv);
				exit(EXIT_FAILURE);
			}
		    break;
        case 'n': //se busca la entrada -n
		    N2 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -n a entero
            if(optarg[0] < '0' || optarg[0] > '9'){
                printf("Ingrese un número entero válido para el parámetro n\n");
                imprimirError(argv);
                exit(EXIT_FAILURE);
            }
		    else if(optarg!=0 && N2 <=0 ){//si no se ingresa un argumento junto a -n o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("La cantidad de discos debe ser un valor mayor a 0.\n");
                imprimirError(argv);
				exit(EXIT_FAILURE);
			}
		    break;
        case 's': //se busca la entrada -s
		    N3 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -s a entero
            if(optarg[0] < '0' || optarg[0] > '9'){
                printf("Ingrese un número entero válido para el parámetro s\n");
                imprimirError(argv);
                exit(EXIT_FAILURE);
            }
		    else if(optarg!=0 && N3 <=0 ){//si no se ingresa un argumento junto a -s o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("El tamaño del buffer debe ser un valor mayor a 0.\n");
                imprimirError(argv);
				exit(EXIT_FAILURE);
			}
		    break;
	    default: /* '?' */
		   imprimirError(argv);
		   exit(EXIT_FAILURE);
	    }
	}

    if(flags==1){//si se encontro un flag -b, se setea la variable global flag = 1, respecto al scope del proceso principal
		(*flag) = 1;
	}
	(*ancho) = N; //se iguala la variable ancho a N, para poder acceder al valor en el main
    (*cantDiscos) = N2; //se iguala la variable cantDiscos a N2, para poder acceder al valor en el main
    (*tamBuffer) = N3; //se iguala la variable tamBuffer a N3, para poder acceder al valor en el main

	if(N<=0){
		printf("El valor que acompaña a -d debe ser un mayor a 0\n: %d",N);
		imprimirError(argv);
		exit(EXIT_FAILURE);
	}
    if(N2<=0){
		printf("El valor que acompaña a -n debe ser un mayor a 0\n");
		imprimirError(argv);
		exit(EXIT_FAILURE);
	}
    if(N3<=0){
		printf("El valor que acompaña a -s debe ser un mayor a 0\n");
		imprimirError(argv);
		exit(EXIT_FAILURE);
	}
}