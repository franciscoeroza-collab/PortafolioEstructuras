#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Estructuras/pila/pila.h"
#include "../captura/captura.h"

#define MAX 256
#define CAPACIDAD -1

int validarParentesis(const char *expresion);
int prioridad(char operador);
void infixAPostfix(const char *infix, char *postfix);
int esOperador(char c);
int esOperando(char c);

int main()
{
    int opcion;
    char expresionInfix[MAX]; 
    char expresionPostfix[MAX * 2]; 

    do
    {
        printf("1. Infix a Postfix\n");
        printf("2. Terminar el programa\n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); 
            opcion = 0; 
            continue;
        }
        
        while (getchar() != '\n');

        switch (opcion)
        {
            case 1:
                printf("\nIngrese la expresion infix: \n> ");
                if (fgets(expresionInfix, MAX, stdin) == NULL) { 
                    printf("Error al leer la entrada.\n");
                    break;
                }
                expresionInfix[strcspn(expresionInfix, "\n")] = 0;

                if (!validarParentesis(expresionInfix))
                {
                    printf("\nERROR: La expresion NO es valida. Hay un desbalance de parentesis.\n");
                }
                else
                {
                    // convertir
                    infixAPostfix(expresionInfix, expresionPostfix);
                    printf("\nExpresion Infix:  %s", expresionInfix);
                    printf("\nExpresion Postfix: %s\n", expresionPostfix);
                }
                break;
            
            case 2:
                printf("\nPrograma terminado\n");
                break;
                
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
                break;
        }
        
    } while (opcion != 2);

    return 0;
}

int validarParentesis(const char *expresion)
{
    Pila pilaParentesis;
    inicializarPila(&pilaParentesis, CAPACIDAD, NULL, free); 

    int resultado = 1;

    for (size_t i = 0; i < strlen(expresion); i++)
    {
        char c = expresion[i];

        if (c == '(')
        {
            char *p_c = (char*)malloc(sizeof(char));
            if (!p_c) {
                printf("Error\n");
                eliminarPila(&pilaParentesis);
                return 0; 
            }
            *p_c = c;
            push(&pilaParentesis, (void*)p_c);
        }
        else if (c == ')')
        {
            if (vacia(pilaParentesis))
            {
                resultado = 0;
                break;
            }
            void *p_c = pop(&pilaParentesis);
            if (p_c != NULL) {
                free(p_c);
            }
        }
    }

    if (!vacia(pilaParentesis))
    {
        resultado = 0; // Quedan paréntesis sin cerrar
    }
    
    eliminarPila(&pilaParentesis); 
    
    return resultado;
}

int prioridad(char operador)
{
    if (operador == '^') return 3;
    if (operador == '*' || operador == '/') return 2;
    if (operador == '+' || operador == '-') return 1;
    return 0; // Para paréntesis y otros
}

int esOperador(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int esOperando(char c)
{
    // Verifica si es una letra minúscula, letra mayúscula, o un dígito.
    return (c >= 'a' && c <= 'z') || 
           (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9');
}

void infixAPostfix(const char *infix, char *postfix)
{
    Pila pilaOperadores;
    inicializarPila(&pilaOperadores, CAPACIDAD, NULL, free); 
    
    int i = 0; // Índice de la expresión infix
    int j = 0; // Índice de la expresión postfix
    
    char *p_op = NULL; 

    while (infix[i] != '\0')
    {
        char c = infix[i];

        if (c == ' ' || c == '\t')
        {
            i++;
            continue;
        }

        if (esOperando(c))
        {
            // Si es operando, agrego a la expresión postfix y tambien un espacio
            postfix[j++] = c;
            postfix[j++] = ' ';
        }
        else if (c == '(')
        {
            char *p_c = (char*)malloc(sizeof(char));
            if (!p_c) {
                eliminarPila(&pilaOperadores);
                return;
            }
            *p_c = c;
            push(&pilaOperadores, (void*)p_c);
        }
        else if (c == ')')
        {
            // Sacar operadores hasta encontrar '('
            while (!vacia(pilaOperadores) && (*((char*)peek(pilaOperadores)) != '('))
            {
                p_op = (char*)pop(&pilaOperadores);
                postfix[j++] = *p_op;
                postfix[j++] = ' ';
                free(p_op);
            }
            
            if (!vacia(pilaOperadores) && (*((char*)peek(pilaOperadores)) == '('))
            {
                p_op = (char*)pop(&pilaOperadores);
                free(p_op); 
            }
        }
        else if (esOperador(c))
        {
            // Sacar operadores de mayor o igual precedencia
            while (!vacia(pilaOperadores) && 
                   (*((char*)peek(pilaOperadores)) != '(') && 
                   (prioridad(*((char*)peek(pilaOperadores))) >= prioridad(c)))
            {
                p_op = (char*)pop(&pilaOperadores);
                postfix[j++] = *p_op;
                postfix[j++] = ' ';
                // Liberar el carácter (operador)
                free(p_op); 
            }
            
            // Empujar el operador actual
            char *p_c = (char*)malloc(sizeof(char));
            if (!p_c) {
                fprintf(stderr, "Error de asignacion de memoria en infixAPostfix.\n");
                eliminarPila(&pilaOperadores);
                return;
            }
            *p_c = c;
            push(&pilaOperadores, (void*)p_c);
        }

        i++;
    }

    while (!vacia(pilaOperadores))
    {
        p_op = (char*)pop(&pilaOperadores);
        postfix[j++] = *p_op;
        postfix[j++] = ' ';
        free(p_op);
    }

    if (j > 0 && postfix[j-1] == ' ') {
        j--;
    }
    postfix[j] = '\0';
}