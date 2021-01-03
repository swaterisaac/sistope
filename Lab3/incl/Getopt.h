#ifndef GETOPT_H
#define GETOPT_H
void imprimirError(char *argv[]);
void recibirArgumentos(int argc, char *argv[], char **nombreArchivo, char **salida, int *ancho, int *cantDiscos, int *tamBuffer, int *flag);
#endif