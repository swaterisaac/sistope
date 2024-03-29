#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////
//Estructura que almacena la primera cabecera de la imagen BMP
//(Información relevante: imageDataOffset, indica dónde comienza la cabecera de los píxeles de la imagen.)
typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char fileMarker1;                                                                                                                                                                                              
    unsigned char fileMarker2;                                                                                                                                                                                               
    unsigned int   bfSize;                                                                                                                                                                                                                   
    uint16_t unused1;                                                                                                                                                                                                                        
    uint16_t unused2;                                                                                                                                                                                                                        
    unsigned int   imageDataOffset;                                                                                                                                                            
} bmpFileHeader;                                                                                                                                                                                                                                
/////////////////////////////////////////////////////////////////////
//Estructura que almacena la segunda cabecera de la imagen BMP
//(Información relevante en este caso: width y height)
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
/////////////////////////////////////////////////////////////////////
//Estructura que permite almacenar los atributos de un pixel, que son R,G, B
//y alpha (que no se ocupa pero hace que se lea correctamente la imagen de 32 bits)
typedef struct __attribute__((__packed__)) {                                                                                                                                                                                                                             
    unsigned char  b;                                                                                                                                                                                                                        
    unsigned char  g;                                                                                                                                                                                                                        
    unsigned char  r;
    unsigned char alpha;                                                                                                                                                                                                                 
}pixel;
/////////////////////////////////////////////////////////////////////
//Estructura que permite reprrsentar un bin, considerando límite inferior y superior, y su valor de I.
typedef struct bin {
  int inferior;
  int superior;
  int valorI;
}bin;
/////////////////////////////////////////////////////////////////////
//Estructura que permite representar un histograma, considerando un arreglo de bins, y su largo.
typedef struct histograma {
  bin *arregloBin;
  int largo;
}histograma;
/////////////////////////////////////////////////////////////////////
//Estructura que permite almacenar una imagen, es decir, una matriz de pixeles,
//el orden y el nivel en el cual está dicha imagen
typedef struct matrizPixel{
    pixel** matriz;
    int orden;
    int nivel;  
}matrizPixel;
/////////////////////////////////////////////////////////////////////
//Variables globales
int niveles, bins;

/////////////////////////////////////////////////////////////////////

void imprimirMatriz(matrizPixel* img){
    for(int i = 0; i < img->orden;i++){
        for(int j = 0; j < img->orden;j++){
            //printf("%d ",img->matriz[i][j]);
            printf("[ %d %d %d ]",img->matriz[i][j].r,img->matriz[i][j].g,img->matriz[i][j].b);
        }
        printf("\n");
    }
}

void imprimirHistograma(histograma* hist){
    for(int i = 0; i < hist->largo; i++){
        printf("[%d,%d]\t%d\n",hist->arregloBin[i].inferior,hist->arregloBin[i].superior,hist->arregloBin[i].valorI);
    }
}

void liberarHist(void** hist){
    free(((histograma*)*hist)->arregloBin);
    free((histograma*)*hist);
}

void liberarMatriz(matrizPixel** matriz){
    for(int i = 0; i < (*matriz)->orden;i++){
        free((*matriz)->matriz[i]);
    }
    free((*matriz)->matriz);
    free((*matriz));
}
//descripción: permite capturar los datos de una imagen con formato BMP.
//entrada: un string que representa el nombre del archivo que contiene la imagen.
//salida: una estructura de tipo matrizPixel.
matrizPixel* cargarImagen(char *filename){
    FILE *f;
    bmpFileHeader header;
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

    fseek(f,header.imageDataOffset,SEEK_SET);
    pixel** matrizPixel = (pixel**)malloc(sizeof(pixel*)*largoImagen);
    for(int i = 0; i < largoImagen;i++){
        pixel* filaPixel = (pixel*)malloc(sizeof(pixel)*largoImagen);
        for(int j = 0;j < largoImagen;j++){
            fread(&filaPixel[j], 1,sizeof(pixel), f);
            //printf("%d %d %d|",filaPixel[j].r,filaPixel[j].g,filaPixel[j].b); //Para printear los píxeles (debug).
        }
        matrizPixel[i] = filaPixel;
        //printf("\n");
    }
    imagenOriginal->matriz=matrizPixel;
    imagenOriginal->orden=largoImagen;
    fclose(f);
    return imagenOriginal;
}

//descripción: obtiene el primer cuadrante de una estructura de tipo matrizPixel.
//entrada: una estructura de tipo matrizPixel.
//salida: una estructura de tipo matrizPixel que representa el cuadrante 1 de la imgen entregada como entrada.
matrizPixel* cuadrante1(matrizPixel* img){
    matrizPixel* nuevaImg= (matrizPixel *)malloc(sizeof(matrizPixel));
    nuevaImg->nivel = img -> nivel + 1;
    
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

//descripción: obtiene el segundo cuadrante de una estructura de tipo matrizPixel.
//entrada: una estructura de tipo matrizPixel.
//salida: una estructura de tipo matrizPixel que representa el cuadrante 2 de la imgen entregada como entrada.
matrizPixel* cuadrante2(matrizPixel* img){
    matrizPixel* nuevaImg= (matrizPixel *)malloc(sizeof(matrizPixel));
    nuevaImg->nivel = img -> nivel + 1;
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

//descripción: obtiene el tercer cuadrante de una estructura de tipo matrizPixel.
//entrada: una estructura de tipo matrizPixel.
//salida: una estructura de tipo matrizPixel que representa el cuadrante 3 de la imgen entregada como entrada.
matrizPixel* cuadrante3(matrizPixel* img){
    matrizPixel* nuevaImg= (matrizPixel *)malloc(sizeof(matrizPixel));
    nuevaImg->nivel = img -> nivel + 1;
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

//descripción: obtiene el cuarto cuadrante de una estructura de tipo matrizPixel.
//entrada: una estructura de tipo matrizPixel.
//salida: una estructura de tipo matrizPixel que representa el cuadrante 4 de la imgen entregada como entrada.
matrizPixel* cuadrante4(matrizPixel* img){
    matrizPixel* nuevaImg= (matrizPixel *)malloc(sizeof(matrizPixel));
    nuevaImg->nivel = img -> nivel + 1;
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

//descripción: permite generar un histograma en base a la cantidad de bins entregada.
//entrada: un entero que representa la cantidad de bins.
//salida: una estructura de tipo histograma.
histograma* generarHistogramaInicial(){
    histograma* hist = (histograma*)malloc(sizeof(histograma));
    int largoBin = 256/bins;
    bin* binRel = (bin*)malloc(sizeof(bin)*bins);
    for(int i = 0; i < bins; i++){
        binRel[i].inferior = i*largoBin;
        binRel[i].superior = largoBin*(i+1) - 1;
        binRel[i].valorI = 0;
    }
    hist->arregloBin = binRel;
    hist->largo = bins;
    return hist;
}

//descripción: suma los resultados de 4 histogramas.
//entrada: 4 void puntero que corresponden a los 4 histogramas.
//salida: un void puntero que representa el histograma final, luego que haber sumado los 4 histogramas.
void* sumarHistograma(void* hist1,void* hist2,void* hist3, void* hist4){
    histograma* cast1 = (histograma*)hist1;
    histograma* cast2 = (histograma*)hist2;
    histograma* cast3 = (histograma*)hist3;
    histograma* cast4 = (histograma*)hist4;
    histograma* final = generarHistogramaInicial();
    for(int i = 0; i < bins;i++){
        final->arregloBin[i].valorI = cast1->arregloBin[i].valorI + cast2->arregloBin[i].valorI + cast3->arregloBin[i].valorI + cast4->arregloBin[i].valorI;
    }
    return (void*) final;
}

//descripción: obtiene el histograma de una determinada porción de imagen.
//entrada: un void puntero que corresponde a la porción de imagen.
//salida: un void puntero que corresponde al histograma.
void* obtenerHistograma(void *img){
    //printf("\n ******* Aquí se encarga de calcular el histograma para este cuadrante: *******\n");
    //Se tranforma la imagen a Matriz de Grises
    matrizPixel* imagen = (matrizPixel*)img;
    histograma* hist = generarHistogramaInicial();
    int grisRelativo;
    //imprimirMatriz(imagen);
    //printf("Orden de img: %d",imagen->orden);
    
    for(int i = 0; i < imagen->orden; i++){
        for(int j = 0; j < imagen->orden; j++){
            grisRelativo = imagen->matriz[i][j].r*0.3 + imagen->matriz[i][j].g*0.59 + imagen->matriz[i][j].b*0.11;
            //printf("%d ", grisRelativo);
            //printf("%d %d %d|",imagen->matriz[i][j].r,imagen->matriz[i][j].g,imagen->matriz[i][j].b);
            for(int k = 0; k < hist->largo;k++){
                if(grisRelativo >= hist->arregloBin[k].inferior && grisRelativo <= hist->arregloBin[k].superior){
                    hist->arregloBin[k].valorI = hist->arregloBin[k].valorI + 1;
                    //break
                    k = hist->largo;
                }
            }   
        }
    }
    return (void*) hist;
}

//descripción: genera las 4 hebras y los cuatro cuadrantes de una imagen o porción de imagen.
//entrada: un void puntero que corresponde a la imagen o porción de imagen.
//salida: un void puntero que corresponde al histograma calculado.
void* generadoraHebras(void *img){
    pthread_t h1;
    pthread_t h2;
    pthread_t h3;
    pthread_t h4;

    void *status1,*status2,*status3,*status4;

    matrizPixel* imgPrincipal = (matrizPixel*) img;

    //Si los cuadrantes no están en el nivel máximo de descomposición, entonces se generan 4 hebras por cada cuadrante.
    //Recordar que la función generadora de Hebras de por sí genera 4 hebras, y es por ello que cada hebra invoca a esta función de manera recursiva.
    if (imgPrincipal->nivel != niveles){
        //printf("\n entra al if?");
        matrizPixel* subImg1 = (matrizPixel*)malloc(sizeof(matrizPixel));
        matrizPixel* subImg2 = (matrizPixel*)malloc(sizeof(matrizPixel));
        matrizPixel* subImg3 = (matrizPixel*)malloc(sizeof(matrizPixel));
        matrizPixel* subImg4 = (matrizPixel*)malloc(sizeof(matrizPixel));
        subImg1 = cuadrante1(imgPrincipal);
        subImg2 = cuadrante2(imgPrincipal);
        subImg3 = cuadrante3(imgPrincipal);
        subImg4 = cuadrante4(imgPrincipal);

        liberarMatriz(&imgPrincipal);
        pthread_create(&h1,NULL,generadoraHebras,(void*) subImg1);
        pthread_create(&h2,NULL,generadoraHebras,(void*) subImg2);
        pthread_create(&h3,NULL,generadoraHebras,(void*) subImg3);
        pthread_create(&h4,NULL,generadoraHebras,(void*) subImg4);

        pthread_join(h1,&status1);
        pthread_join(h2,&status2);
        pthread_join(h3,&status3);
        pthread_join(h4,&status4);
    }
    else
    {
        //Calcula histograma de la subimagen
        return obtenerHistograma((void*) imgPrincipal);
    }
    
    histograma* retorno = sumarHistograma(status1,status2,status3,status4);
    liberarHist(&status1);
    liberarHist(&status2);
    liberarHist(&status3);
    liberarHist(&status4);
    return retorno;
}

//descripción: permite verificar si un número es potencia de 2.
//entrada: un valor entero.
//salida: 1 si el valor ingresado es potencia de 2, o 0 si no lo es.
int comprobarPot2(int num){
    if(num == 0){
        return 1;
    }
    int res=2;
    if(num == res){
        return 1;
    }
    while(res != num){
        res=2*res;
        if(res == num){
            return 1;
        }
        if (res > num){
            return 0;
        }
    }
    return 0;
}

//descripción: permiter obtener una potencia.
//entrada: dos números enteros. 
//salida: un entero que representa la potencia.
int potencia(int num1,int num2){
  if(num1 == 1){
    return 1;
  }
  if(num2 == 0){
    return 1;
  }
  return num1*potencia(num1,num2-1);
}

//descripción: Esta función calcula el logaritmo en base dos del parámetro entrante (orden). Sirve para
//simplificar la condición de si la imagen es más chica que los cuadrantes resultantes por los niveles.
//entrada: Un entero que representa el número a calcular (en este caso siempre será el orden de una imagen o matriz).
//salida: Un entero que representa su logaritmo en base 2.
int calcularOrdenDisminuido(int orden){
    int x = 0;
    while(orden != 1){
        orden = orden/2;
        x++;
        //printf("%d\n",orden);
    }
    return x;
}

//descripción: Esta función valida que la cantidad total de pixeles (dimesiónxdimesión) de la imagen es mayor o igual
//a la cantidad de cuadrantes generados por los niveles.
//entrada: Un entero que representa el orden de la imagen leída.
//Salida: un entero que tira 1 en el caso de ser verdadera la condición (nivel ingresado inválido) y 0 si es falsa
//la condición (nivel ingresado válido con la imagen).
int calcularCondicionPixel(int orden){
    return niveles >= calcularOrdenDisminuido(orden) && potencia(2, 2*niveles - calcularOrdenDisminuido(orden)) > orden;
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
		printf("Se ingresó un número incorrecto de argumentos.\n");
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
            if(optarg[0] < '0' || optarg[0] > '9'){
                printf("Ponga un número entero válido para el parámetro L\n");
                imprimirError(argv);
                exit(EXIT_FAILURE);
            }
		    else if(optarg!=0 && N<0){//si no se ingresa un argumento junto a -n o si no se logra parsear el argumento ingresado, se considera como invalido
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
            if(optarg[0] < '0' || optarg[0] > '9'){
                printf("Ponga un número entero válido para el parámetro B\n");
                imprimirError(argv);
                exit(EXIT_FAILURE);
            }
		    else if(optarg!=0 && N2 <1 ){//si no se ingresa un argumento junto a -c o si no se logra parsear el argumento ingresado, se considera como invalido
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

//descripción: permite escribir el histograma en el archivo de salida.
//entrada: el nombre del archivo y el histograma final.
//salida: como es de tipo void, no tiene salida.
void escribirArchivo (char *archivo, histograma* hist){
    FILE* arch = fopen(archivo,"w");
    for(int i = 0; i < bins; i++){
        fprintf(arch,"[%d,   ",hist->arregloBin[i].inferior);
        fprintf(arch,"%d]    ",hist->arregloBin[i].superior);
        fprintf(arch,"%d",hist->arregloBin[i].valorI);
        fprintf(arch,"\n");
    }
}

int main(int argc, char *argv[]){
	pthread_t h1;
    void *resultadoFinal;
    histograma* histogramaFinal;

    char *nombreArchivo, *salida;
    
    //Se reciben atributos por el getOPT
	recibirArgumentos(argc, argv, &nombreArchivo, &salida, &niveles, &bins);
    printf("El argumento de flag -i es: %s\n", nombreArchivo);
    printf("El argumento de flag -o es: %s\n", salida);
    printf("El argumento de flag -L es: %d\n", niveles);
    printf("El argumento de flag -B es: %d\n", bins);



    matrizPixel* imagenOriginal = (matrizPixel*)malloc(sizeof(matrizPixel));
    imagenOriginal -> nivel = 0;
    imagenOriginal = cargarImagen(nombreArchivo);

    //Validación niveles
    printf("%d\n\n",calcularCondicionPixel(imagenOriginal->orden));
    if(calcularCondicionPixel(imagenOriginal->orden)){
       printf("La cantidad de cuadrantes generados con %d niveles son más que las dimensiones de la imagen\n",niveles);
       printf("(%dx%d)\n",imagenOriginal->orden,imagenOriginal->orden); 
       exit(1);
    }

    //Crear primera hebra
    pthread_create(&h1, NULL, generadoraHebras, (void *) imagenOriginal);
    pthread_join(h1,&resultadoFinal);

    //Capturar el histograma armado
    histogramaFinal = (histograma*) resultadoFinal;

    escribirArchivo(salida,histogramaFinal);
    imprimirHistograma(histogramaFinal);
	return 0;
}   
