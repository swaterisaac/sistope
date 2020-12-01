#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
typedef struct bmpFileHeader
{
  
  uint32_t size;        
  uint16_t resv1;       
  uint16_t resv2;      
  uint32_t offset;      
} bmpFileHeader;

typedef struct bmpInfoHeader
{
  uint32_t headersize;    
  uint32_t width;       
  uint32_t height;      
  uint16_t planes;          
  uint16_t bpp;             
  uint32_t compress;      
  uint32_t imgsize;     
  uint32_t bpmx;        
  uint32_t bpmy;       
  uint32_t colors;      
  uint32_t imxtcolors;      
} bmpInfoHeader;

typedef struct __attribute__((__packed__)){

  unsigned char b;
  unsigned char g;
  unsigned char r;
}pixel;
*/

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
  int *arregloBin;
  int largo;
}histograma;

void TextDisplay(bmpInfoHeader *info, unsigned char *img)
{
  int x, y;
  /* Reducimos la resolución vertical y horizontal para que la imagen entre en pantalla */
  static const int reduccionX=1, reduccionY=1;
  /* Si la componente supera el umbral, el color se marcará como 1. */
  static const int umbral=90;
  /* Asignamos caracteres a los colores en pantalla */
  static unsigned char colores[9]=" bgfrRGB";
  int r,g,b;

  /* Dibujamos la imagen */
  for (y=100; y>0; y-=reduccionY)
    {
      for (x=0; x<100; x+=reduccionX)
    {
      b=(img[3*(x+y*info->width)]>umbral);
      g=(img[3*(x+y*info->width)+1]>umbral);
      r=(img[3*(x+y*info->width)+2]>umbral);

      printf("%c", colores[b+g*2+r*4]);
    }
      printf("\n");
    }
}

unsigned char *LoadBMP(char *filename )
{

  FILE *f;
  bmpFileHeader header;     /* cabecera */
  unsigned char *imgdata;   /* datos de imagen */
  uint16_t type;        /* 2 bytes identificativos */
  bmpInfoHeader bInfoHeader;

  f=fopen (filename, "rb");
  if (!f)
    return NULL;        /* Si no podemos leer, no hay imagen*/

  /* Leemos los dos primeros bytes */
  /*
  fread(&type, sizeof(uint16_t), 1, f);
  if (type !=0x4D42)       
    {
      fclose(f);
      return NULL;
    }
  */
  /* Leemos la cabecera de fichero completa */
  fread(&header, sizeof(unsigned char), sizeof(bmpFileHeader), f);

  /* Leemos la cabecera de información completa */
  fread(&bInfoHeader, sizeof(unsigned char), sizeof(bmpInfoHeader), f);
  

  /* Reservamos memoria para la imagen, ¿cuánta?
     Tanto como indique imgsize */
  //imgdata=(unsigned char*)malloc(bInfoHeader->imgsize);

  /* Nos situamos en el sitio donde empiezan los datos de imagen,
   nos lo indica el offset de la cabecera de fichero*/
  //fseek(f, header.offset, SEEK_SET);

  /* Leemos los datos de imagen, tantos bytes como imgsize */
  pixel** matrizPixel = (pixel**)malloc(sizeof(pixel*)*bInfoHeader.height);
  for(int i = 0; i < bInfoHeader.height;i++){

    matrizPixel[i] = (pixel*)malloc(sizeof(pixel)*bInfoHeader.width);
  }
  for(size_t i = 0; i < bInfoHeader.height;i++){
    for(size_t j = 0;j < bInfoHeader.width;j++){
      fread(&matrizPixel[i][j], 1,sizeof(pixel), f);
      printf("%d %d %d|",matrizPixel[i][j].r,matrizPixel[i][j].g,matrizPixel[i][j].b);
    }
    printf("\n");
  }
    
  printf("TAMAÑO: %d x %d\n", bInfoHeader.height, bInfoHeader.width);
  /* Cerramos */
  fclose(f);

  /* Devolvemos la imagen */
  return imgdata;
}

/*void DisplayInfo(bmpInfoHeader *info)
{
  printf("Tamaño de la cabecera: %u\n", info->headersize);
  printf("Anchura: %d\n", info->width);
  printf("Altura: %d\n", info->height);
  printf("Planos (1): %d\n", info->planes);
  printf("Bits por pixel: %d\n", info->bpp);
  printf("Compresión: %d\n", info->compress);
  printf("Tamaño de datos de imagen: %u\n", info->imgsize);
  printf("Resolucón horizontal: %u\n", info->bpmx);
  printf("Resolucón vertical: %u\n", info->bpmy);
  printf("Colores en paleta: %d\n", info->colors);
  printf("Colores importantes: %d\n", info->imxtcolors);
}
*/
int main()
{
  //bmpInfoHeader info;
  //unsigned char *img;

  LoadBMP("negro.bmp");
  //DisplayInfo(&info);
  //TextDisplay(&info, img);

  return 0;
}





