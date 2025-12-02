#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include "../Estructuras/pila/pila.h"
#include "../captura/captura.h"
#include "../Estructuras/cola/cola.h"
#include "../Estructuras/arbol/arbol.h"

#define MAX_EXPRESION 256
#define MAX_VARIABLES 50

typedef struct
{
    char caracter;
    float *valor;
    int es_operador;
} Dato;

int esOperador(char caracter);
int prioridad(char operador);
Dato* crearDato(char caracter, int es_op);
void imprimirDatoArbol(void *dato);
void liberarDato(void *dato); 
void liberarNodoA(void* nodo);

void** infixAPostfix(Pila *pila);
void construirArbolExp(Cola* cola, Arbol* arbol, Pila *pilaSubArboles);

void imprimirArbolEnorden(NodoA* raiz);
void capturarValores(NodoA* raiz, char letras[], float valores[], int *n);
float realizarCalculo(float a, float b, char op);
float evaluarArbol(NodoA* raiz);


int main(void){

    Pila pilaOperadores; 
    Cola colaPostfija = inicializarCola(); 
    Arbol arbol;
    
    inicializarArbol(&arbol, imprimirDatoArbol, NULL, liberarDato);

    inicializarPila(&pilaOperadores, -1, imprimirDatoArbol, liberarDato);

    int option;

    do {
        printf("\n PRACTICA 8\n");
        printf("[1] CAPTURAR EXPRESION \n[2] IMPRIMIR ARBOL DE EXPRESION (Inorden)\n");
        printf("[3] EVALUAR EXPRESION \n[4] TERMINAR PROGRAMA\n");
        
        inputEntero("\n Selecciona opcion: ",&option);

        switch(option){
            case 1:
            {
                destruirArbol(&arbol); 
                inicializarArbol(&arbol, imprimirDatoArbol, NULL, liberarDato);
                
                eliminarPila(&pilaOperadores);
                inicializarPila(&pilaOperadores, -1, imprimirDatoArbol, liberarDato);
                
                eliminarCola(&colaPostfija);
                colaPostfija = inicializarCola();

                // Infix a Postfix 
                void **postfix_datos = infixAPostfix(&pilaOperadores);
                
                if (postfix_datos != NULL) {
                    printf("\n Postfix: ");
                    int i;
                    for(i=0; postfix_datos[i] != NULL; i++) {
                        printf("%c ", ((Dato*)postfix_datos[i])->caracter);
                        enqueue(&colaPostfija, postfix_datos[i]); 
                    }
                    printf("\n");
                    free(postfix_datos);

                    // construccion del arbol
                    Pila pilaSubArboles; 
                    // Esta pila almacena punteros a NodoA
                    inicializarPila(&pilaSubArboles, -1, NULL, liberarNodoA); 
                    
                    construirArbolExp(&colaPostfija, &arbol, &pilaSubArboles);
                    eliminarPila(&pilaSubArboles); 
                    
                    if (arbol.raiz != NULL) {
                        printf("\n ARBOL CONSTRUIDO.\n");
                    } else {
                        printf("\n ERROR: No se pudo construir el arbol. Expresion invalida o vacia.\n");
                    }
                }
                break;
            }
            case 2:
                if (!arbol.raiz) {
                    printf("\n ERROR: Arbol no construido.\n");
                } else {
                    printf("\n --- ARBOL DE EXPRESION (Grafico) ---\n");
                    imprimirArbol(arbol); 
                    printf("\n");
                }
                break;
            case 3:
            {
                if (!arbol.raiz) {
                    printf("\n ERROR: Arbol no construido.\n");
                    break;
                }
                
                char letras[MAX_VARIABLES];
                float valores[MAX_VARIABLES]; 
                int n = 0;

                printf("\n--- Captura de Valores ---\n");
                capturarValores(arbol.raiz, letras, valores, &n);

                float resultado = evaluarArbol(arbol.raiz);
                printf("\n RESULTADO: %.2f \n", resultado);
                break;
            }
            case 4:
                printf("\n PROGRAMA TERMINADO.\n");
                break;
            default:
                printf("\n Opcion invalida.\n");
                break;
        }
    } while(option != 4);

    destruirArbol(&arbol); 
    eliminarPila(&pilaOperadores);
    eliminarCola(&colaPostfija);
    
    return 0;
}

Dato* crearDato(char caracter, int es_op){
    Dato *nuevo = (Dato*)calloc(1, sizeof(Dato));
    if (nuevo) {
        nuevo->caracter = caracter;
        nuevo->es_operador = es_op;
        nuevo->valor = NULL; 
    }
    return nuevo;
}

void imprimirDatoArbol(void *dato) {
    Dato *info = (Dato*)dato;
    if (info) {
        printf("%c", info->caracter);
    }
}

void liberarDato(void *dato) {
    if (dato) {
        free(dato);
    }
}

void liberarNodoA(void* nodo) {
    if (nodo) {
        free(nodo);
    }
}

int esOperador(char caracter){
    return (caracter == '+' || caracter == '-' || caracter == '*' || caracter == '/' || caracter == '^');
}

int prioridad(char operador) {
    if (operador == '^') return 3;
    if (operador == '*' || operador == '/') return 2;
    if (operador == '+' || operador == '-') return 1;
    return 0;
}

void** infixAPostfix(Pila *pila){
    void** salida = calloc(MAX_EXPRESION, sizeof(void*)); 
    char mensaje[MAX_EXPRESION];
    
    if (!salida) {
        printf("\n ERROR: Falla al asignar memoria para la salida Postfix.\n");
        return NULL;
    }
    
    if (inputCadena("\n Ingrese expresion (solo letras y operadores): ", mensaje, MAX_EXPRESION) != 1) {
        free(salida);
        return NULL;
    }
    
    int i = 0;
    int pos = 0;
    while(mensaje[i] != '\0'){
        char a = mensaje[i];

        if(a == ' ' || a == '\t') {
            i++;
            continue;
        }

        if((a >= 'A' && a<= 'Z') || (a >= 'a' && a<= 'z')){ 
            Dato* dato_operando = crearDato(a, 0); 
            if (!dato_operando) {
                // Liberar todos los Datos ya creados en 'salida'
                for(int j = 0; j < pos; j++) liberarDato(salida[j]); 
                free(salida); 
                return NULL; 
            }
            salida[pos++] = dato_operando; 

        } else if(a == '('){
            Dato* dato_parentesis = crearDato(a, 0); 
            if (!dato_parentesis) { 
                for(int j = 0; j < pos; j++) liberarDato(salida[j]); 
                free(salida); 
                return NULL; 
            }
            push(pila, dato_parentesis);

        } else if(esOperador(a)){
            while(!vacia(*pila) && ((Dato*)peek(*pila))->caracter != '(' &&
                     prioridad(((Dato*)peek(*pila))->caracter) >= prioridad(a)){ 
                salida[pos++] = (Dato*)pop(pila);
            }
            Dato* dato_operador = crearDato(a, 1); 
            if (!dato_operador) { 
                for(int j = 0; j < pos; j++) liberarDato(salida[j]); 
                free(salida); 
                return NULL; 
            }
            push(pila, dato_operador); 

        } else if(a == ')'){
            while(!vacia(*pila) && ((Dato*)peek(*pila))->caracter != '('){
                salida[pos++] = (Dato*)pop(pila);
            }
            if(vacia(*pila)){
                printf("\n ERROR: EXPRESION NO VALIDA (Parentesis desbalanceado)");
                for(int j = 0; j < pos; j++) liberarDato(salida[j]); 
                free(salida); 
                return NULL;
            }
            liberarDato(pop(pila)); // Sacar y liberar el '('
        } else {
            // Manejar caracteres inválidos 
            printf("\n ERROR: Caracter invalido en la expresion: %c", a);
            for(int j = 0; j < pos; j++) liberarDato(salida[j]); 
            free(salida); 
            return NULL;
        }
        i++;
    }

    while (!vacia(*pila)) {
        Dato *dato_cima = (Dato*)pop(pila);
        
        if (dato_cima->caracter == '('){ 
            printf("\n ERROR: EXPRESION NO VALIDA (Parentesis sin cerrar)");
            liberarDato(dato_cima);
            for (int j = 0; j < pos; j++) liberarDato(salida[j]); 
            free(salida); 
            return NULL; 
        }
        salida[pos++] = dato_cima;
    }
    
    if (pos == 0) { free(salida); return NULL; }
    
    return salida;
}

void construirArbolExp(Cola* cola, Arbol* arbol, Pila *pilaSubArboles){
    
    while(!vaciaC(*cola)){
        Dato* dato_info = (Dato*)dequeue(cola); 
        if (!dato_info) continue; 
        
        // Si es operando/variable
        if(dato_info->es_operador == 0){
            NodoA* nodo_operando = crearNodoA(dato_info); 
            if (!nodo_operando) { liberarDato(dato_info); return; }
            push(pilaSubArboles, nodo_operando); 
        } 
        
        else if(dato_info->es_operador == 1){
            NodoA *raiz = crearNodoA(dato_info);
            if (!raiz) { liberarDato(dato_info); return; }

            if (vacia(*pilaSubArboles)) { liberarDato(dato_info); liberarNodoA(raiz); return; }
            NodoA* hijo_derecho = (NodoA*)pop(pilaSubArboles);

            if (vacia(*pilaSubArboles)) { 
                liberarDato(dato_info); 
                liberarNodoA(raiz); 
                push(pilaSubArboles, hijo_derecho);
                return;
            }
            NodoA* hijo_izquierdo = (NodoA*)pop(pilaSubArboles);

            // Enlazar y apilar
            raiz->dch = hijo_derecho;
            raiz->izq = hijo_izquierdo;
            push(pilaSubArboles, raiz);
        }
    }
    
    // Asignar la raíz final
    if (pilaSubArboles->cantidad == 1) {
        arbol->raiz = (NodoA*)pop(pilaSubArboles);
    } else {
        arbol->raiz = NULL;
    }
}

void imprimirArbolEnorden(NodoA* raiz) {
    if (raiz == NULL) return;
    
    Dato* info = (Dato*)raiz->dato;

    // Lógica para paréntesis
    int necesita_parentesis = info->es_operador; 

    if (necesita_parentesis) printf("(");
    
    imprimirArbolEnorden(raiz->izq);
    
    printf("%c", info->caracter);

    imprimirArbolEnorden(raiz->dch);
    
    if (necesita_parentesis) printf(")");
}


void capturarValores(NodoA* raiz, char letras[], float valores[], int *n) {
    if (raiz == NULL) return;
    
    capturarValores(raiz->izq, letras, valores, n); 

    Dato* info = (Dato*)raiz->dato;

    if (info->es_operador == 0) {
        int existe = 0;

        // Busca si ya se pidió esta letra
        for (int i = 0; i < *n; i++) {
            if (letras[i] == info->caracter) {
                info->valor = &valores[i];
                existe = 1;
                break;
            }
        }

        // Si no existe, se pide el valor y se registra
        if (!existe && *n < MAX_VARIABLES) {
            letras[*n] = info->caracter;
            printf("Ingresa valor para %c: ", info->caracter);
            inputFloat("", &valores[*n]); 
            info->valor = &valores[*n];
            (*n)++;
        }
    }
    
    capturarValores(raiz->dch, letras, valores, n);
}

float realizarCalculo(float a, float b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                fprintf(stderr, "Error: Division por cero.\n");
                return 0; 
            }
            return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

float evaluarArbol(NodoA* raiz) {
    if (raiz == NULL) return 0;
    
    Dato* info = (Dato*)raiz->dato; 

    if (info->es_operador == 0) { 
        if (info->valor != NULL) {
            return *(info->valor); // Retorna el valor capturado
        } else {
            return 0;
        }
    }

    float val_izq = evaluarArbol(raiz->izq);
    float val_dch = evaluarArbol(raiz->dch);

    // Aplica la operación
    return realizarCalculo(val_izq, val_dch, info->caracter);
}