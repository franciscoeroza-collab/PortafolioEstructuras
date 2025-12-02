#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include "Nodo.h"

typedef struct
{
	Nodo *inicio;
	int (*comparar)(void*,void*);	
	void (*imprimir)(void*);
	int cant;		
}Lista;

void mostrarLista(Lista);
void insertarFinal(Lista*,void*);
void borrarLista(Lista *);
void borrarDato(Lista *lista,void* dato);
void* buscarDato(Lista lista,void *dato);

void insertarOrdenado(Lista *lista,void* dato);
void insertarInicio(Lista *lista,void* dato);
void insertarEnPosicion(Lista *lista,void* dato,int pos);
void eliminarEnPosicion(Lista *lista,int pos);

#endif