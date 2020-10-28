#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct matriz{
	int cantLineas;
	int largoCadena;
	char** matrizC;
}typedef matriz;



//Por defecto, cursorX y cursorY deberían ser 0.
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

	while(!feof(archivo) || i == -1){//Mientras no llegue al final del archivo.
		aux = '0';
		cont++;
		fgets(relativo,70,archivo);
		//fgetc(archivo);
		char* linea = (char*)malloc(sizeof(char)*largoCadena);
		//////////////////////////////////////////////////////////////////
		if(i == 0){
			for(int j = 0; j < cursorX;j++){
				linea[j] = aux;
			}
			for(int j = cursorX; aux != '\0';j++){
				aux = relativo[j];
				linea[j] = aux;
			}
		}

		else{
			for(int j = 0; aux != '\0';j++){
				aux = relativo[j];
		
				if(aux != '\n'){
					linea[j] = aux;
				}
				else{
					aux == '\0';
				}
			}	
		}
		matrizM[i] = linea;
		i++;
		if(i == cantLineas){
			i = -1;
		}
	}
	if(cantLineas > cont){
		cantLineas = cont;
	}

	fclose(archivo);
	matriz matrizRetorno;
	matrizRetorno.matrizC = matrizM;
	matrizRetorno.largoCadena = largoCadena;
	matrizRetorno.cantLineas = cantLineas;
	return matrizRetorno;
}

void imprimirMatriz(matriz matrizC){
	for(int i = 0; i < matrizC.cantLineas;i++){
		for(int j = 0; j < matrizC.largoCadena;j++){
			printf("%c",matrizC.matrizC[i][j]);
		}
		printf("\n");
	}

}
int main(){
	matriz matriz1 = leerAtributos("ejemploGenerado.txt",10,25,2);
	imprimirMatriz(matriz1);
	return 0;
}