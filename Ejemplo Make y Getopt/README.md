Uso de getopt, bash y makefile simple.

Primero se debe hacer make en la consola, en la ubicación en que se encuentre tanto el makefile como los archivos .c y .h

Se ejecuta con los flags:
m: no se debe acompañar con nada y puede no estar presente (Ej: -m)
h: se acompaña de un entero positivo (Ej: -h 3)

Ejemplo de uso:
./main -h 5 -m   -> Ejecución correcta con valor de h igual a 5 y flag -m identificado.
./main -h 3      -> Ejecución correcta con valor de h igual a 3 y sin flag -m.
./main           -> Fallo de ejecución, falta argumento -h entero
./main -h        -> Fallo, falta entero que acompaña al flag -h

Dentro de funciones.c se encuentra la función recibirArgumentos(), en la cual se usa getopt(), comprobando y guardando los argumentos en caso de ser necesario.

Signal.h contiene un struct que no se utiliza, pero lo incluyo para que vean como se puede separar un archivo que contenga un struct en el makefile.

Shared.h contiene variables globales que se importan y utilizan en funciones.c

funciones.h contiene las cabeceras de las funciones.

main.c solo tiene el main, en el cual se llaman a funciones definidas en funciones.h y funciones.c



