cflags   = -Wall -Werror -pedantic -std=c99 -O2
dflags   = -Wall -Werror -pedantic -std=c99 -g
includes = -I ../src/captura \
           -I ../src/Estructuras/pila \
           -I ../src/Estructuras/cola \
           -I ../src/Estructuras/arbol
libs     = ../../lib -lCaptura -lPila -lCola -lArbol

source   = $(wildcard *.c)
output   = ../../bin/practica8

build:
	gcc $(cflags) $(source) -o $(output) $(includes) -L $(libs)
	@echo "Compilado Exitosamente!!!"

debug:
	gcc $(dflags) $(source) -o $(output)_debug $(includes) -L $(libs)
	@echo "Compilado en modo DEBUG!"