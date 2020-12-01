#include <stdio.h>
//#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

struct porcionImg{
    int** matriz;
    int orden;
}typedef porcionImg;

//void* f(void* img){
    

int global;

void imprimirMatriz(porcionImg* img){
    for(int i = 0; i < img->orden;i++){
        for(int j = 0; j < img->orden;j++){
            printf("%d ",img->matriz[i][j]);
        }
        printf("\n");
    }
}

porcionImg* crearImg(int orden){
    porcionImg* img = (porcionImg*)malloc(sizeof(porcionImg));
    img->orden = orden;
    int** matriz = (int**)malloc(sizeof(int*)*orden);
    for(int i = 0; i < orden;i++){
        matriz[i] = (int*)malloc(sizeof(int)*orden);
        for(int j = 0; j < orden;j++){
            matriz[i][j] = i;
        }
    }
    img->matriz = matriz;
    return img;
}
/*porcionImg* dividirEn4(porcionImg* img){
    porcionImg* nuevaImg = (porcionImg*)malloc(sizeof(porcionImg));

    nuevoOrden = img->orden/4;
    nuevaImg->orden = nuevoOrden;
    for(int i = 0; i < nuevoOrden;i++){
        for(int j = 0; j < nuevoOrden;j++){

        }
    }
    return nuevaImg;
}*/

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
            nuevaImg->matriz[i][j]=img->matriz[i+nuevaImg->orden][j];
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
            nuevaImg->matriz[i][j]=img->matriz[i][j+nuevaImg->orden];
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
void *generadoraHebras(void *img){
    pthread_t h2;
    pthread_t h3;
    pthread_t h4;
    pthread_t h5;
    //Se llaman a los cuadrantes y se almacena memoria para 4 nuevas img.
    //Estas img serán el parámetro de cada hebra, y estas van a llamar a suma.
    //El ciclo terminará cuando el contador llegue a 0.
    //Cuando el ciclo termine, entonces las 4 hebras que se crean en esta función seguirán a otra función.
    //Esta función calcula el histograma de la porción de imagen que se pasó como parámetro.
    //No olvidar los free().
    //Esta función debe sumar las histogramas.

    /*pthread_create(&h2,NULL,suma,(void *)a);
    pthread_join(h2,&status);
    pthread_create(&h3,NULL,suma,(void *)a);
    pthread_create(&h4,NULL,suma,(void *)a);
    pthread_create(&h5,NULL,suma,(void *)a);
	porcionImg* img = ((porcionImg*)a);*/
    //imprimirMatriz(img);
}

int main(){
	pthread_t h1;

    int altura = 128;
    porcionImg* img = crearImg(altura);
	void *status;
	pthread_create(&h1, NULL, generadoraHebras, (void *) img);
	//pthread_join(h1,&status);
    printf("\nuno\n");
    porcionImg* uno= cuadrante1(img);
    imprimirMatriz(uno);
    printf("\ndos\n");
    porcionImg* dos= cuadrante2(img);
    imprimirMatriz(dos);
    printf("\ntres\n");
    porcionImg* tres= cuadrante3(img);
    imprimirMatriz(tres);
    printf("\ncuatro\n");
    porcionImg* cuatro= cuadrante4(img);
    imprimirMatriz(cuatro);
	return 0;
}