#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

typedef struct porcionImg{
    int** matriz;
    int orden;
}porcionImg;

//void* f(void* img){

typedef struct quadTree{
    int nivel;
    porcionImg *imagen;
    struct quadTree *cuadrante1;
    struct quadTree *cuadrante2;
    struct quadTree *cuadrante3;
    struct quadTree *cuadrante4;
}quadTree;

int global, niveles, contador=0;
porcionImg *imagenOriginal;

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

    printf("\n ******* Aquí se encarga de calcular el histograma para este cuadrante: *******\n");
    imprimirMatriz(img);
}

void* generadoraHebras(void *img){
    //printf("aaaa\n");
    pthread_t h1;
    pthread_t h2;
    pthread_t h3;
    pthread_t h4;

    void *status;

    quadTree* tree=(quadTree*)img;
    //La imagen se divide en los 4 cuadrantes y aumenta el nivel
    tree->cuadrante1=(quadTree*)malloc(sizeof(quadTree));
    tree->cuadrante1->imagen=cuadrante1(tree->imagen);
    //printf("\nCUADRANTE 1\n");
    //imprimirMatriz(tree->cuadrante1->imagen);
    tree->cuadrante1->nivel=tree->nivel+1;

    tree->cuadrante2=(quadTree*)malloc(sizeof(quadTree));
    tree->cuadrante2->imagen=cuadrante2(tree->imagen);
    //printf("\nCUADRANTE 2\n");
    //imprimirMatriz(tree->cuadrante2->imagen);
    tree->cuadrante2->nivel=tree->nivel+1;

    tree->cuadrante3=(quadTree*)malloc(sizeof(quadTree));
    tree->cuadrante3->imagen=cuadrante3(tree->imagen);
    //printf("\nCUADRANTE 3\n");
    //imprimirMatriz(tree->cuadrante3->imagen);
    tree->cuadrante3->nivel=tree->nivel+1;

    tree->cuadrante4=(quadTree*)malloc(sizeof(quadTree));
    tree->cuadrante4->imagen=cuadrante4(tree->imagen);
    //printf("\nCUADRANTE 4\n");
    //imprimirMatriz(tree->cuadrante4->imagen);
    tree->cuadrante4->nivel=tree->nivel+1;

    /*printf("\nValor del nivel cuadrante 1: %d", tree->cuadrante1->nivel);
    printf("\nValor del nivel cuadrante 2: %d", tree->cuadrante2->nivel);
    printf("\nValor del nivel cuadrante 3: %d", tree->cuadrante3->nivel);
    printf("\nValor del nivel cuadrante 4: %d", tree->cuadrante4->nivel);*/

    //Si los cuadrantes no están en el nivel máximo de descomposición, entonces se generan 4 hebras por cada cuadrante.
    //Recordar que la función generadora de Hebras de por sí genera 4 hebras, y es por ello que cada hebra invoca a esta función de manera recursiva.
    if (tree->cuadrante1->nivel!=niveles && tree->cuadrante2->nivel!=niveles 
    && tree->cuadrante3->nivel!=niveles && tree->cuadrante4->nivel!=niveles){
        //printf("\n entra al if?");
        pthread_create(&h1,NULL,generadoraHebras,tree->cuadrante1);
        pthread_join(h1,&status);

        pthread_create(&h2,NULL,generadoraHebras,tree->cuadrante2);
        pthread_join(h2,&status);

        pthread_create(&h3,NULL,generadoraHebras,tree->cuadrante3);
        pthread_join(h3,&status);

        pthread_create(&h4,NULL,generadoraHebras,tree->cuadrante4);
        pthread_join(h4,&status);

    }
    else
    {
        printf("\n \nENTRANDO AL ELSE?\n");
        pthread_create(&h1,NULL,funcion,tree->cuadrante1->imagen);
        pthread_join(h1,&status);
        pthread_create(&h2,NULL,funcion,tree->cuadrante2->imagen);
        pthread_join(h2,&status);
        pthread_create(&h3,NULL,funcion,tree->cuadrante3->imagen);
        pthread_join(h3,&status);
        pthread_create(&h4,NULL,funcion,tree->cuadrante4->imagen);
        pthread_join(h4,&status);
    }
    


    /*void *status1;
    void *status2;
    contador++;
    printf("\nVALOR DEL CONTADOR: %d",contador);

    printf("\nCreando cuadrante uno\n");
    porcionImg* nuevaImg1=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg1=cuadrante1(img);
    printf("\nValor del cuadrante 1 en nivel: %d\n",contador);
    imprimirMatriz(nuevaImg1);*/

    /*printf("\nCreando cuadrante dos\n");
    porcionImg* nuevaImg2=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg2=cuadrante2(img);
    printf("\nValor del cuadrante 2 en nivel: %d\n",contador);
    imprimirMatriz(nuevaImg2);

    printf("\nCreando cuadrante tres\n");
    porcionImg* nuevaImg3=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg3=cuadrante3(img);
    printf("\nValor del cuadrante 3 en nivel: %d\n",contador);
    imprimirMatriz(nuevaImg3);

    printf("\nCreando cuadrante cuatro\n");
    porcionImg* nuevaImg4=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg4=cuadrante4(img);
    printf("\nValor del cuadrante 4 en nivel: %d\n",contador);
    imprimirMatriz(nuevaImg4);*/

    //Aquí se pregunta si el contador el igual al valor de niveles, si lo es entonces hacer el trabajo, sino generar 4 hebras para esta hebra.
    /*if(contador==niveles){
        pthread_create(&h1, NULL, funcion, (void *) nuevaImg1);
        pthread_join(h1,&resultadoCuad1);
    }
    else{
        pthread_create(&h1, NULL, generadoraHebras, (void *) nuevaImg1);
        //imprimirMatriz(nuevaImg1);
        pthread_join(h1,&status1);
    }*/

    
    /*printf("\n\n\n PASANDO A CUADRANTE 2\n\n\n");
    printf("\nCreando cuadrante dos\n");
    porcionImg* nuevaImg2=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg2=cuadrante2(img);
    printf("\nValor del cuadrante 2 en nivel: %d\n",contador);
    imprimirMatriz(nuevaImg2);
    //Aquí se pregunta si el contador el igual al valor de niveles, si lo es entonces hacer el trabajo, sino generar 4 hebras para esta hebra.
    if(contador==niveles){
        pthread_create(&h2, NULL, funcion, (void *) nuevaImg2);
        pthread_join(h2,&resultadoCuad2);
    }
    else{
        pthread_create(&h2, NULL, generadoraHebras, (void *) nuevaImg2);
        //imprimirMatriz(nuevaImg1);
        pthread_join(h2,&status2);
    }*/


    /*pthread_create(&h3, NULL, funcion, (void *) nuevaImg2);
    pthread_join(h3,&resultadoCuad2);*/

    /*printf("\nCreando cuadrante tres\n");
    porcionImg* nuevaImg3=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg3=cuadrante3(img);
    pthread_create(&h4, NULL, funcion, (void *) nuevaImg3);
    pthread_join(h4,&resultadoCuad3);

    printf("\nCreando cuadrante cuatro\n");
    porcionImg* nuevaImg4=(porcionImg*)malloc(sizeof(porcionImg));
    nuevaImg4=cuadrante4(img);
    pthread_create(&h5, NULL, funcion, (void *) nuevaImg4);
    pthread_join(h5,&resultadoCuad4);*/


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
    int altura = 8;
    //niveles está como variable global
    niveles=2;
    //porcionImg* img = crearImg(altura);
    //En el primer nivel se tendría la imagen original, es decir, sin dvidirla por cuadrantes.
    //imagenOriginal está como variable global.
    imagenOriginal = crearImg(altura);
    quadTree* main=(quadTree*)malloc(sizeof(quadTree));
    main->nivel=0;
    main->imagen=imagenOriginal;
    main->cuadrante1=NULL;
    main->cuadrante2=NULL;
    main->cuadrante3=NULL;
    main->cuadrante4=NULL;
    
    imprimirMatriz(imagenOriginal);
	void *resultadoFinal;
    printf("\nNiveles: %d",niveles);

	pthread_create(&h1, NULL, generadoraHebras, (void *) main);
    pthread_join(h1,&resultadoFinal);
	return 0;
}