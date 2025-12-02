#ifndef NODO_ARBOL
#define NODO_ARBOL
#include <stdlib.h>

typedef struct nodo
{
	struct nodo *izq;
	struct nodo *dch;
	void *dato;
}NodoA;

NodoA* crearNodoA(void *dato);

#endif