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

int obtenerLineas(FILE *archivo){
    int contador = 0;
    double temp;
    while(fscanf(archivo,"%lf,", &temp)!=EOF){
        contador++;
    }
    int lineas = contador/COLUMNAS;
    printf("Cantidad lineas archivo: %d\n",lineas);
    fclose(archivo);
    return lineas;
}

Tabla* leerArchivo(char* nombreArchivo){
    Tabla* tabla = inicializarTabla(0,COLUMNAS);
    FILE* archivo = fopen(nombreArchivo,"r");
    

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

    /*int i = 0;
    while(!feof(archivo)){
        aumentarFila(tabla);
        for(int j = 0; j < COLUMNAS;j++){
            fscanf(archivo,"%lf,", &tabla->matriz[i][j]);
            printf("%lf\n",tabla->matriz[i][j]);
        }
        i++;
    }*/

    //No funcionó con esto
    /*int i = 0;
    aumentarFila(tabla);
    while(fscanf(archivo,"%lf,%lf,%lf,%lf,%lf", &tabla->matriz[i][0], &tabla->matriz[i][1], &tabla->matriz[i][2], &tabla->matriz[i][3],&tabla->matriz[i][4])!=EOF){
        aumentarFila(tabla);
        i++;
    }*/

    //Se obtienen las lineas del archivo
    int lineas = obtenerLineas(archivo);
    //Se vuelve a abrir el archivo
    archivo = fopen(nombreArchivo,"r");
    for (int i=0; i<lineas;i++){
        aumentarFila(tabla);
        for(int j = 0; j < COLUMNAS;j++){
            fscanf(archivo,"%lf,", &tabla->matriz[i][j]);
            printf("%lf\n",tabla->matriz[i][j]);
        }
    }
    fclose(archivo);
    return tabla;
}


void generarHebras_Monitores(){

    //Podríamos tener un arreglo con las hebras y monitores y así poder iterar con un ciclo
    pthread_t hilo[discos];
    while(i < numeroHebras){
        pthread_create(&hilo[i], NULL, alguna funcion, algo);
        i++;
    }
}
int main(){
    imprimirTabla(leerArchivo("prueba1.csv"));

    //Argumentos a recibir por línea de comandos (esto será parte del getopt)
    int radios; //número de radios
    int anchoIntervalo; //ancho de cada intervalo
    char *archivoEntrada; 
    char *archivoSalida;
    int tamBuffer; //tamaño del buffer de cada monitor

    /*
    El proceso principal debe crear tantos monitores y hebras como n ́umero de discos hayan sido especifi-
    cados (una hebra por cada disco). Además de mutex, variables de condici ́on y cualquier herramienta
    de pthreads que sea necesaria para proveer exclusi ́on mutua y sincronizaci ́on.
    */

    //Aquí supongo que habría que crear la hebra principal y generar las hebras y monitores
    pthread_t h0; //se crea la hebra principal (hebra padre)
    pthread_create(&h0, NULL, generarHebras_Monitores, algo como un struct );


    return 0;
}

/*void llenarBasura(Tabla* tablaRel){
    for(int i = 0; i < tablaRel->cantidadFilas; i++){
        for(int j = 0; j < tablaRel->cantidadColumnas;j++){
            tablaRel->matriz[i][j] = j + 0.1*i;
        }
    }
}*/
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