#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct matriz{
	int cantLineas;
	int largoCadena;
	char** matrizC;
}typedef matriz;




matriz leerAtributos(char* nombreArchivo){
	int i = 0;

	int largoCadena = 60;
	int cantLineas = 30;

	FILE* archivo = fopen(nombreArchivo,"r");
	//Si el archivo no existe:
	if(archivo == NULL){
		printf("No existe tal archivo.\n");
		exit(1);
	}

	char aux;
	
	//fgetc(archivo);
	//fgetc(archivo); //Nos saltamos el primer salto de linea
	char relativo[70];
	char** matrizM = (char**)malloc(sizeof(char*)*30);

	#ifdef __linux__
	fgets(relativo,70,archivo);
	#endif

	while(!feof(archivo)){//Mientras no llegue al final del archivo.
		aux = '0';
		fgets(relativo,70,archivo);
		//fgetc(archivo);
		char* linea = (char*)malloc(sizeof(char)*60);
		for(int j = 0; aux != '\0';j++){

			aux = relativo[j];
		
			if(aux != '\n'){
				linea[j] = aux;
			}
			else{
				aux == '\0';
			}
		}
		matrizM[i] = linea;
		i++;
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
	char* nombreArchivo;
	matriz matriz1 = leerAtributos("ejemploGenerado.txt");
	imprimirMatriz(matriz1);
	return 0;
}
