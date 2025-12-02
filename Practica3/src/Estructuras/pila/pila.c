#include "pila.h"
#include <stdio.h>
int llena(Pila pila)
{
	return pila.cantidad == pila.capacidad;
}
int vacia(Pila pila)
{
	return pila.cantidad == 0 && pila.cima==NULL ;
}
void push(Pila *pila,void *dato)
{
	if(llena(*pila))
	{
		printf("\n Overflow");
	}
	else
	{
		Nodo *nuevo = crearNodo(dato);
		nuevo->sig = pila->cima;
		pila->cima = nuevo;
		pila->cantidad++;
	}
}
void* pop(Pila *pila)
{
	if(vacia(*pila))
	{
		printf("\n Underflow");
		return NULL;
	}
	else
	{
		//DESENCADENAR EL NODO
		Nodo *aux = pila->cima;		
		pila->cima = aux->sig;
		aux->sig = NULL;
		//EXTRAER EL DATO
		void *dato = aux->dato;
		//LIBERAR EL NODO
		free(aux);
		pila->cantidad--;
		return dato;
	}
}

void push_nodo(Pila *pila,Nodo *nuevo)
{
	if(llena(*pila))
	{
		printf("\n Overflow");
	}
	else
	{
		nuevo->sig = pila->cima;
		pila->cima = nuevo;
		pila->cantidad++;
	}
}

Nodo* pop_nodo(Pila *pila)
{
	if(vacia(*pila))
	{
		printf("\n Underflow");
		return NULL;
	}
	else
	{
		//DESENCADENAR EL NODO
		Nodo *aux = pila->cima;		
		pila->cima = aux->sig;
		aux->sig = NULL;
		pila->cantidad--;
		return aux;
	}
}

void imprimirPila(Pila pila)
{
	Pila aux = (Pila){NULL,pila.capacidad,0,NULL};
	Nodo *nodo;
	printf("\n\nPila [%d]:",pila.cantidad);
	while( !vacia(pila) )
	{
		nodo = pop_nodo(&pila);
		printf("\n ");
		pila.imprimir(nodo->dato);
		push_nodo(&aux,nodo);
	}
	while( !vacia(aux) )
		push_nodo(&pila,pop_nodo(&aux));
}
void imprimirPilaInvertida(Pila pila)
{
	Pila aux = (Pila){NULL,pila.capacidad,0,NULL};
	Nodo *nodo;
	printf("\n\nPila Invertida [%d]:",pila.cantidad);
	while( !vacia(pila) )
		push_nodo(&aux,pop_nodo(&pila));
	while( !vacia(aux) )
	{		
		nodo = pop_nodo(&aux);
		printf("\n ");
		pila.imprimir(nodo->dato);
		push_nodo(&pila,nodo);
	}	
}

void eliminarPila(Pila *pila)
{
	void *dato;
	while(!vacia(*pila))
	{
		dato = pop(pila);
		if( pila->liberar )
			pila->liberar(dato);
	}
}
