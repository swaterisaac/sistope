#ifndef Estructuras_h_
#define Estructuras_h_

//Empaquetado de la información que se manda a través del PIPE
struct empaquetado{
	char nombreArchivo[40];
	int cantidadLineasLeer;
	int cursorY;
	char cadenaComparar[5];
	int flag;
}typedef empaquetado;

/*Estructura que permite almacenar el contenido del archivo (completo o una porción del archivo), 
por medio de una matriz, la cantidad 
de lineas que posee la porción de archivo y el largo de las cadenas*/
struct matriz{
	int cantLineas;
	int largoCadena;
	char** matrizC;
}typedef matriz;

#endif // Estructuras_h_