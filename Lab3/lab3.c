#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <stdint.h>
//#include <unistd.h>
#include <string.h>
#include <math.h>

//Ya que los archivos csv siempre van a tener una cantidad fija de columnas.
#define COLUMNAS 5
struct{
    double u;
    double v;
    double r;
    double i;
    double w;
}typedef Visibilidad;

struct{
    Visibilidad* listaVisibilidad;
    int cantidadFilas;
}typedef Tabla;

struct{
    double mediaReal;
    double mediaImaginaria;
    double potencia;
    double ruidoTotal;
}typedef Disco;

struct{
    Disco** discos;
}typedef EstructuraComun;

//largoMaximo será una variable global
struct{
    int largoBuffer;
    Visibilidad* listaVisibilidad;
}typedef Buffer;

//Variables globales
int largoMaximo, cantidadDiscos,anchoDisco;
EstructuraComun estructuraComun;

//Debug funciones:
void llenarBasura(Tabla* tablaRel);

Tabla* inicializarTabla(int cantidadFilas){
    Tabla* tablaRet = (Tabla*)malloc(sizeof(Tabla));
    tablaRet->listaVisibilidad = (Visibilidad*)malloc(sizeof(Visibilidad)*cantidadFilas);
    /*
    for(int i = 0; i < cantidadFilas; i++){
        tablaRet->listaVisibilidad
[i] = (double*)malloc(sizeof(double)*cantidadColumnas);
    }
    */
    tablaRet->cantidadFilas = cantidadFilas;
    return tablaRet;
}


void aumentarFila(Tabla* tablaRel){
    tablaRel->listaVisibilidad = (Visibilidad*)realloc(tablaRel->listaVisibilidad,sizeof(Visibilidad)*(tablaRel->cantidadFilas+1));
    tablaRel->cantidadFilas = tablaRel->cantidadFilas + 1;
    return;
}

void aumentarFilaTablas(Tabla** tablasRel, int index){
    //Tabla* tablaNueva = (Tabla*)malloc(sizeof(Tabla)*(tablasRel[index]->cantidadFilas + 1));
    Tabla* tablaNueva = inicializarTabla(tablasRel[index]->cantidadFilas + 1);
    for(int i = 0 ; i < tablasRel[index]->cantidadFilas;i++){
        tablaNueva->listaVisibilidad[i].u = tablasRel[index]->listaVisibilidad[i].u;
        tablaNueva->listaVisibilidad[i].v = tablasRel[index]->listaVisibilidad[i].v;
        tablaNueva->listaVisibilidad[i].r = tablasRel[index]->listaVisibilidad[i].r;
        tablaNueva->listaVisibilidad[i].i = tablasRel[index]->listaVisibilidad[i].i;
        tablaNueva->listaVisibilidad[i].w = tablasRel[index]->listaVisibilidad[i].w;
    }
    free(tablasRel[index]);
    tablasRel[index] = tablaNueva;
}

void imprimirTabla(Tabla* tabla){
    for(int i = 0; i < tabla->cantidadFilas;i++){
        printf("%5lf;%5lf;%5lf;%5lf;%5lf\n",tabla->listaVisibilidad[i].u,tabla->listaVisibilidad[i].v,tabla->listaVisibilidad[i].w,tabla->listaVisibilidad[i].i,tabla->listaVisibilidad[i].w);
    }
    return;
}
void imprimirTablas(Tabla** tablas){
    for(int i = 0; i < cantidadDiscos;i++){
        printf("\nDisco %d:\n\n",i+1);
        imprimirTabla(tablas[i]);
    }
    return;
}

void inicializarEstructuraComun(){
    estructuraComun.discos = (Disco**)malloc(sizeof(Disco*)*cantidadDiscos);
    for(int i = 0; i < cantidadDiscos; i++){
        estructuraComun.discos[i] = (Disco*)malloc(sizeof(Disco));
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
int calcularDisco(Visibilidad visibilidad){
    double resultado = sqrt(pow(visibilidad.u,2) + pow(visibilidad.v,2));
    int ite = 0;
    for(int i = 0; i < cantidadDiscos - 1;i++){
        if(resultado >= ite && resultado < anchoDisco*(i+1)){
            return i;
        }
        ite += anchoDisco;
    }
    return cantidadDiscos-1;
}

Tabla** leerArchivo(char* nombreArchivo){
    
    Tabla** tablas = (Tabla**)malloc(sizeof(Tabla*)* cantidadDiscos);
    for(int i = 0; i < cantidadDiscos;i++){
        tablas[i] = inicializarTabla(0);
    }
    FILE* archivo = fopen(nombreArchivo,"r");
    

    if(archivo == NULL){
        printf("No existe tal archivo.\n");
    }

    //Se obtienen las lineas del archivo
    int lineas = obtenerLineas(archivo);
    int index = 0;
    Visibilidad* aux = (Visibilidad*)malloc(sizeof(Visibilidad)*lineas);
    //Se vuelve a abrir el archivo
    archivo = fopen(nombreArchivo,"r");

    for (int i=0; i<lineas;i++){
        fscanf(archivo,"%lf,%lf,%lf,%lf,%lf,",&aux[i].u,&aux[i].v,&aux[i].r,&aux[i].i,&aux[i].w);
        index = calcularDisco(aux[i]);
        printf("INDEX: %d\n",index);
        
        aumentarFilaTablas(tablas,index);
        printf("Soy la tabla %d y mi cantidad de filas son %d\n\n",index,tablas[index]->cantidadFilas);
        tablas[index]->listaVisibilidad[i].u = aux[i].u;
        tablas[index]->listaVisibilidad[i].v = aux[i].v;
        tablas[index]->listaVisibilidad[i].r = aux[i].r;
        tablas[index]->listaVisibilidad[i].i = aux[i].i;
        tablas[index]->listaVisibilidad[i].w = aux[i].w;
        
        //printf("%lf,%lf,%lf,%lf,%lf\n",aux[i].u,aux[i].v,aux[i].r,aux[i].i,aux[i].w);
    }
    free(aux);
    fclose(archivo);
    return tablas;
}


/*void generarHebras_Monitores(){

    //Podríamos tener un arreglo con las hebras y monitores y así poder iterar con un ciclo
    pthread_t hilo[discos];
    while(i < numeroHebras){
        pthread_create(&hilo[i], NULL, alguna funcion, algo);
        i++;
    }
}
*/

int main(){
    cantidadDiscos = 3;
    anchoDisco = 2;

    imprimirTablas(leerArchivo("prueba2.csv"));

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
    //pthread_t h0; //se crea la hebra principal (hebra padre)
    //pthread_create(&h0, NULL, generarHebras_Monitores, algo como un struct );


    return 0;
}

/*void llenarBasura(Tabla* tablaRel){
    for(int i = 0; i < tablaRel->cantidadFilas; i++){
        for(int j = 0; j < tablaRel->cantidadColumnas;j++){
            tablaRel->listaVisibilidad
    [i][j] = j + 0.1*i;
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
        tabla->listaVisibilidad
[tabla->cantidadFilas-1][i] = 10+i;
    }
    imprimirTabla(tabla);
    */

           /*Visibilidad* aux = (Visibilidad*)malloc(sizeof(Visibilidad));
        double temp1;
        double temp2;
        double temp3;
        double temp4;
        double temp5;

        fscanf(archivo,"%lf,",&temp1);
        fscanf(archivo,"%lf,",&temp2);
        fscanf(archivo,"%lf,",&temp3);
        fscanf(archivo,"%lf,",&temp4);
        fscanf(archivo,"%lf,",&temp5);

        aux->u = temp1;
        aux->v = temp2;
        aux->r = temp3;
        aux->i = temp4;
        aux->w = temp5;
        printf("%lf,%lf,%lf,%lf,%lf\n",aux->u,aux->v,aux->r,aux->i,aux->w);
        int index = calcularDisco(aux);
        tablas[index]->listaVisibilidad[i].u = aux->u;
        tablas[index]->listaVisibilidad[i].v = aux->v;
        tablas[index]->listaVisibilidad[i].r = aux->r;
        tablas[index]->listaVisibilidad[i].i = aux->i;
        tablas[index]->listaVisibilidad[i].w = aux->w;
        free(aux);
        */