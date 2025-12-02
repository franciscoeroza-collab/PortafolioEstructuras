#ifndef HEAP_H
#define HEAP_H

#include "../arbol/arbol.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX 1
#define MIN 0

#define HIJO_IZQUIERDO(p) (2 * (p) + 1)
#define HIJO_DERECHO(p) (2 * (p) + 2)
#define PADRE(i) ((i - 1) / 2) 

typedef struct
{
	NodoA **arr;
	int cantidad;
	Arbol arbol;
	void (*imprimir)(void*);
	int (*comparar)(void*,void*);
	int tipo;
}Heap;

Heap inicializarHeap(void (*imprimir)(void*),int (*comparar)(void*,void*) );
void insertarHeap(Heap *heap, void *dato);
void* quitarHeap(Heap *heap);

void heapify(Heap *heap);
void heapifyUp(Heap *heap, int index);
void heapifyDown(Heap *heap, int index); 

void actualizarArbol(Heap *heap);
void imprimirHeap(Heap heap);

// Funciones auxiliares
void intercambiarNodos(NodoA **a, NodoA **b);
int siCambiar(Heap *heap, int indice, int hijo);
#endif