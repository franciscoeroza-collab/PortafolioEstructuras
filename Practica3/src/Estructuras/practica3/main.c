#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Estructuras/pila/pila.h"
#include "../captura/captura.h"

#define LONGITUD 128

void imprimirCadena(void *a);
void liberarCadena(void *a);

int esPalindromo(const char *cadena);
int verificarBalanceo(const char *cadena);

int main(void)
{
    int opcion;
    char entrada[LONGITUD];

    do {
        printf("\n==== MENU ====\n");
        printf("1. Verificar Palindromo\n");
        printf("2. Verificar Parentesis/Llaves/Corchetes\n");
        printf("3. Salir\n");
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpiar buffer

        switch(opcion)
        {
            case 1:
                printf("\nIngresa la cadena: ");
                fgets(entrada, LONGITUD, stdin);
                entrada[strcspn(entrada, "\n")] = 0; // quitar salto de linea
                if (esPalindromo(entrada))
                    printf("\n Es palindromo.\n");
                else
                    printf("\n No es palindromo.\n");
                break;

            case 2:
                printf("\nIngresa la cadena: ");
                fgets(entrada, LONGITUD, stdin);
                entrada[strcspn(entrada, "\n")] = 0;
                if (verificarBalanceo(entrada))
                    printf("\n Balanceado correctamente.\n");
                else
                    printf("\n No esta balanceado.\n");
                break;

            case 3:
                printf("\nFin del programa.\n");
                break;

            default:
                printf("\nOpcion invalida.\n");
        }
    } while(opcion != 3);

    return 0;
}

void imprimirCadena(void *a)
{
    char *aa = a;
    printf("%s", aa);
}

void liberarCadena(void *a)
{
    char *aa = a;
    free(aa);
}

int esPalindromo(const char *cadena)
{
    Pila pila = {NULL, -1, 0, NULL, NULL};
    int i, len = strlen(cadena);

    // Filtrar solo letras y números, convertir mayúsculas a minúsculas
    char sinFormato[LONGITUD] = {0};
    int j = 0;
    for(i = 0; i < len; i++) {
        char c = cadena[i];

        // Si es mayúscula -> convertir a minúscula
        if(c >= 'A' && c <= 'Z') {
            c = c + 32;
            sinFormato[j++] = c;
        }
        // Si es minúscula o número -> guardar igual
        else if((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            sinFormato[j++] = c;
        }
        // Si es espacio u otro símbolo -> ignorar
    }
    sinFormato[j] = '\0';

    // Guardar en pila
    for(i = 0; i < j; i++) {
        char *c = malloc(sizeof(char));
        *c = sinFormato[i];
        push(&pila, c);
    }

    // Comparar al revés
    for(i = 0; i < j; i++) {
        char *c = pop(&pila);
        if(*c != sinFormato[i]) {
            eliminarPila(&pila);
            return 0; // no palindromo
        }
        free(c);
    }

    return 1; // sí palíndromo
}

int verificarBalanceo(const char *cadena)
{
    Pila pila = {NULL, -1, 0, NULL, NULL};
    int i, len = strlen(cadena);

    for(i = 0; i < len; i++) {
        char c = cadena[i];

        if(c == '(' || c == '{' || c == '[') {
            char *ap = malloc(sizeof(char));
            *ap = c;
            push(&pila, ap);
        }
        else if(c == ')' || c == '}' || c == ']') {
            if(vacia(pila)) return 0; // cierre sin apertura
            char *tope = pop(&pila);

            if( (c == ')' && *tope != '(') ||
                (c == '}' && *tope != '{') ||
                (c == ']' && *tope != '[') ) {
                free(tope);
                eliminarPila(&pila);
                return 0; // error de correspondencia
            }
            free(tope);
        }
    }

    int balanceado = vacia(pila);
    eliminarPila(&pila);
    return balanceado;
}