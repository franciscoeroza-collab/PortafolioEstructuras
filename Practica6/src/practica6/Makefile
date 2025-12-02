CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -O2

# Directorios de includes
INCLUDES = -I ../Estructuras/arbol -I ../captura

# Directorio de librerías
LIBDIR = ../../lib

# Librerías
LIBS = -L $(LIBDIR) -lArbol -lCaptura

# Archivos fuente
SRC = $(wildcard *.c)

# Ejecutable final
OUT = ../../bin/practica6.exe

# Regla principal
build:
	@echo Compilando practica 6...
	@if not exist "..\..\bin" mkdir "..\..\bin"
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(INCLUDES) $(LIBS)
	@echo Ejecutable creado en bin/practica6.exe

# Limpieza
clean:
	del /Q *.o 2>nul
	@echo Objetos eliminados