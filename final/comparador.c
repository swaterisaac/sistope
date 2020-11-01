//Importación de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include "Estructuras.h"


//descripción: permite almacenar los datos de una porción de archivo, en un struct matriz.
//entradas: el nombre del archivo a leer, la posición del cursor en coordenadas x e y, la cantidad de líneas que se desean leer.
//salida: un struct matriz que representa el contenido de una porción de archivo.
matriz leerAtributos(char* nombreArchivo,int cursorX,int cursorY,int cantLineas){
	int i = 0;
	int cont  = 0;

	int largoCadena = 60;
	//int cantLineas = 30;

	FILE* archivo = fopen(nombreArchivo,"r");
	//Si el archivo no existe:
	if(archivo == NULL){
		printf("No existe tal archivo.\n");
		exit(1);
	}
	if(cursorX > largoCadena){
		printf("Cursor incorrecto.\n");
		exit(1);
	}

	char aux;
	
	//fgetc(archivo);
	//fgetc(archivo); //Nos saltamos el primer salto de linea
	char relativo[70];
	char** matrizM = (char**)malloc(sizeof(char*)*30);

	/*
	#ifdef __linux__
	fgets(relativo,70,archivo);
	#endif
	*/

	for(int cont = 0; cont < cursorY; cont++){
		fgets(relativo,(largoCadena+10), archivo);
	}

	while(!feof(archivo) && i != -1){//Mientras no llegue al final del archivo.
		aux = '0';
		cont++;
		fgets(relativo,70,archivo);
		//fgetc(archivo);
		char* linea = (char*)malloc(sizeof(char)*largoCadena);
		//////////////////////////////////////////////////////////////////
		for(int j = 0; j < cursorX;j++){
			linea[j] = aux;
		}
		for(int j = cursorX; aux != '\0' || j < largoCadena;j++){
			aux = relativo[j];
			if(aux != '\n'){
				linea[j] = aux;	
			}
			else{
				aux = '\0';
				linea[j] = '\n';
			}
		}
		cursorX = 0;

		matrizM[i] = linea;
		i++;
		if(i == cantLineas){
			i = -1;
		}
	}
	if(cantLineas > cont){
		cantLineas = cont;
	}
	//Cierre del archivo
	fclose(archivo);
	matriz matrizRetorno;
	matrizRetorno.matrizC = matrizM;
	matrizRetorno.largoCadena = largoCadena;
	matrizRetorno.cantLineas = cantLineas;
	return matrizRetorno;
}

/*
//descripción: permite leer el archivo completo, considerando que el cursor comienza en las coordenada (x,y)=(0,0)
//entrada: el nombre del archivo a leer, y la cantidad de líneas del archivo
//salida: invoca a la función leerAtributos, por lo que retorna un struct matriz con todo el contenido del archivo
matriz leerArchCompleto(char* nombreArchivo, int cantLineas){
	return leerAtributos(nombreArchivo,0,0,cantLineas);
}*/


//descripción: permite verificar si una cadena está contenida en la línea de una porción de archivo.
//entrada: struct matriz que corresponde a la porción de archivo, la línea del texto donde se desea buscar la cadena y la cadena a buscar.
//salida: 1 si la cadena está contenida en la línea, o 0 si no está.
int compararLinea(matriz textoArchivo,int linea,char* cadenaComparar){
	//El largo de la cadena debe ser igual a 4
	if(strlen(cadenaComparar) != 4){
		printf("La cadena a comparar es incorrecta.\n");
		exit(1);
	}

	char* aux = textoArchivo.matrizC[linea];
	for(int i = 0; i < textoArchivo.largoCadena - 3; i++){
		if(aux[i] == cadenaComparar[0] && aux[i+1] == cadenaComparar[1] && aux[i+2] == cadenaComparar[2] && aux[i+3] == cadenaComparar[3]){
			return 1;
		}
	}
	return 0;
}

//Función usada para debugguear
/*matriz separarTexto(matriz textoArchivo,int cursorX,int cursorY,int cantLineas){
	if(textoArchivo.cantLineas - cursorY < cantLineas || textoArchivo.largoCadena < cursorX || textoArchivo.cantLineas < cursorY){
		printf("La cantidad de lineas o el cursor ingresado son incorrectos.\n");
		exit(1);
	}

	char** nuevoTexto = (char**)malloc(sizeof(char*)*cantLineas);
	int cont = 0;

	for(int i = cursorY; i < cantLineas + cursorY;i++){
		char* linea = (char*)malloc(sizeof(char)*textoArchivo.largoCadena);
		for(int j = 0; j < cursorX;j++){
			linea[j] = '0';
		}	
		for(int j = cursorX;j < textoArchivo.largoCadena;j++){
			linea[j] = textoArchivo.matrizC[i][j];
		}
		cursorX = 0;
		nuevoTexto[cont] = linea;
		cont++;
	}

	matriz textoRetorno;
	textoRetorno.matrizC = nuevoTexto;
	textoRetorno.cantLineas = cantLineas;
	textoRetorno.largoCadena = textoArchivo.largoCadena;
	return textoRetorno;

}*/

//descripción: permite transformar un entero a un string "SI" o "NO".
//entrada: un entero, el cual debe ser 1 o 0.
//salida: un string. "SI" si el entero ingresado es 1, o "NO" si el entero ingresado es 0.
char* transformarBool(int boolR){
	if(boolR == 1){
		return "SI";
	}
	else if(boolR == 0){
		return "NO";
	}
	else{
		printf("resultado no es ni 1 ni 0");
		exit(1);
	}
}

//descripción: permite escribir en el archivo las líneas que fueron asignadas a un determinado proceso, 
//además de indicar si se encontró o no la cadena en cada línea.
//entradas: la porción de archivo a escribir, la cadena que fue buscada en esa porción de archivo, 
//y el flag para indicar si se desea imprimir el resultado por pantalla o no.
//salida: como es void, sólo se escribe en el archivo de salida.
void escribirArchivo(matriz lectura,char* cadena,int flag){
	char nombreArchivo[40];
	char PID[10];
	
	sprintf(PID,"%d",(int)getpid());
	//printf("%s\n",PID);

	strcpy(nombreArchivo,"rp_");
	strcat(nombreArchivo, cadena);
	strcat(nombreArchivo,"_");
	strcat(nombreArchivo, PID);
	strcat(nombreArchivo,".txt");

	FILE* arch = fopen(nombreArchivo,"w");
	
	for(int i = 0; i < lectura.cantLineas;i++){
		for(int j = 0; j < lectura.largoCadena;j++){
			if(flag){
				printf("%c",lectura.matrizC[i][j]);
			}
			fprintf(arch,"%c",lectura.matrizC[i][j]);
		}
		char aux[3];
		strcpy(aux,transformarBool(compararLinea(lectura,i,cadena)));
		if(flag){
			printf("  %s\n",aux);	
		}
		fprintf(arch,"  %s\n",aux);

	}
	fclose(arch);
	return;
}


//descripción: muestra por pantalla el contenido de una matriz.
//entrada: un struct matriz.
//salida: void, imprime por pantalla.
void imprimirMatriz(matriz matrizC){
	for(int i = 0; i < matrizC.cantLineas;i++){
		for(int j = 0; j < matrizC.largoCadena;j++){
			printf("%c",matrizC.matrizC[i][j]);
		}
		printf("\n");
	}
}


int main(){

	//Parámetros que va a recibir del coordinador:
	char nombreArchivo[40];
	int cantidadLineasLeer, cursorY;
	char cadenaComparar[5];
	int flag;

	empaquetado paqueteRel;
	read(STDIN_FILENO, &paqueteRel, sizeof(empaquetado));

	strcpy(nombreArchivo, paqueteRel.nombreArchivo);
	cantidadLineasLeer = paqueteRel.cantidadLineasLeer;
	strcpy(cadenaComparar, paqueteRel.cadenaComparar);
	flag = paqueteRel.flag;
	cursorY = paqueteRel.cursorY;


	/*
	strcpy(nombreArchivo,"ejemploGenerado.txt"); 
	strcpy(cadenaComparar,"TAAT");
	cantidadLineasLeer = 4;
	cursorY = 0;
	flag = 1;
	*/


	matriz matriz1 = leerAtributos(nombreArchivo,0,cursorY,cantidadLineasLeer);
	//imprimirPaquete(paqueteRel);
	//imprimirMatriz(matriz1);

	//¿Se imprime esta sección de código?
	/*if(flag = 1){
		escribirArchivo(matriz1,cadenaComparar);
		imprimirMatriz(matrizImprimir);
		exit(1);
	}
	else{
		escribirArchivo(matriz1,cadenaComparar);
	}*/

	escribirArchivo(matriz1,cadenaComparar,flag);
	return 0;
}
