#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../captura/captura.h"
#include "../Estructuras/cola/cola.h"

#define NUMERO_CAJAS 5
#define HABILITADA 1
#define DESHABILITADA 0
#define MIN_ARTICULOS 1
#define MAX_ARTICULOS 50
#define MAX_CLIENTES 20

typedef struct {
    Cola cola;
    int estado;
} Caja;

int* generar_articulos(void);
void imprimirEntero(void *a);
void escanear_articulos(Caja *caja);
void habilitar_deshabilitar_caja(Caja *cajas);
void liberar_cajas(Caja *cajas);

int main(void) {
    srand(time(NULL));

    Caja cajas[NUMERO_CAJAS];

    for (int i = 0; i < NUMERO_CAJAS; i++) {
        cajas[i].cola = inicializarCola();
        cajas[i].cola.imprimir = &imprimirEntero;
        cajas[i].estado = HABILITADA;
        cajas[i].cola.capacidad = MAX_CLIENTES;
    }

    int opcion;
    do {
        //Estado decajas
        printf("\n\n===== ESTADO DE CAJAS =====\n");
        for (int i = 0; i < NUMERO_CAJAS; i++) {
            printf("[%d] (%s)\t", i + 1, cajas[i].estado ? "HABILITADA" : "DESHABILITADA");
            imprimirCola(cajas[i].cola);
            printf("\n");
        }

        printf("\n===== MENu =====");
        printf("\n[1] Generar clientes");
        printf("\n[2] Escanear articulos");
        printf("\n[3] Habilitar/Deshabilitar caja");
        printf("\n[4] Terminar programa");
        inputEntero("\nSelecciona opcion: ", &opcion);

        switch (opcion) {
            case 1:
                for (int i = 0; i < NUMERO_CAJAS; i++) {
                    if (cajas[i].estado && !llenaC(cajas[i].cola)) {
                        enqueue(&cajas[i].cola, generar_articulos());
                    }
                }
                break;

            case 2:
                for (int i = 0; i < NUMERO_CAJAS; i++) {
                    escanear_articulos(&cajas[i]);
                }
                break;

            case 3:
                habilitar_deshabilitar_caja(cajas);
                break;

            case 4:
                liberar_cajas(cajas);
                break;

            default:
                printf("\nOpcion no valida.");
        }

    } while (opcion != 4);

    return 0;
}

int* generar_articulos(void) {
    int *articulos = malloc(sizeof(int));
    *articulos = rand() % (MAX_ARTICULOS - MIN_ARTICULOS + 1) + MIN_ARTICULOS;
    return articulos;
}

void imprimirEntero(void *a) {
    int *valor = (int*)a;
    printf("%d", *valor);
}

void escanear_articulos(Caja *caja) {
    if (vaciaC(caja->cola)) return;

    int *articulos = peekC(caja->cola);
    (*articulos)--;

    if (*articulos <= 0) {
        free(dequeue(&caja->cola));  // Cliente terminado
    }
}

void habilitar_deshabilitar_caja(Caja *cajas) {
    int numero;
    inputEntero("\nSelecciona numero de caja (1-5): ", &numero);
    if (numero < 1 || numero > NUMERO_CAJAS) {
        printf("\nNumero invalido.");
        return;
    }

    int i = numero - 1;
    cajas[i].estado = !cajas[i].estado;
    printf("\nCaja [%d] ahora esta %s.", numero,
           cajas[i].estado ? "HABILITADA" : "DESHABILITADA");
}

void liberar_cajas(Caja *cajas) {
    for (int i = 0; i < NUMERO_CAJAS; i++) {
        while (!vaciaC(cajas[i].cola)) {
            free(dequeue(&cajas[i].cola));
        }
    }
}