CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -O2

# Objetos que deben ir en la librería
OBJ = arbol.o ../nodos/nodoarbol.o

all: $(OBJ)
	ar rcs ../../../lib/libArbol.a $(OBJ)
	@echo "Libreria libArbol.a creada correctamente en ../../../lib"

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o
