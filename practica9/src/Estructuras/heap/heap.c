#include "heap.h"
#include <stdlib.h> 
#include <stdio.h>

void intercambiarNodos(NodoA **a, NodoA **b)
{
	NodoA *temp = *a;
	*a = *b;
	*b = temp;
}

// Retorna 1 si el elemento 'hijo' debe ser priorizado sobre 'indice'
int siCambiar(Heap *heap, int indice, int hijo)
{
	// aqui comparo y devuelv0 menor a 0 si hijo tiene menos páginas o mayor a 0 si hijo tiene más páginas
	
	if (heap->tipo == MAX)
	{
		return heap->comparar(heap->arr[hijo]->dato, heap->arr[indice]->dato) > 0;
	}
	else
	{
		return heap->comparar(heap->arr[hijo]->dato, heap->arr[indice]->dato) < 0;
	}
}

void heapifyDown(Heap *heap, int index)
{
	int max_o_min = index; 

	while (1)
	{
		int hi = HIJO_IZQUIERDO(index);
		int hd = HIJO_DERECHO(index);
		max_o_min = index;

		// comparar con el hijo izquierdo
		if (hi < heap->cantidad && siCambiar(heap, max_o_min, hi))
		{
			max_o_min = hi;
		}

		// comparar con el hijo derecho
		if (hd < heap->cantidad && siCambiar(heap, max_o_min, hd))
		{
			max_o_min = hd;
		}

		if (max_o_min != index)
		{
			intercambiarNodos(&heap->arr[index], &heap->arr[max_o_min]);
			index = max_o_min;
		}
		else
		{
			break;
		}
	}
}

void heapifyUp(Heap *heap, int index)
{
	int padre = PADRE(index);

	while (index > 0 && siCambiar(heap, padre, index))
	{
		intercambiarNodos(&heap->arr[index], &heap->arr[padre]);
		index = padre;
		padre = PADRE(index);
	}
}

// Reconstruye todo el Heap
void heapify(Heap *heap)
{
	// Recorre desde el último padre hacia la raíz (0)
	for (int p = heap->cantidad / 2 - 1; p >= 0; p--)
	{
		heapifyDown(heap, p);
	}
	actualizarArbol(heap);
}

Heap inicializarHeap(void (*imprimir)(void*),int (*comparar)(void*,void*) )
{
	Arbol arbol = (Arbol){NULL,0,imprimir,NULL,NULL};
	return (Heap){NULL,0,arbol,imprimir,comparar,MIN};
}

void insertarHeap(Heap *heap, void *dato)
{
	NodoA **aux;
	heap->cantidad++;
	
	aux = realloc(heap->arr,sizeof(NodoA*)*heap->cantidad);
	if(aux == NULL) {
		fprintf(stderr, "Fallo al reasignar memoria en insertarHeap\n");
		heap->cantidad--;
		return;
	}
	
	heap->arr = aux;
	heap->arr[heap->cantidad-1] = crearNodoA(dato); 
	
	heapifyUp(heap, heap->cantidad-1);
	actualizarArbol(heap); 
}

void* quitarHeap(Heap *heap)
{
	if(heap->cantidad == 0)
		return NULL;
	
	//Guardar dato y nodo a liberar
	void *dato = heap->arr[0]->dato;
	NodoA *nodo_a_liberar = heap->arr[0];
	
	//Mover el último elemento a la raíz
	heap->arr[0] = heap->arr[heap->cantidad-1];
	heap->cantidad--;
	
	//Liberar el nodo antiguo
	free(nodo_a_liberar); 
	
	//Reducir el tamaño del arreglo
	if(heap->cantidad > 0)
	{
		NodoA **aux = realloc(heap->arr,sizeof(NodoA*)*heap->cantidad);
		if(aux != NULL) { heap->arr = aux; }
		heapifyDown(heap, 0); 
	}
	else
	{
		free(heap->arr);
		heap->arr = NULL;
	}
	
	actualizarArbol(heap);
	return dato;
}

void actualizarArbol(Heap *heap)
{
	int i,hi,hd;
	for( i = 0 ; i < heap->cantidad ; i++)
	{
		hi = HIJO_IZQUIERDO(i);
		hd = HIJO_DERECHO(i);
		heap->arr[i]->izq = (hi<heap->cantidad) ? heap->arr[hi]:NULL;
		heap->arr[i]->dch = (hd<heap->cantidad) ? heap->arr[hd]:NULL;
	}
	if(heap->arr && heap->cantidad > 0)
	{
		heap->arbol.raiz = heap->arr[0];
		heap->arbol.cantidad = heap->cantidad;
	}
	else 
	{
		heap->arbol.raiz = NULL;
		heap->arbol.cantidad = 0;
	}
}


void imprimirHeap(Heap heap)
{
	printf("[");
	for( int i = 0; i< heap.cantidad ; i++)
	{
		heap.imprimir( heap.arr[i]->dato);
		if (i < heap.cantidad - 1) printf(", ");
	}
	printf("]");
}