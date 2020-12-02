#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char fileMarker1;                                                                                                                                                                                              
    unsigned char fileMarker2;                                                                                                                                                                                               
    unsigned int   bfSize;                                                                                                                                                                                                                   
    uint16_t unused1;                                                                                                                                                                                                                        
    uint16_t unused2;                                                                                                                                                                                                                        
    unsigned int   imageDataOffset;                                                                                                                                                            
} bmpFileHeader;                                                                                                                                                                                                                                

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned int   biSize;                                                                                                                                                                                                                   
    int            width;                                                                                                                                                                
    int            height;                                                                                                                                                                     
    uint16_t planes;                                                                                                                                                                                                                         
    uint16_t bitPix;                                                                                                                                                                                                                         
    unsigned int   biCompression;                                                                                                                                                                                                            
    unsigned int   biSizeImage;                                                                                                                                                                                                              
    int            biXPelsPerMeter;                                                                                                                                                                                                          
    int            biYPelsPerMeter;                                                                                                                                                                                                          
    unsigned int   biClrUsed;                                                                                                                                                                                                                
    unsigned int   biClrImportant;                                                                                                                                                                                                           
} bmpInfoHeader; 

typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char  b;                                                                                                                                                                                                                        
    unsigned char  g;                                                                                                                                                                                                                        
    unsigned char  r;                                                                                                                                                                                                                        
}pixel;

typedef struct matrizPixel{
    pixel** matriz;
    int orden;
}matrizPixel;

typedef struct quadTree{
    int nivel;
    matrizPixel *imagen;
    struct quadTree *cuadrante1;
    struct quadTree *cuadrante2;
    struct quadTree *cuadrante3;
    struct quadTree *cuadrante4;
}quadTree;

int global, niveles, contador=0;

void imprimirMatriz(matrizPixel* img){
    for(int i = 0; i < img->orden;i++){
        for(int j = 0; j < img->orden;j++){
            //printf("%d ",img->matriz[i][j]);
            printf("[ %d %d %d ]",img->matriz[i][j].r,img->matriz[i][j].g,img->matriz[i][j].b);
        }
        printf("\n");
    }
}

matrizPixel* cargarImagen(char *filename){
    FILE *f;
    bmpFileHeader header;
    unsigned char *imgdata;
    uint16_t type;
    bmpInfoHeader bInfoHeader;

    f = fopen (filename, "rb");
    if (!f){
        printf("Error al abrir el archivo\n");
        return NULL;
    }
    matrizPixel* imagenOriginal=(matrizPixel*)malloc(sizeof(matrizPixel));
    fread(&header, sizeof(unsigned char), sizeof(bmpFileHeader), f);
    fread(&bInfoHeader, sizeof(unsigned char), sizeof(bmpInfoHeader), f);
    int largoImagen = bInfoHeader.width;
    printf("Largo de la imagen: %d\n",largoImagen);
    /*pixel** matrizPixel = (pixel**)malloc(sizeof(pixel*)*largoImagen);
    for(int i = 0; i < largoImagen;i++){
        pixel* filaPixel = (pixel*)malloc(sizeof(pixel)*largoImagen);
        for(int j = 0;j < largoImagen;j++){
            fread(&filaPixel[j], 1,sizeof(pixel), f);
            printf("%d %d %d|",matrizPixel[i][j].r,matrizPixel[i][j].g,matrizPixel[i][j].b); //Para printear los píxeles (debug).
        }
        matrizPixel[i] = filaPixel;
        printf("\n");
    }*/
  
    pixel** matriz = (pixel**)malloc(sizeof(pixel*)*largoImagen);
    for(int i=0;i<largoImagen;i++){
        matriz[i]=(pixel*)malloc(sizeof(pixel));
    }
    for (int i=0;i<largoImagen;i++){
        for(int j=0;j<largoImagen;j++){
            fread(&matriz[i][j],1,sizeof(pixel),f);
            printf("%d %d %d|",matriz[i][j].r,matriz[i][j].g,matriz[i][j].b); //Para printear los píxeles (debug).
        }
        printf("\n");
    }
    
    imagenOriginal->matriz=matriz;
    imagenOriginal->orden=largoImagen;
    fclose(f);
    return imagenOriginal;
}


/*matrizPixel* crearImg(int orden){
    int contador=1;
    matrizPixel* img = (matrizPixel*)malloc(sizeof(matrizPixel));
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
}*/

matrizPixel* cuadrante1(matrizPixel* img){
    matrizPixel* nuevaImg= (matrizPixel *)malloc(sizeof(matrizPixel));
    
    nuevaImg->orden=img->orden/2;
    nuevaImg->matriz=(pixel **)malloc(sizeof(pixel*)*nuevaImg->orden);
    for (int i=0; i< nuevaImg->orden; i++){
        nuevaImg->matriz[i]=(pixel *)malloc(sizeof(pixel)*nuevaImg->orden);
        for (int j=0; j<nuevaImg->orden;j++){
            nuevaImg->matriz[i][j]=img->matriz[i][j];
            //printf("%d ",nuevaImg->matriz[i][j]);
        }
        //printf("\n");
    }
    return nuevaImg;
}

matrizPixel* cuadrante2(matrizPixel* img){
    matrizPixel* nuevaImg= (matrizPixel *)malloc(sizeof(matrizPixel));
    nuevaImg->orden=img->orden/2;
    nuevaImg->matriz=(pixel **)malloc(sizeof(pixel*)*nuevaImg->orden);
    for (int i=0; i< nuevaImg->orden; i++){
        nuevaImg->matriz[i]=(pixel *)malloc(sizeof(pixel)*nuevaImg->orden);
        for (int j=0; j<nuevaImg->orden;j++){
            nuevaImg->matriz[i][j]=img->matriz[i][j+nuevaImg->orden];
            //printf("%d ",nuevaImg->matriz[i][j]);
        }
        //printf("\n");
    }
    return nuevaImg;
}

matrizPixel* cuadrante3(matrizPixel* img){
    matrizPixel* nuevaImg= (matrizPixel *)malloc(sizeof(matrizPixel));
    nuevaImg->orden=img->orden/2;
    nuevaImg->matriz=(pixel **)malloc(sizeof(pixel*)*nuevaImg->orden);
    for (int i=0; i< nuevaImg->orden; i++){
        nuevaImg->matriz[i]=(pixel *)malloc(sizeof(pixel)*nuevaImg->orden);
        for (int j=0; j<nuevaImg->orden;j++){
            nuevaImg->matriz[i][j]=img->matriz[i+nuevaImg->orden][j];
            //printf("%d ",nuevaImg->matriz[i][j]);
        }
        //printf("\n");
    }
    return nuevaImg;
}

matrizPixel* cuadrante4(matrizPixel* img){
    matrizPixel* nuevaImg= (matrizPixel *)malloc(sizeof(matrizPixel));
    nuevaImg->orden=(img->orden)/2;
    nuevaImg->matriz=(pixel **)malloc(sizeof(pixel*)*nuevaImg->orden);
    for (int i=0; i< nuevaImg->orden; i++){
        nuevaImg->matriz[i]=(pixel *)malloc(sizeof(pixel)*nuevaImg->orden);
        for (int j=0; j<nuevaImg->orden;j++){
            nuevaImg->matriz[i][j]=img->matriz[i+nuevaImg->orden][j+nuevaImg->orden];
            //printf("%d ",matriz[i][j]);
        }
        //printf("\n");
    }
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
    matrizPixel* nuevaImg1=(matrizPixel*)malloc(sizeof(matrizPixel));
    nuevaImg1=cuadrante1(img);
    printf("\nValor del cuadrante 1 en nivel: %d\n",contador);
    imprimirMatriz(nuevaImg1);*/

    /*printf("\nCreando cuadrante dos\n");
    matrizPixel* nuevaImg2=(matrizPixel*)malloc(sizeof(matrizPixel));
    nuevaImg2=cuadrante2(img);
    printf("\nValor del cuadrante 2 en nivel: %d\n",contador);
    imprimirMatriz(nuevaImg2);

    printf("\nCreando cuadrante tres\n");
    matrizPixel* nuevaImg3=(matrizPixel*)malloc(sizeof(matrizPixel));
    nuevaImg3=cuadrante3(img);
    printf("\nValor del cuadrante 3 en nivel: %d\n",contador);
    imprimirMatriz(nuevaImg3);

    printf("\nCreando cuadrante cuatro\n");
    matrizPixel* nuevaImg4=(matrizPixel*)malloc(sizeof(matrizPixel));
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
    matrizPixel* nuevaImg2=(matrizPixel*)malloc(sizeof(matrizPixel));
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
    matrizPixel* nuevaImg3=(matrizPixel*)malloc(sizeof(matrizPixel));
    nuevaImg3=cuadrante3(img);
    pthread_create(&h4, NULL, funcion, (void *) nuevaImg3);
    pthread_join(h4,&resultadoCuad3);

    printf("\nCreando cuadrante cuatro\n");
    matrizPixel* nuevaImg4=(matrizPixel*)malloc(sizeof(matrizPixel));
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

//descripción: permite verificar si un número es potencia de 2.
//entrada: un valor entero.
//salida: 1 si el valor ingresado es potencia de 2, o 0 si no lo es.
int comprobarPot2(int num){
    int res=2;
    if(num==res){
        return 1;
    }
    while(res!=num){
        res=2*res;
        if(res==num){
            return 1;
        }
        if (res>num){
            return 0;
        }
    }
}
//descripción: imprime un mensaje de error por pantalla.
//entrada: arreglo con las entradas ingresadas.
//salida: imprime por pantalla.
void imprimirError(char *argv[]){
	fprintf(stderr, "Uso correcto: %s \n[-i string: Nombre del archivo]\n[-o string: Nombre del archivo del histograma]"
	"\n[-L numero entero: Cantidad de niveles]\n[-B numero entero: Número de bins]\n",
	argv[0]);
	return;
}

//descripción: permite ingresar argumentos desde la línea de comandos.
//Entradas: Recibe 
//argc: que consiste en el contador de argumentos ingresados en la linea de comandos. El nombre del programa que se ejecuta se cuenta como un argumento
//argv: arreglo con las entradas ingresadas por linea de comandos
//**nombreArchivo, **salida, *cantidadNiveles, *cantidadBins: punteros a variables que se utilizarán para pasar valores por referencia
//Salida: No posee retorno, ya que es una función void.
void recibirArgumentos(int argc, char *argv[], char **nombreArchivo, char **salida, int *cantidadNiveles, int *cantidadBins){
    int opt;
	char *aux3;
	aux3 = malloc(10*sizeof(char));
	if(argc < 9 || argc > 10){//si se ingresa un numero de argumentos menor a 9 o mayor a 10, se finaliza la ejecución del programa
		printf("Se ingresoó un número incorrecto de argumentos.\n");
		imprimirError(argv);
		exit(EXIT_FAILURE);
	}
	int N = -1;
    int N2 = -1;
	//Se inicia un ciclo while hasta que se verifiquen todos los argumentos ingresados como entradas, los cuales se reciben con getopt()
	//int getopt (int argc, char *const *argv, const char *options): Siempre se le debe entregar argc y argv. El tercer argumento consiste en el formato de las entradas, ya sea
	//el caracter correspondiente a la opcion/entrada que se quiere recibir y además se debe indicar si dicha entrada estará acompañada de algún valor
	//Esto se indica con ":". Por lo tanto, "i:" quiere decir que se espera recibir la opcion -i y esta si o si debe estar acompañada de un valor
	while((opt = getopt(argc, argv, "i:o:L:B:")) != -1 ){ 
	    //opt recibe el argumento leido (se hace de forma secuencial) y se ingresa a un switch
	    //En caso de que opt sea -1, se dejaran de buscar entradas y por lo tanto se podrá salir del while
	    switch(opt) {
	    case 'i': //se busca la entrada -i
			*nombreArchivo= optarg;
            FILE * fp = fopen(*nombreArchivo, "rb");
		    if(optarg!=0 && strcmp(*nombreArchivo,"")==0){//si no se ingresa un argumento junto a -i o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("Error de formato.\n");
				imprimirError(argv);
				exit(EXIT_FAILURE);
		    }
            else if(fp==NULL){
               printf("No existe tal archivo.\n");
               imprimirError(argv);
			    exit(EXIT_FAILURE);
            }
		    break;

        case 'o': //se busca la entrada -o
			*salida= optarg;
		    if(optarg!=0 && strcmp(*salida,"")==0){//si no se ingresa un argumento junto a -i o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("Error de formato\n");
				imprimirError(argv);
				exit(EXIT_FAILURE);
		    }
		    break;
        case 'L': //se busca la entrada -L
		    N = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -L a entero
		    if(optarg!=0 && N<0){//si no se ingresa un argumento junto a -n o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("Procure ingresar un número positivo menor a 8 para el parámetro L.\n");
                imprimirError(argv);
				exit(EXIT_FAILURE);
			}
            else if(N>8){
                printf("La cantidad de niveles no puede ser un valor mayor a 8.\n");
                imprimirError(argv);
				exit(EXIT_FAILURE);
            }
            else if(comprobarPot2(N)==0){
                printf("La cantidad de niveles debe ser un valor potencia de 2.\n");
				imprimirError(argv);
				exit(EXIT_FAILURE);
            }
		    break;
        case 'B': //se busca la entrada -B
		    N2 = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -B a entero
		    if(optarg!=0 && N2 <1 ){//si no se ingresa un argumento junto a -c o si no se logra parsear el argumento ingresado, se considera como invalido
				printf("La cantidad de bins debe ser un valor mayor a 1.\n");
                imprimirError(argv);
				exit(EXIT_FAILURE);
			}
            else if(comprobarPot2(N2)==0){
                printf("La cantidad de bins debe ser un valor potencia de 2.\n");
				imprimirError(argv);
				exit(EXIT_FAILURE);
            }
		    break;
	    default: /* '?' */
		   imprimirError(argv);
		   exit(EXIT_FAILURE);
	    }
	}
	(*cantidadNiveles) = N; //se iguala la variable cantidadNiveles a N, para poder acceder al valor en el main
    (*cantidadBins) = N2; //se iguala la variable cantidadBins a N2, para poder acceder al valor en el main
	if(N<=0 && *cantidadNiveles>8){
		printf("El valor que acompaña a -L debe ser un mayor a 0\n: %d",N);
		imprimirError(argv);
		exit(EXIT_FAILURE);
	}
    if(N2<1){
		printf("El valor que acompaña a -B debe ser un mayor a 1\n");
		imprimirError(argv);
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char *argv[]){
	pthread_t h1;
    int altura = 8;
    
    matrizPixel* imagenOriginal= cargarImagen("imagen8.bmp");
    //niveles está como variable global
    /*niveles=2;
    //matrizPixel* img = crearImg(altura);
    //En el primer nivel se tendría la imagen original, es decir, sin dvidirla por cuadrantes.
    //imagenOriginal está como variable global.
    //imagenOriginal = crearImg(altura);
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
    pthread_join(h1,&resultadoFinal);*/


    /*int cantidadNiveles = 0, cantidadBins = 0;
    char *nombreArchivo, *salida;
    //Se reciben atributos por el getOPT
	recibirArgumentos(argc, argv, &nombreArchivo, &salida, &cantidadNiveles, &cantidadBins );
    printf("El argumento de flag -i es: %s\n", nombreArchivo);
    printf("El argumento de flag -o es: %s\n", salida);
    printf("El argumento de flag -L es: %d\n", cantidadNiveles);
    printf("El argumento de flag -B es: %d\n", cantidadBins);*/

    /*int numero;
    scanf("%d",&numero);
    printf("potencia de 2: %d\n",comprobarPot2(numero));*/


	return 0;
}   