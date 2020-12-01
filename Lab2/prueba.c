#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

struct porcionImg{
    int** matriz;
    int orden;
}typedef porcionImg;

//void* f(void* img){
    

int global, niveles=3;

void imprimirMatriz(porcionImg* img){
    for(int i = 0; i < img->orden;i++){
        for(int j = 0; j < img->orden;j++){
            printf("%d ",img->matriz[i][j]);
        }
        printf("\n");
    }
}

porcionImg* crearImg(int orden){
    int contador=1;
    porcionImg* img = (porcionImg*)malloc(sizeof(porcionImg));
    img->orden = orden;
    int** matriz = (int**)malloc(sizeof(int*)*orden);
    for(int i = 0; i < orden;i++){
        matriz[i] = (int*)malloc(sizeof(int)*orden);
        for(int j = 0; j < orden;j++){
            matriz[i][j] = contador;
            contador++;
        }
    }
    img->matriz = matriz;
    return img;
}

porcionImg* cuadrante1(porcionImg* img){
    porcionImg* nuevaImg= (porcionImg *)malloc(sizeof(porcionImg));
    
    nuevaImg->orden=img->orden/2;
    nuevaImg->matriz=(int **)malloc(sizeof(int*)*nuevaImg->orden);
    for (int i=0; i< nuevaImg->orden; i++){
        nuevaImg->matriz[i]=(int *)malloc(sizeof(int)*nuevaImg->orden);
        for (int j=0; j<nuevaImg->orden;j++){
            nuevaImg->matriz[i][j]=img->matriz[i][j];
            //printf("%d ",nuevaImg->matriz[i][j]);
        }
        //printf("\n");
    }
    return nuevaImg;
}

porcionImg* cuadrante2(porcionImg* img){
    porcionImg* nuevaImg= (porcionImg *)malloc(sizeof(porcionImg));
    nuevaImg->orden=img->orden/2;
        nuevaImg->matriz=(int **)malloc(sizeof(int*)*nuevaImg->orden);
    for (int i=0; i< nuevaImg->orden; i++){
        nuevaImg->matriz[i]=(int *)malloc(sizeof(int)*nuevaImg->orden);
        for (int j=0; j<nuevaImg->orden;j++){
            nuevaImg->matriz[i][j]=img->matriz[i][j+nuevaImg->orden];
            //printf("%d ",nuevaImg->matriz[i][j]);
        }
        //printf("\n");
    }
    return nuevaImg;
}

porcionImg* cuadrante3(porcionImg* img){
    porcionImg* nuevaImg= (porcionImg *)malloc(sizeof(porcionImg));
    nuevaImg->orden=img->orden/2;
    nuevaImg->matriz=(int **)malloc(sizeof(int*)*nuevaImg->orden);
    for (int i=0; i< nuevaImg->orden; i++){
        nuevaImg->matriz[i]=(int *)malloc(sizeof(int)*nuevaImg->orden);
        for (int j=0; j<nuevaImg->orden;j++){
            nuevaImg->matriz[i][j]=img->matriz[i+nuevaImg->orden][j];
            //printf("%d ",nuevaImg->matriz[i][j]);
        }
        //printf("\n");
    }
    return nuevaImg;
}

porcionImg* cuadrante4(porcionImg* img){
    porcionImg* nuevaImg= (porcionImg *)malloc(sizeof(porcionImg));
    nuevaImg->orden=(img->orden)/2;
    int ** matriz=(int **)malloc(sizeof(int*)*nuevaImg->orden);
    for (int i=0; i< nuevaImg->orden; i++){
        matriz[i]=(int *)malloc(sizeof(int)*nuevaImg->orden);
        for (int j=0; j<nuevaImg->orden;j++){
            matriz[i][j]=img->matriz[i+nuevaImg->orden][j+nuevaImg->orden];
            //printf("%d ",matriz[i][j]);
        }
        //printf("\n");
    }
    nuevaImg->matriz=matriz;
    return nuevaImg;
}
void* funcion(void *img){
    imprimirMatriz(img);
}

void* generadoraHebras(void *img){
    pthread_t h2;
    pthread_t h3;
    pthread_t h4;
    pthread_t h5;
    void *resultadoCuad1;
    void *resultadoCuad2;
    void *resultadoCuad3;
    void *resultadoCuad4;

    printf("\nCreando cuadrante uno\n");
    porcionImg* nuevaImg1=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg1=cuadrante1(img);
    pthread_create(&h2, NULL, funcion, (void *) nuevaImg1);
    pthread_join(h2,&resultadoCuad1);

    printf("\nCreando cuadrante dos\n");
    porcionImg* nuevaImg2=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg2=cuadrante2(img);
    pthread_create(&h3, NULL, funcion, (void *) nuevaImg2);
    pthread_join(h3,&resultadoCuad2);

    printf("\nCreando cuadrante tres\n");
    porcionImg* nuevaImg3=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg3=cuadrante3(img);
    pthread_create(&h4, NULL, funcion, (void *) nuevaImg3);
    pthread_join(h4,&resultadoCuad3);

    printf("\nCreando cuadrante cuatro\n");
    porcionImg* nuevaImg4=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg4=cuadrante4(img);
    pthread_create(&h5, NULL, funcion, (void *) nuevaImg4);
    pthread_join(h5,&resultadoCuad4);


    //Se llaman a los cuadrantes y se almacena memoria para 4 nuevas img.
    //Estas img serán el parámetro de cada hebra, y estas van a llamar a suma.
    //El ciclo terminará cuando el contador llegue a 0.
    //Cuando el ciclo termine, entonces las 4 hebras que se crean en esta función seguirán a otra función.
    //Esta función calcula el histograma de la porción de imagen que se pasó como parámetro.
    //No olvidar los free().
    //Esta función debe sumar las histogramas.

}

int main(){
	pthread_t h1;

    int altura = 4;
    porcionImg* img = crearImg(altura);
	void *resultadoFinal;
	pthread_create(&h1, NULL, generadoraHebras, (void *) img);
    pthread_join(h1,&resultadoFinal);
	return 0;
}