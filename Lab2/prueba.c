#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

struct xd{
    int** matriz;
    int orden;
    int cuadrante;
}typedef xd;

//void* f(void* ola){
    

int global;

void imprimirMatriz(xd* ola){
    for(int i = 0; i < ola->orden;i++){
        for(int j = 0; j < ola->orden;j++){
            printf("%d ",ola->matriz[i][j]);
        }
        printf("\n");
    }
}

xd* crearXD(int orden){
    xd* ola = (xd*)malloc(sizeof(xd));
    ola->orden = orden;
    int** matriz = (int**)malloc(sizeof(int*)*orden);
    for(int i = 0; i < orden;i++){
        matriz[i] = (int*)malloc(sizeof(int)*orden);
        for(int j = 0; j < orden;j++){
            matriz[i][j] = 1;
        }
    }
    ola->matriz = matriz;
    return ola;
}
xd* dividirEn4(xd* img){
    xd* nuevaImg = (xd*)malloc(sizeof(xd));

    nuevoOrden = img->orden/4;
    nuevaImg->orden = nuevoOrden;
    for(int i = 0; i < nuevoOrden;i++){
        for(int j = 0; j < nuevoOrden;j++){

        }
    }
    return nuevaImg;
}
void *suma(void *a){
    pthread_t h2;
    pthread_t h3;
    pthread_t h4;
    pthread_t h5;
	xd* ola = ((xd*)a);
    imprimirMatriz(ola);
}

int main(){
	pthread_t h1;

    int altura = 128;
    xd* ola = crearXD(altura);
	void *status;
	pthread_create(&h1, NULL, suma, (void *) ola);
	pthread_join(h1,&status);
	return 0;
}