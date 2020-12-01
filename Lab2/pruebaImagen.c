#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

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
} pixel;

typedef struct bin {
  int inferior;
  int superior;
  int valorI;
}bin;

typedef struct histograma {
  bin *arregloBin;
  int largo;
}histograma;

typedef struct matrizPixel{
  pixel** matriz;
  int alturaIMG;
}MP;

typedef struct matrizGris{
  int** matriz;
  int alturaIMG;
}MG;


//Variables globales
MP imagen;




void cargarImagen(char *filename )
{

  FILE *f;
  bmpFileHeader header;
  unsigned char *imgdata;
  uint16_t type;
  bmpInfoHeader bInfoHeader;

  f = fopen (filename, "rb");
  if (!f){
    return NULL;
  }
  
  fread(&header, sizeof(unsigned char), sizeof(bmpFileHeader), f);
  fread(&bInfoHeader, sizeof(unsigned char), sizeof(bmpInfoHeader), f);
  int largoImagen = bInfoHeader.width;
  pixel** matrizPixel = (pixel**)malloc(sizeof(pixel*)*largoImagen);
  for(int i = 0; i < largoImagen;i++){
    pixel* filaPixel = (pixel*)malloc(sizeof(pixel)*largoImagen);
    for(int j = 0;j < largoImagen;j++){
      fread(&filaPixel[j], 1,sizeof(pixel), f);
      //printf("%d %d %d|",matrizPixel[i][j].r,matrizPixel[i][j].g,matrizPixel[i][j].b); //Para printear los píxeles (debug).
    }
    matrizPixel[i] = filaPixel;
    //printf("\n");
  }
  imagen.matriz = matrizPixel;
  imagen.alturaIMG = largoImagen;
  //printf("TAMAÑO: %d x %d\n", bInfoHeader.height, bInfoHeader.width);
  fclose(f);
  return;
}

int comprobarNum(int num){
  if(num == 2){
    return 1;
  }
  else if(num == 0 || num == 1){
    return 0;
  }
  comprobarNum(num/2);
}

int potencia(int num1,int num2){
  if(num1 == 1){
    return 1;
  }
  if(num2 == 0){
    return 1;
  }
  return num1*potencia(num1,num2-1);
}

void calcularDivision(int niveles){
  int altura = imagen.alturaIMG;
  int cuadrantes = potencia(4,niveles);
  
}

int main()
{
  LoadBMP("negro.bmp");
}