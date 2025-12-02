#Compilador y banderas
CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -O2

# Directorios
INCLUDES = -I ../Estructuras/cola -I ../captura
LIBDIR = ../../lib

# Librerías a enlazar
LIBS = -L $(LIBDIR) -lCola -lCaptura

# Archivos fuente y salida
SRC = $(wildcard *.c)
OUT = ../../bin/practica5

# Regla principal
build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(INCLUDES) $(LIBS)
	@echo "✅ Compilación exitosa de practica5!"

# Limpieza
clean:
	del *.o
	@echo "🧹 Archivos objeto eliminados"