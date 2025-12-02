#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h" 

typedef struct {
    char *nombre;
    int noPaginas;
} Archivo;

void estadoHeap(Heap heap, const char *tiempo);
void agregarArchivo(Heap *heap);
void imprimirArchivo(void *);
void procesarArchivo(Heap *heap);
void eliminarArchivo(Heap *heap);
void archivoCLS(Heap *heap);
void imprimirA5(Heap heap);
int compararArchivo(void*a,void*b);

int main(void)
{
    Heap heap = inicializarHeap( &imprimirArchivo,&compararArchivo);
    int option;

    do {
        imprimirA5(heap);

        printf("\n [0] CAMBIAR PRIORIDAD \n [1] MOSTRAR COLA DE IMPRESION");
        printf("\n [2] AGREGAR ARCHIVO \n [3] PROCESAR / IMPRIMIR ARCHIVO");
        printf("\n [4] ELIMINAR ARCHIVO \n [5] ELIMINAR TODOS LOS ARCHIVOS");
        printf("\n [6] TERMINAR PROGRAMA");
        inputEntero("\n Selecciona opcion: ",&option);

        switch(option) {
            case 0:
                estadoHeap(heap,"ANTES");
                printf("\n HEAP CAMBIADO A: ");
                if(heap.tipo == MIN){
                    heap.tipo = MAX;
                    printf("MAXIMO (Mayor paginas = Mayor prioridad)\n");
                }
                else{
                    heap.tipo = MIN;
                    printf("MINIMO (Menor paginas = Mayor prioridad)\n");
                }
                heapify(&heap);
                estadoHeap(heap,"DESPUES");
                break;
            case 1:
                printf("\n COLA DE IMPRESION: ");
                imprimirHeap(heap);
                printf("\n");
                break;
            case 2:
                estadoHeap(heap,"ANTES");
                agregarArchivo(&heap);
                estadoHeap(heap,"DESPUES");
                break;
            case 3:
                estadoHeap(heap,"ANTES");
                procesarArchivo(&heap);
                estadoHeap(heap,"DESPUES");
                break;
            case 4:
                estadoHeap(heap,"ANTES");
                eliminarArchivo(&heap);
                estadoHeap(heap,"DESPUES");
                break;
            case 5:
                estadoHeap(heap,"ANTES");
                archivoCLS(&heap);
                estadoHeap(heap,"DESPUES");
                break;
            default:
                break;
        }
    } while(option != 6);

    //Se liberan todos los Archivos y NodosA restantes.
    while( heap.cantidad )
    {
        procesarArchivo(&heap);
    }
    
    // Liberar el arreglo del heap si queda un puntero
    if (heap.arr) free(heap.arr);
    
    return 0;
}

void imprimirA5(Heap heap)
{
    printf("\n\n");
    printf("\n ----------------- PRIMEROS 5 ARCHIVOS -----------------\n");
    if(heap.cantidad == 0)
    {
        printf(" Cola Vacia\n");
        return;
    }
    
    int aux = (heap.cantidad < 5) ? heap.cantidad : 5;

    for(int i = 0; i<aux;i++)
    {
        heap.imprimir(heap.arr[i]->dato);
        printf("\n");
    }
    printf(" --------------------------------------------------\n");
    printf("\n\n");
}

void archivoCLS(Heap *heap)
{
    if(heap->cantidad == 0)
    {
        printf("\n La cola ya esta vacia.\n");
        return;
    }
        
    // Liberar el dato (Archivo) y el NodoA de cada elemento
    Archivo *aux;
    for(int i = 0; i<heap->cantidad;i++)
    {
        aux = heap->arr[i]->dato;
        free(aux->nombre);
        free(aux);
        free(heap->arr[i]); // Liberar el NodoA
    }
    
    // Liberar el arreglo de punteros
    free(heap->arr);
    heap->arr = NULL;
    
    heap->cantidad = 0;
    heap->arbol.cantidad = 0;
    heap->arbol.raiz = NULL;
    printf("\n Todos los archivos eliminados. Cola de Impresion vacia.\n");
}

void eliminarArchivo(Heap *heap)
{
    if(heap->cantidad == 0)
    {
        printf("\n La cola esta vacia.\n");
        return;
    }
    
    Archivo *a;
    int aux;
    printf("\nARCHIVOS DISPONIBLES: \n");
    for(int i = 0; i<heap->cantidad;i++)
    {
        a = heap->arr[i]->dato;
        printf(" [%d] %s: %d paginas\n", (i+1),a->nombre,a->noPaginas);
    }
    
    inputEntero("\n Selecciona archivo a eliminar (numero): ",&aux);
    aux--;
    
    if (aux < 0 || aux >= heap->cantidad) {
        printf("\n Error: Seleccion invalida.\n");
        return;
    }

    // Guardar punteros a liberar
    a = heap->arr[aux]->dato;
    NodoA *nodo_a_liberar = heap->arr[aux];

    // Mover el último elemento al lugar del eliminado
    heap->arr[aux] = heap->arr[heap->cantidad - 1];
    heap->cantidad--;
    
    // Liberar el dato y el nodo
    free(a->nombre);
    free(a);
    free(nodo_a_liberar); 

    // Reducir memoria y restaurar la propiedad Heap
    if(heap->cantidad > 0)
    {
        NodoA **new_arr = realloc(heap->arr, sizeof(NodoA*) * heap->cantidad);
        if (new_arr != NULL) heap->arr = new_arr;
        
        heapifyUp(heap, aux);
        heapifyDown(heap, aux);

        actualizarArbol(heap);
    }
    else
    {
        free(heap->arr);
        heap->arr = NULL;
        actualizarArbol(heap);
    }
    printf("\n Archivo eliminado y Heap reorganizado.\n");
}

void procesarArchivo(Heap *heap)
{
    // Eliminar el archivo de mayor prioridad
    if(heap->cantidad == 0)
    {
        printf("\n La cola esta vacia.\n");
        return;
    }
    
    Archivo *a = (Archivo*)quitarHeap(heap); 
    printf("\n Procesando archivo: ");
    imprimirArchivo(a);
    printf("\n");
    free(a->nombre);
    free(a);
}

void estadoHeap(Heap heap, const char *tiempo)
{
    printf("\n             --- ESTADO %s ---", tiempo);
    printf("\n -----------------------------------");
    printf("\n HEAP (Arreglo): ");
    if(heap.cantidad == 0)
    {
        printf(" Vacio \n");
    }
    else
    {
        imprimirHeap(heap);
        printf("\n\n ");
        imprimirArbol(heap.arbol); 
    }

}

void agregarArchivo(Heap *heap)
{
    Archivo *archivo = (Archivo*)calloc(1,sizeof(Archivo));
    if (archivo == NULL) {
        fprintf(stderr, "\nError: Falla al asignar memoria para Archivo.\n");
        return;
    }
    
    int cantPaginas;
    char cad[100];

    inputCadena("\n Ingrese nombre del archivo: ", cad, 100);
    archivo->nombre = cadenaDinamica(cad);
    do{
        inputEntero("\n Ingrese cantidad de paginas: ",&cantPaginas);
    }while(cantPaginas <= 0);
    
    archivo->noPaginas = cantPaginas;
    insertarHeap(heap,archivo);
    printf("\n Archivo '%s' agregado.\n", archivo->nombre);
}

void imprimirArchivo(void *arch){
    Archivo *a = (Archivo *)arch;
    printf(" [ %s: %d ]",a->nombre,a->noPaginas);
}

int compararArchivo(void*a,void*b)
{
    Archivo *aa = (Archivo*)a;
    Archivo *bb = (Archivo*)b;
    if(aa->noPaginas==bb->noPaginas)return 0;
    else if(aa->noPaginas<bb->noPaginas)return -1; 
    else return 1; 
}