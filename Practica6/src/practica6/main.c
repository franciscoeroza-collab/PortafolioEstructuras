#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../captura/captura.h"
#include "../Estructuras/arbol/arbol.h"

int* crearEntero(int dato)
{
	int *nuevo = (int*)malloc(sizeof(int));
	if (nuevo)
		*nuevo = dato;
	return nuevo;
}

int compararEntero(void*a,void*b)
{
	int *aa = (int*)a, *bb = (int*)b;
	if(*aa == *bb) return 0;
	else if(*aa < *bb) return -1;
	else return 1;
}

void imprimirEntero(void *a)
{
	int *aa = (int*)a;
	printf("%d",*aa);
}

void liberarEntero(void *a)
{
    if (a)
        free((int*)a);
}

void mostrarMenu()
{
	printf("\n--- MENU ---\n");
	printf("1. Insertar en Arbol A\n");
	printf("2. Insertar en Arbol B\n");
	printf("3. Comparar Arboles\n");
	printf("4. Eliminar dato en Arbol A\n");
	printf("5. Eliminar dato en Arbol B\n");
	printf("6. Profundidades/Alturas de Arboles\n");
	printf("7. Equilibrar Arboles A y B\n");
	printf("8. Mostrar Arboles A y B\n");
	printf("0. Terminar programa (Liberar memoria en POSTORDEN)\n");
	printf("-----------------------------------\n");
	printf("Seleccione una opcion: ");
}

int main(void)
{
	// Inicialización de los dos árboles
	Arbol arbolA, arbolB;
	inicializarArbol(&arbolA, &imprimirEntero, &compararEntero, &liberarEntero);
	inicializarArbol(&arbolB, &imprimirEntero, &compararEntero, &liberarEntero);
	
	int opcion;
	int dato;
	
	do
	{
		mostrarMenu();
		if (scanf("%d", &opcion) != 1)
		{
			while(getchar() != '\n');
			opcion = -1;
			printf("Opcion invalida. Intente de nuevo.\n");
			continue;
		}

		switch(opcion)
		{
			case 1:
				printf("Ingrese el valor entero a insertar en Arbol A: ");
				if (scanf("%d", &dato) == 1)
				{
					insertarArbol(&arbolA, crearEntero(dato));
					printf("Dato %d insertado en Arbol A.\n", dato);
				}
				break;
			case 2:
				printf("Ingrese el valor entero a insertar en Arbol B: ");
				if (scanf("%d", &dato) == 1)
				{
					insertarArbol(&arbolB, crearEntero(dato));
					printf("Dato %d insertado en Arbol B.\n", dato);
				}
				break;
			case 3:
				{
					bool soloDatos = false;
					bool iguales = compararArboles(arbolA, arbolB, &soloDatos);
					
					if (iguales)
						printf("\nLos Arboles A y B son IGUALES en ESTRUCTURA y DATOS.\n");
					else if (soloDatos)
						printf("\nLos Arboles A y B son IGUALES SOLO en DATOS, pero la estructura es diferente.\n");
					else
						printf("\nLos Arboles A y B son DIFERENTES.\n");
				}
				break;
			case 4:
					//no hay break asi que pasa al siguiente case
			case 5:
				{
					printf("Ingrese el valor entero a eliminar: ");
					if (scanf("%d", &dato) == 1)
					{
						int *datoAEliminar = crearEntero(dato);
						bool eliminado = false;
						
						if (opcion == 4)
							eliminado = eliminarDatoArbol(&arbolA, datoAEliminar);
						else
							eliminado = eliminarDatoArbol(&arbolB, datoAEliminar);
						
						free(datoAEliminar); 
						
						if (eliminado)
							printf("Dato %d eliminado exitosamente de Arbol %c.\n", dato, (opcion == 4 ? 'A' : 'B'));
						else
							printf("El dato %d no se encontro en Arbol %c.\n", dato, (opcion == 4 ? 'A' : 'B'));
					}
				}
				break;
			case 6:
				printf("Altura de Arbol A: %d\n", calcularProfundidad(arbolA));
				printf("Altura de Arbol B: %d\n", calcularProfundidad(arbolB));
				break;
			case 7:
				printf("Equilibrando Arbol A (Antes: Altura %d):\n", calcularProfundidad(arbolA));
				equilibrarArbol(&arbolA);
				
				printf("Equilibrando Arbol B (Antes: Altura %d):\n", calcularProfundidad(arbolB));
				equilibrarArbol(&arbolB);
				break;
			case 8: // Mostrar Árboles
				printf("\n===================================\n");
				printf("           ARBOL A (Cantidad: %d)          \n", arbolA.cantidad);
				printf("===================================\n");
				imprimirArbol(arbolA);
				printf("\n\n");
				
				printf("\n===================================\n");
				printf("           ARBOL B (Cantidad: %d)          \n", arbolB.cantidad);
				printf("===================================\n");
				imprimirArbol(arbolB);
				printf("\n");
				break;
			case 0:
				printf("Liberando memoria de Arbol A con recorrido POSTORDEN. Elementos: ");
				imprimirOrden(arbolA, POSTORDEN);
				destruirArbol(&arbolA);
				
				printf("\nLiberando memoria de Arbol B con recorrido POSTORDEN. Elementos: ");
				imprimirOrden(arbolB, POSTORDEN);
				destruirArbol(&arbolB);
				break;
			default:
				printf("Opcion no reconocida. Intente de nuevo.\n");
				break;
		}

	} while(opcion != 0);
	
	return 0;
}