#ifndef PILA_H
#define PILA_H

#include "../nodos/nodo.h"

typedef struct
{
	Nodo *cima;
	int capacidad;
	int cantidad;
	void (*imprimir)(void*); //SE ASIGNA LA FUNCION PARA IMPRIMIR LOS DATOS
	void (*liberar)(void*); //SE ASIGNA LA FUNCION PARA LIBERAR DATOS,SI SE NECESITA
}Pila;


int llena(Pila pila);
int vacia(Pila pila);
void push(Pila *pila,void *dato);
void* pop(Pila *pila);
void imprimirPila(Pila pila);
void imprimirPilaInvertida(Pila pila);
void eliminarPila(Pila *pila);
#endif