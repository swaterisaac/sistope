#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <stdint.h>
//#include <unistd.h>
#include <string.h>

//Ya que los archivos csv siempre van a tener una cantidad fija de columnas.
#define COLUMNAS 5

struct{
    double** matriz;
    int cantidadColumnas;
    int cantidadFilas;
}typedef Tabla;

//Debug funciones:
void llenarBasura(Tabla* tablaRel);

Tabla* inicializarTabla(int cantidadFilas, int cantidadColumnas){
    Tabla* tablaRet = (Tabla*)malloc(sizeof(Tabla));
    tablaRet->matriz = (double**)malloc(sizeof(double*)*cantidadFilas);
    for(int i = 0; i < cantidadFilas; i++){
        tablaRet->matriz[i] = (double*)malloc(sizeof(double)*cantidadColumnas);
    }
    tablaRet->cantidadColumnas = cantidadColumnas;
    tablaRet->cantidadFilas = cantidadFilas;
    return tablaRet;
}


void aumentarFila(Tabla* tablaRel){
    tablaRel->matriz = (double**)realloc(tablaRel->matriz,sizeof(double**)*tablaRel->cantidadFilas+1);
    tablaRel->matriz[tablaRel->cantidadFilas] = (double*)malloc(sizeof(double)*tablaRel->cantidadColumnas);

    tablaRel->cantidadFilas = tablaRel->cantidadFilas + 1;
    return;
}



void imprimirTabla(Tabla* tabla){
    for(int i = 0; i < tabla->cantidadFilas;i++){
        for(int j = 0; j < tabla->cantidadColumnas - 1;j++){
            printf("%5lf;",tabla->matriz[i][j]);        
        }
        printf("%5lf",tabla->matriz[i][tabla->cantidadColumnas - 1]);
        printf("\n");
    }
    return;
}

Tabla* leerArchivo(char* nombreArchivo){
    Tabla* tabla = inicializarTabla(0,COLUMNAS);
    FILE* archivo = fopen(nombreArchivo,"r");
    int i = 0;

    if(archivo == NULL){
        printf("No existe tal archivo.\n");
    }
    /*
    double aux;
    for(int i = 0; i < 100; i++){
        fscanf(archivo,"%lf", &aux);
        printf("%lf\n",aux);
        fgetc(archivo);
    }
    */

    
    while(!feof(archivo)){
        aumentarFila(tabla);
        for(int j = 0; j < COLUMNAS;j++){
            fscanf(archivo,"%lf,", &tabla->matriz[i][j]);
            printf("%lf\n",tabla->matriz[i][j]);
        }
        i++;
    }
    
    fclose(archivo);
    return tabla;
}
int main(){
    imprimirTabla(leerArchivo("prueba2.csv"));
    return 0;
}

void llenarBasura(Tabla* tablaRel){
    for(int i = 0; i < tablaRel->cantidadFilas; i++){
        for(int j = 0; j < tablaRel->cantidadColumnas;j++){
            tablaRel->matriz[i][j] = j + 0.1*i;
        }
    }
}
//Debug 1
    /*
    Tabla* tabla = inicializarTabla(0,COLUMNAS);
    for(int i = 0; i < 10;i++){
        aumentarFila(tabla);
    }
    llenarBasura(tabla);
    //imprimirTabla(tabla);

    aumentarFila(tabla);
    for(int i = 0; i < COLUMNAS; i++){
        tabla->matriz[tabla->cantidadFilas-1][i] = 10+i;
    }
    imprimirTabla(tabla);
    */