#ifndef ARBOL_H
#define ARBOL_H

#include "../nodos/nodoarbol.h"
#include <stdio.h>
#include <stdbool.h>

#define PREORDEN 1
#define ORDEN 2
#define POSTORDEN 3
#define INVERSO 4

typedef struct
{
	NodoA *raiz;
	int cantidad;
	void (*imprimir)(void*);
	int (*comparar)(void*,void*);
	void (*liberar)(void*);
}Arbol;

void inicializarArbol(Arbol *arbol, void (*imprimir)(void*), int (*comparar)(void*,void*), void (*liberar)(void*));
void insertarArbol(Arbol *arbol,void *dato);
void imprimirArbol(Arbol arbol);
void imprimirOrden(Arbol arbol,int opcion);

bool compararArboles(Arbol a, Arbol b, bool *soloDatos);
int calcularProfundidad(Arbol arbol);
bool eliminarDatoArbol(Arbol *arbol, void *dato);
void equilibrarArbol(Arbol *arbol);
void destruirArbol(Arbol *arbol);

#endif