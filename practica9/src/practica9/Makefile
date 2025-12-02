cflags   = -Wall -Werror -pedantic -std=c99 -O2
dflags   = -Wall -Werror -pedantic -std=c99 -g

# Includes
includes = -I ../../src/captura \
           -I ../../src/Estructuras/pila \
           -I ../../src/Estructuras/cola \
           -I ../../src/Estructuras/arbol \
           -I ../../src/Estructuras/heap

# Librerías
libs = -L ../../lib -lCaptura -lPila -lCola -lArbol -lHeap

# Archivos
source = $(wildcard *.c)
output = ../../bin/practica9

build:
	gcc $(cflags) $(source) -o $(output) $(includes) $(libs)
	@echo "Compilado Exitosamente!"

debug:
	gcc $(dflags) $(source) -o $(output)_debug $(includes) $(libs)
	@echo "Compilado en modo DEBUG"