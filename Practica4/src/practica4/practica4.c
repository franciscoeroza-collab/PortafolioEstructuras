#include <stdio.h>
#include <stdlib.h>
#include "../Estructuras/pila/pila.h"

#define ARRIBA 1
#define ABAJO 2
#define DERECHA 4
#define IZQUIERDA 8

#define REN 27
#define COL 26
#define VACIO 0

typedef struct
{
    int x,y;
}Coordenada;


unsigned char ** crear_laberinto(void);
void imprimirLab(unsigned char **lab);


Coordenada* crearCoordenada(int x,int y);
int compararCoordenada(void *a,void *b);
void imprimirCoordenada(void *dato);
void liberarCoordenada(void *dato);
unsigned char alternativas(unsigned char **lab,Coordenada *coordenada);
void imprimirPosibles(unsigned char);

// --- 
Coordenada *buscar_coordenada(unsigned char **lab, char objetivo);
int resolver_laberinto(unsigned char **lab, Coordenada *inicio, Coordenada *destino);

int main(void)
{
    unsigned char **lab; 
    lab = crear_laberinto();
    
    // Inicializar y buscar coordenadas de inicio 'A' y destino 'B'
    Coordenada *inicio = buscar_coordenada(lab, 'A');
    Coordenada *destino = buscar_coordenada(lab, 'B');

    printf("Laberinto inicial (A = Inicio, B = Destino, ' ' = Camino):\n");
    imprimirLab(lab);

    if (inicio && destino) {
        resolver_laberinto(lab, inicio, destino);
    } else {
        printf("\n\rError: No se encontro el punto de inicio 'A' y/o el destino 'B'.");
    }

    // Liberar memoria
    liberarCoordenada(inicio);
    liberarCoordenada(destino);

    // Liberar la memoria del laberinto
    for(int i=0;i<REN;i++)
    {
        free(lab[i]);
    }
    free(lab);

    printf("\n\n");
    return 0;
}


Coordenada *buscar_coordenada(unsigned char **lab, char objetivo)
{
    int i, j;
    for (i = 0; i < REN; i++)
    {
        for (j = 0; j < COL; j++)
        {
            if (lab[i][j] == objetivo)
            {
                return crearCoordenada(i, j);
            }
        }
    }
    return NULL;
}

//Los movimientos se prueban en el siguiente orden de prioridad: ARRIBA, ABAJO, DERECHA, IZQUIERDA.

int resolver_laberinto(unsigned char **lab, Coordenada *inicio, Coordenada *destino)
{
    // Inicializar la pila. REN * COL como capacidad máxima.
    Pila camino = {NULL, REN * COL, 0, imprimirCoordenada, liberarCoordenada};

    Coordenada *actual = crearCoordenada(inicio->x, inicio->y);
    push(&camino, actual);
    lab[actual->x][actual->y] = '.'; // Aqui marca 'A' como visitado

    int encontrado = 0;
    unsigned char caminos_posibles;

    while (!vacia(camino))
    {
        actual = (Coordenada *)peek(camino);

        if (lab[actual->x][actual->y] == 'B') 
        {
            encontrado = 1;
            break;
        }

        caminos_posibles = alternativas(lab, actual);
        
        if (caminos_posibles != VACIO)
        {
            int next_x = actual->x;
            int next_y = actual->y;

            // Elige la primera dirección válida
            if (caminos_posibles & ARRIBA) {
                next_x = actual->x - 1;
            } else if (caminos_posibles & ABAJO) {
                next_x = actual->x + 1;
            } else if (caminos_posibles & DERECHA) {
                next_y = actual->y + 1;
            } else if (caminos_posibles & IZQUIERDA) {
                next_y = actual->y - 1;
            }

            Coordenada *siguiente = crearCoordenada(next_x, next_y);

            // Comprobar si la siguiente posición es el destino 'B'
            if (lab[next_x][next_y] == 'B') {
                push(&camino, siguiente);
                encontrado = 1;
                break;
            }

            // Si no es 'B', marcar la nueva celda como visitada y hacer push
            lab[next_x][next_y] = '.';
            push(&camino, siguiente);
        }
        else
        {
            lab[actual->x][actual->y] = 'o'; 

            Coordenada *coord_a_liberar = (Coordenada *)pop(&camino);
            if (coord_a_liberar) {
                free(coord_a_liberar); // Liberar la memoria del callejón sin salida.
            }
        }
    }

    if (encontrado)
    {
        printf("\n\r\n\rCamino encontrado (coordenadas [fila,columna]):");
        imprimirPilaInvertida(camino);
        imprimirLab(lab);
    }
    else
    {
        printf("\n\r\n\rNo se encontro el camino.");
    }

    //libera los nodos restantes y las Coordenadas*
    eliminarPila(&camino); 
    return encontrado;
}

void liberarCoordenada(void *dato)
{
    if (dato) {
        free(dato);
    }
}


unsigned char ** crear_laberinto(void)
{
    int i,j;
    char *tmp = 
"xxxxxxxxxxxxxxxxxxxxxxxxxx\
xAx*****x****************x\
x*xxx*x*x*xxxxxxxxx*xxxx*x\
x*****x*x******x**x****x*x\
xxxxxxx*xxxxxx*xx*xxxx*xxx\
x*****x*x****x*x****xx***x\
x*xxx*x*xxxx*x*x*xxxxx*x*x\
x***x*x****x*****xxxxxxx*x\
xxx*x*xxxx*xxxxxxx****x**x\
x*x*x***xx****xx***xx*x*xx\
x*x*x*x*xxxxx**x*xxxx*x**x\
x*x*x*x***x*xx*x****x*xx*x\
x*x*x*xxx****x*x*xx*x****x\
x*x*x*xxxxxxxx*x**x*xxxx*x\
x***x********x*xx*x*x****x\
x*xxxxxxxxxx*x**xxx*x*xxxx\
x***x******x**x*****x**x*x\
xxx*x*xxxxxxx*xxxxxxxx*x*x\
x*x*x*******x****xx****x*x\
x*x*x*xxxxx*xxxx*xx*xxxx*x\
x*x*x****xx***x**xx*x****x\
x*x*xxxxxxx*x**x*xx*x*x*xx\
x*B*********xx*x*xx*xxx*xx\
x*xxxxxxxxxxx**x*********x\
x***x***x***x*xxxxxxxxxx*x\
x*x***x***x*************xx\
xxxxxxxxxxxxxxxxxxxxxxxxxx\0";

    
    unsigned char **lab;
    lab = (unsigned char**) malloc(sizeof(unsigned char *)*REN);
    
    for(i=0;i<REN;i++)
    {
        lab[i] = (unsigned char*) malloc(sizeof(unsigned char)*COL);
    }
    
    for(i=0;i<REN;i++)
    {
        for(j=0;j<COL;j++)
        {
            lab[i][j] = tmp[(i*COL)+j];
        }   
    }
    return lab;
}


void imprimirLab(unsigned char **lab)
{
    int i,j;
    for(i=0;i<REN;i++)
    {
        printf("\n\r");
        for(j=0;j<COL;j++)
        {
            if(lab[i][j]=='*')printf("  ");
            else printf("%c ",lab[i][j]);
        }   
    }
}

Coordenada* crearCoordenada(int x,int y)
{
    Coordenada *nueva = malloc(sizeof(Coordenada));
    nueva->x=x;
    nueva->y=y;
    return nueva;
}


void imprimirCoordenada(void *dato)
{
    Coordenada *a = dato;
    // La fila corresponde a 'x' y la columna a 'y' en este diseño.
    printf("[%-2d,%-2d]", a->x,a->y); 
}

int compararCoordenada(void *a,void *b)
{
    Coordenada *aa=a,*bb=b;
    if(aa->x==bb->x && aa->y==bb->y)
        return 1;
    return 0;
}


unsigned char alternativas(unsigned char **lab,Coordenada *coordenada)
{         
    unsigned char caracter;
    unsigned char banderas;
    banderas = 0;   

    caracter = lab[coordenada->x-1][coordenada->y];
    if(caracter=='*' || caracter=='B') 
        banderas |= ARRIBA;
    
    caracter = lab[coordenada->x+1][coordenada->y];
    if(caracter=='*' || caracter=='B') 
        banderas |= ABAJO;
    
    caracter = lab[coordenada->x][coordenada->y+1];
    if(caracter=='*' || caracter=='B')
        banderas |= DERECHA;

    caracter = lab[coordenada->x][coordenada->y-1];
    if(caracter=='*' || caracter=='B')
        banderas |= IZQUIERDA;
        
    return banderas;        
}


void imprimirPosibles(unsigned char caminos)
{
    if(caminos&ARRIBA)
        printf("\n ARRIBA");
    if(caminos&ABAJO)
        printf("\n ABAJO");
    if(caminos&IZQUIERDA)
        printf("\n IZQUIERDA");
    if(caminos&DERECHA)
        printf("\n DERECHA");
}