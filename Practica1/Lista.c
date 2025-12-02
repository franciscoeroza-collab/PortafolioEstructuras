#include "Lista.h"

void borrarLista(Lista *lista)
{
	Nodo *actual,*anterior = NULL;
	for( actual = lista->inicio ; actual!=NULL ;  )
	{
		anterior = actual;
		actual = actual->sig;
		//free(anterior->dato);
		free(anterior);		
	}
	lista->inicio = NULL;
	lista->cant = 0;		
}

void borrarDato(Lista *lista,void* dato)
{
	Nodo *actual,*anterior = NULL;
	for( actual = lista->inicio ; actual!=NULL ;  actual = actual->sig)
	{
		if(lista->comparar(dato,actual->dato) == 0)
		{
			if(anterior!=NULL)
				anterior->sig = actual->sig;
			else
				lista->inicio = actual->sig;
			//free(actual->dato);
			free(actual);
			lista->cant--;
			break;			
		}		
		anterior = actual;
	}	
}

void* buscarDato(Lista lista,void *dato)
{
	Nodo *actual,*anterior=NULL;
	for( actual = lista.inicio ; actual!=NULL ;  actual = actual->sig)
	{
		if( lista.comparar(dato,actual->dato) == 0)
		{
			return actual->dato;
		}
		anterior = actual;
	}	
	return NULL;
	
}

void insertarOrdenado(Lista *lista,void* dato)
{
	Nodo *nuevo = crearNodo(dato);
	Nodo *actual,*anterior=NULL;
	for( actual = lista->inicio ; actual!=NULL ;  actual = actual->sig)
	{
		if( lista->comparar(dato,actual->dato) == -1)
		//if( dato < actual->dato) //CONDICION DE PARO: AQUI VOY A INSERTAR
		{
			if(anterior!=NULL)
			{
				//INSERTO EN MEDIO
				anterior->sig = nuevo;
				nuevo->sig = actual;
			}
			else
			{
				//INSERTAR EN EL INICIO
				nuevo->sig = actual;
				lista->inicio = nuevo;
			}
			break;
		}
		anterior = actual;
	}		
	if(lista->inicio == NULL) //LISTA VACIA : INSERTAR UNICO DATO
		lista->inicio = nuevo;
	else if ( anterior !=NULL) // RECORRI TODA LA LISTA: INSERTAR AL FINAL
		anterior->sig = nuevo;	
	lista->cant++;	
}

void mostrarLista(Lista lista)
{
	Nodo *p;	
	printf("\n Lista[%d]: ",lista.cant);
	for( p = lista.inicio ;  p!=NULL ; p = p->sig  )
	{
		printf(" ");
		lista.imprimir(p->dato);
		printf("->");
	}
	printf("NULL");
}

void insertarFinal(Lista *lista,void* dato)
{
	//CREAR NODO
	Nodo *nuevo = crearNodo(dato);
	if(lista->inicio == NULL && lista->cant == 0)
	{
		//LISTA VACIA
		//ASOCIAMOS AL INICIO
		lista->inicio = nuevo;
	}
	else
	{
		//RECORRER Y ENCONTRAR EL ULTIMO NODO
		//INSERTAR
		Nodo *p;
		for( p = lista->inicio ;  p!=NULL ; p = p->sig  )
		{
			if(p->sig == NULL)
			{
				p->sig = nuevo;
				break;
			}
		}
	}
	lista->cant++;
}



void insertarInicio(Lista *lista,void* dato)
{
	Nodo *nuevo = crearNodo(dato);
	nuevo->sig = lista->inicio;
	lista->inicio = nuevo;	
	lista->cant++;
}

void insertarEnPosicion(Lista *lista,void* dato,int pos)
{	
	//VALIDAR
	if( pos==0 ||(pos > 0  && pos <= lista->cant ))
	{			
		Nodo *anterior ,*actual;
		anterior = NULL;
		actual = lista->inicio;
		for (int i=0; i<=lista->cant  ; i++)
		{
			if( i==pos)
			{
				if(anterior == NULL)
				{
					insertarInicio(lista,dato);
					return;
				}
				else
				{
					Nodo *nuevo = crearNodo(dato);
					nuevo->sig = actual;
					anterior->sig = nuevo;
					lista->cant++;
					return;
				
				}
			}
			anterior = actual;
			actual = actual->sig;
		}				
	}	
}


void eliminarEnPosicion(Lista *lista,int pos)
{		
	//VALIDAR
	if( pos==0 ||(pos > 0  && pos < lista->cant ))
	{			
		Nodo *anterior ,*actual;
		anterior = NULL;
		actual = lista->inicio;
		for (int i=0; i<lista->cant  ; i++)
		{
			if( i==pos)
			{
				if(anterior == NULL)
					lista->inicio = actual->sig;
				else
					anterior->sig = actual->sig;
	
				free(actual);
				lista->cant--;
				break;
			}
			anterior = actual;
			actual = actual->sig;
		}			
	}	
}