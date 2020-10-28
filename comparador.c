#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


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
			for(int j = cursorX; aux != '\0' || j < largoCadena;j++){
				aux = relativo[j];
				if(aux != '\n'){
					linea[j] = aux;	
				}
				else{
					aux = '\0';
				}
			}
		}

		else{
			for(int j = 0; aux != '\0' || j < largoCadena;j++){
				aux = relativo[j];
		
				if(aux != '\n'){
					linea[j] = aux;
				}
				else{
					linea[j] = '\n';
					aux = '\0';
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

matriz leerArchCompleto(char* nombreArchivo, int cantLineas){
	return leerAtributos(nombreArchivo,0,0,cantLineas);
}

int compararLinea(matriz textoArchivo,int linea,char* cadenaComparar){
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

matriz separarTexto(matriz textoArchivo,int cursorX,int cursorY,int cantLineas){
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

}

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

void escribirArchivo(matriz lectura,char* cadena){
	char nombreArchivo[40];
	char PID[10];
	
	sprintf(PID,"%d",(int)getpid());

	strcpy(nombreArchivo,"rp_");
	strcat(nombreArchivo, cadena);
	strcat(nombreArchivo,"_");
	strcat(nombreArchivo, PID);
	strcat(nombreArchivo,".txt");

	FILE* arch = fopen(nombreArchivo,"w");
	for(int i = 0; i < lectura.cantLineas;i++){
		//strcpy(lineaRel,lectura.matrizC[i]);
		char* lineaRel = (char*)malloc(sizeof(char)*lectura.largoCadena);
		for(int j = 0;j < lectura.largoCadena;j++){
			lineaRel[j] = lectura.matrizC[i][j];
		}
		strcat(lineaRel, "  ");
		strcat(lineaRel, transformarBool(compararLinea(lectura,i,cadena)));
		strcat(lineaRel,"\n");
		fputs(lineaRel,arch);
		//strcat("\n");
	}
	fclose(arch);
	return;
}

void imprimirMatriz(matriz matrizC){
	for(int i = 0; i < matrizC.cantLineas;i++){
		for(int j = 0; j < matrizC.largoCadena;j++){
			printf("%c",matrizC.matrizC[i][j]);
		}
		printf("\n");
	}

}
void imprimirMatriz2(matriz matrizC){
	for(int i = 0; i < matrizC.cantLineas;i++){
		printf("%s",matrizC.matrizC[i]);
		//printf("\n");
	}
}
int main(){
	//matriz matriz1 = leerAtributos("ejemploGenerado.txt",10,25,2);
	matriz matriz1 = leerArchCompleto("ejemploGenerado.txt",30);
	matriz matriz2 = separarTexto(matriz1,0,29,1);
	//imprimirMatriz2(matriz1);
	//imprimirMatriz(matriz2);
	//printf("%d\n", compararLinea(matriz2,0,"TTAA"));
	escribirArchivo(matriz1,"AAAA");
	return 0;
}
