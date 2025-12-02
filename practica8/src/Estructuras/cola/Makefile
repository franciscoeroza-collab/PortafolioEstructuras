# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -O2

# Archivos fuente y objetos
SRC = cola.c nodo.c
OBJ = $(SRC:.c=.o)

# Ruta donde se guardará la librería
LIBDIR = ../../../lib
LIBNAME = libCola.a

# Regla principal: compila los .o y genera la librería estática
build: $(OBJ)
	ar rcs $(LIBDIR)/$(LIBNAME) $(OBJ)
	@echo "✅ Librería $(LIBNAME) creada correctamente en $(LIBDIR)"

# Compilación de objetos
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpieza
clean:
	del *.o
	@echo "🧹 Objetos eliminados"

cleanlib:
	del $(LIBDIR)/$(LIBNAME)
	@echo "🧹 Librería eliminada"
