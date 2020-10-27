import random as rnd
largoLineas = 64

def randomizadorADN():
    var = rnd.randint(1,4)
    if var == 1:
        return 'A'
    elif var == 2:
        return 'C'
    elif var == 3:
        return 'G'
    else:
        return 'T'
def crearLineaRandom():
    i = 0
    resul = ""
    while i < largoLineas:
        resul += randomizadorADN()
        i += 1
    return resul

def escribirArchivo(cantLineas,nombre):
    arch = open(nombre + ".txt","w")
    i = 0
    while i < cantLineas:
        arch.write(crearLineaRandom() + "\n")
        i += 1
    arch.close()
    return

def main():
    nombreArchivo = input("Ingrese el nombre del archivo (sin extensión): ")
    largo = int(input("Ingrese la cantidad de lineas: "))
    escribirArchivo(largo,nombreArchivo)
    print("Éxito")
    return


main()
        
