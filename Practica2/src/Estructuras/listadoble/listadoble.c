#include "listadoble.h"
#include <stdio.h>
 

ListaD inicializarListaD(void)
{
	return (ListaD) {NULL,NULL,0,NULL,NULL,NULL};
}

void borrarLista(ListaD *lista)
{
    NodoD *actual = lista->inicio;
    NodoD *tmp;

    while (actual != NULL) {
        tmp = actual;
        actual = actual->sig;
        free(tmp);
    }

    lista->inicio = NULL;
    lista->fin = NULL; 
    lista->cant = 0;
}

void borrarDato(ListaD *lista, void* dato)
{
    NodoD *actual = lista->inicio;

    while (actual != NULL) {
        if (lista->comparar(dato, actual->dato) == 0) {
            // actualizar enlaces
            if (actual->ant != NULL)
                actual->ant->sig = actual->sig;
            else
                lista->inicio = actual->sig; // borraste el primero

            if (actual->sig != NULL)
                actual->sig->ant = actual->ant;
            else
                lista->fin = actual->ant;    // borraste el último

            //free(actual->dato);
            free(actual);
            lista->cant--;
            return;
        }
        actual = actual->sig;
    }
}

void mostrarListaAdelante(ListaD lista) {
    NodoD *p;
    printf("\nLista adelante (%d elementos):\n", lista.cant);
    for (p = lista.inicio; p != NULL; p = p->sig) {
		printf(" ");
        printf("->");
        lista.imprimir(p->dato);
    }
    printf("\n");
}

void mostrarListaAtras(ListaD lista) {
    NodoD *p = lista.fin; 
    printf("\nLista atrás (%d elementos):\n", lista.cant);
    while (p != NULL) {
        lista.imprimir(p->dato);
        printf(" <- ");
        p = p->ant;
    }
    printf("NULL\n");
}

void* buscarDato(ListaD lista,void *dato)
{
	NodoD *actual;//*anterior=NULL;
	for( actual = lista.inicio ; actual!=NULL ;  actual = actual->sig)
	{
		if( lista.comparar(dato,actual->dato) == 0)
		{
			return actual->dato;
		}
		//anterior = actual;
	}	
	return NULL;	
}

void insertarOrdenado(ListaD *lista, void* dato) {
    NodoD *nuevo = crearNodoD(dato);
    NodoD *actual = lista->inicio;
    NodoD *anterior = NULL;

    while (actual != NULL && lista->comparar(dato, actual->dato) > 0) {
        anterior = actual;
        actual = actual->sig;
    }

    if (anterior == NULL) {
        // insertar al inicio
        nuevo->sig = lista->inicio;
        if (lista->inicio != NULL)
            lista->inicio->ant = nuevo;
        lista->inicio = nuevo;

        if (lista->fin == NULL) // si era el primer nodo
            lista->fin = nuevo;
    } else {
        // insertar entre anterior y actual o al final
        nuevo->sig = actual;
        nuevo->ant = anterior;
        anterior->sig = nuevo;

        if (actual != NULL)
            actual->ant = nuevo;
        else
            lista->fin = nuevo; // caso al final
    }

    lista->cant++;
}