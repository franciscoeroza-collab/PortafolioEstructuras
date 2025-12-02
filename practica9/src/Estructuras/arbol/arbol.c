#include "arbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inicializarArbol(Arbol *arbol, void (*imprimir)(void*), int (*comparar)(void*,void*), void (*liberar)(void*))
{
	arbol->raiz = NULL;
	arbol->cantidad = 0;
	arbol->imprimir = imprimir;
	arbol->comparar = comparar;
	arbol->liberar = liberar;
}

void destruirNodos(NodoA *raiz, void (*liberar)(void*))
{
	if (!raiz)
		return;
	
	destruirNodos(raiz->izq, liberar);
	destruirNodos(raiz->dch, liberar);
	
	if (liberar && raiz->dato)
		liberar(raiz->dato);
	
	free(raiz);
}

void destruirArbol(Arbol *arbol)
{
	destruirNodos(arbol->raiz, arbol->liberar);
	arbol->raiz = NULL;
	arbol->cantidad = 0;
}

void insertarArbolOrdenado(NodoA *raiz,void *dato,int (*comparar)(void*,void*))
{
	if(comparar(dato,raiz->dato)<=0)
	{
		// IZQUIERDA (Menor o igual)
		if(raiz->izq)
			insertarArbolOrdenado(raiz->izq,dato,comparar);
		else
			raiz->izq = crearNodoA(dato);
	}
	else
	{
		// DERECHA (Mayor)
		if(raiz->dch)
			insertarArbolOrdenado(raiz->dch,dato,comparar);
		else
			raiz->dch = crearNodoA(dato);
	}
}

void insertarArbol(Arbol *arbol,void *dato)
{
	if(!arbol->raiz)
		arbol->raiz = crearNodoA(dato);
	else 
		insertarArbolOrdenado(arbol->raiz,dato,arbol->comparar);
	arbol->cantidad++;
}

// funciones de recorrido e impresion

void imprimir_arbol(NodoA* nodo, int nivel,void (*imprimir)(void*))
{
    int i;
    if (nodo != NULL)
    {
        printf(" ");
        imprimir(nodo->dato);
        
        if(nodo->dch)
        {
            printf("\n");
            for (i = 0; i < nivel+1; i++)
            {
                if(i==nivel)
                    printf(" |____R ");
                else
                    printf(" |      ");
            }
            imprimir_arbol(nodo->dch, nivel + 1,imprimir);
        }
        
        if(nodo->izq)
        {
            printf("\n");
            for (i = 0; i < nivel+1; i++)
            {
                printf(" |      ");
            }
            printf("\n");
            for (i = 0; i < nivel+1; i++)
            {
                if(i==nivel)
                    printf(" |____L ");
                else
                    printf(" |      ");
            }
            imprimir_arbol(nodo->izq, nivel + 1,imprimir);
        }
    }
}

void imprimirArbol(Arbol arbol)
{
	imprimir_arbol(arbol.raiz,0,arbol.imprimir);
}

void preorden(NodoA *raiz,void (*imprimir)(void*))
{
	if(!raiz) return;
	printf(" ");
	imprimir(raiz->dato);
	preorden(raiz->izq,imprimir);
	preorden(raiz->dch,imprimir);
}
 
void orden(NodoA *raiz,void (*imprimir)(void*))
{
	if(!raiz) return;
	orden(raiz->izq,imprimir);
	printf(" ");
	imprimir(raiz->dato);
	orden(raiz->dch,imprimir);	
}

void inverso(NodoA *raiz,void (*imprimir)(void*))
{
	if(!raiz) return;
	inverso(raiz->dch,imprimir);	
	printf(" ");
	imprimir(raiz->dato);
	inverso(raiz->izq,imprimir);
}

void postorden(NodoA *raiz,void (*imprimir)(void*))
{
	if(!raiz) return;
	postorden(raiz->izq,imprimir);
	postorden(raiz->dch,imprimir); 
	printf(" ");
	imprimir(raiz->dato);	
}

void imprimirOrden(Arbol arbol,int opcion)
{
	switch(opcion)
	{
		case PREORDEN: 
			preorden(arbol.raiz,arbol.imprimir);
			break;
		case ORDEN: 
			orden(arbol.raiz,arbol.imprimir);
			break;
		case INVERSO: 
			inverso(arbol.raiz,arbol.imprimir);
			break;
		case POSTORDEN: 
			postorden(arbol.raiz,arbol.imprimir);
			break;
	}
}

int alturaArbol(NodoA *raiz)
{
	if (!raiz)
		return 0;

	int altIzq = alturaArbol(raiz->izq);
	int altDch = alturaArbol(raiz->dch);

	return (altIzq > altDch ? altIzq : altDch) + 1;
}

int calcularProfundidad(Arbol arbol)
{
	return alturaArbol(arbol.raiz);
}

bool compararNodos(NodoA *n1, NodoA *n2, int (*comparar)(void*, void*))
{
	// Caso base: ambos son NULL (iguales en estructura)
	if (!n1 && !n2)
		return true;
	
	// Si uno es NULL y el otro no (diferentes en estructura)
	if (!n1 || !n2)
		return false;

	// Compara los datos del nodo actual
	if (comparar(n1->dato, n2->dato) != 0)
		return false;

	// Compara recursivamente subárboles izquierdo y derecho
	return compararNodos(n1->izq, n2->izq, comparar) && 
	       compararNodos(n1->dch, n2->dch, comparar);
}

bool compararSoloDatos(NodoA *raiz, Arbol *otroArbol, int (*comparar)(void*, void*))
{
    if (!raiz)
        return true;
        
    NodoA *actual = otroArbol->raiz;
    while(actual)
    {
        int res = comparar(raiz->dato, actual->dato);
        if (res == 0)
            break; // Encontrado
        else if (res < 0)
            actual = actual->izq;
        else
            actual = actual->dch;
    }
    
    if (!actual)
        return false; // Dato no encontrado
        
    // Continuar con los subárboles
    return compararSoloDatos(raiz->izq, otroArbol, comparar) && 
           compararSoloDatos(raiz->dch, otroArbol, comparar);
}

bool compararArboles(Arbol a, Arbol b, bool *soloDatos)
{
	*soloDatos = false;
	
	if (a.cantidad != b.cantidad)
		return false; // si no tienen la misma cantidad, no pueden ser iguales

	if (compararNodos(a.raiz, b.raiz, a.comparar))
	{
		return true;
	}

	// verifica si es igual solo en los datos. Arriba si tiene la misma estructura y datos
	if (compararSoloDatos(a.raiz, &b, a.comparar))
	{
		*soloDatos = true;
		return false; 
	}

	return false; // No es igual en nada
}

NodoA **buscarSucesor(NodoA **pRaiz)
{
	NodoA **act = &(*pRaiz)->dch;
	
	while((*act)->izq)
		act = &(*act)->izq;
	
	return act;
}

bool eliminarNodo(NodoA **pRaiz, void *dato, int (*comparar)(void*,void*), void (*liberar)(void*))
{
	if (!*pRaiz)
		return false;

	int res = comparar(dato, (*pRaiz)->dato);

	if (res < 0)
	{
		return eliminarNodo(&(*pRaiz)->izq, dato, comparar, liberar);
	}
	else if (res > 0)
	{
		return eliminarNodo(&(*pRaiz)->dch, dato, comparar, liberar);
	}
	else
	{
		NodoA *aEliminar = *pRaiz;
		
		if (!aEliminar->izq && !aEliminar->dch) //nodo hoja
		{
			*pRaiz = NULL;
		}
		else if (!aEliminar->izq) //modo con solo hijo derecho
		{
			*pRaiz = aEliminar->dch;
		}
		else if (!aEliminar->dch) //nodo con solo hijo izquierdo
		{
			*pRaiz = aEliminar->izq;
		}
		else // reemplazar con sucesor si tienes dos hijos
		{
			NodoA **pSucesor = buscarSucesor(&aEliminar);
			NodoA *sucesor = *pSucesor;

			*pSucesor = sucesor->dch; 
			
			sucesor->izq = aEliminar->izq;
			sucesor->dch = aEliminar->dch;
			*pRaiz = sucesor;
		}
		
		// Liberar memoria
		if (liberar)
			liberar(aEliminar->dato);
		free(aEliminar);
		
		return true;
	}
}

bool eliminarDatoArbol(Arbol *arbol, void *dato)
{
	bool eliminado = eliminarNodo(&arbol->raiz, dato, arbol->comparar, arbol->liberar);
	if (eliminado)
		arbol->cantidad--;
	return eliminado;
}

// equilibrio

void arbolOrdenar(NodoA *raiz, void **array, int *i)
{
	if (!raiz)
		return;

	// Guardar en orden (izquierda, raíz, derecha)
	arbolOrdenar(raiz->izq, array, i);
	
	// Mover el dato y liberar el nodo (no el dato)
	array[*i] = raiz->dato;
	(*i)++;
	
	arbolOrdenar(raiz->dch, array, i);
	
	// Liberar el nodo, el dato se guardó en el array
	free(raiz);
}

NodoA* reconstruirArbol(void **array, int inicio, int fin, Arbol *arbol)
{
	if (inicio > fin)
		return NULL;

	// aqui encuentro el elemento medio
	int medio = inicio + (fin - inicio) / 2;
	NodoA *raiz = crearNodoA(array[medio]);

	// Construir recursivamente los subárboles
	raiz->izq = reconstruirArbol(array, inicio, medio - 1, arbol);
	raiz->dch = reconstruirArbol(array, medio + 1, fin, arbol);

	return raiz;
}

void equilibrarArbol(Arbol *arbol)
{
	if (!arbol->raiz || arbol->cantidad <= 1)
		return;
	
	// guardo en una lista, destruyo nodos pero no losd atos
	void **datosOrdenados = (void**)malloc(arbol->cantidad * sizeof(void*));
	if (!datosOrdenados)
	{
		fprintf(stderr, "Error de asignación de memoria para equilibrado.\n");
		return;
	}
	
	int i = 0;

	arbolOrdenar(arbol->raiz, datosOrdenados, &i);
	arbol->raiz = NULL; 
	
	// Reconstruyo el arbol
	arbol->raiz = reconstruirArbol(datosOrdenados, 0, arbol->cantidad - 1, arbol);

	free(datosOrdenados);
	
	printf("Árbol equilibrado exitosamente. Nueva altura: %d\n", calcularProfundidad(*arbol));
}