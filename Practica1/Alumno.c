#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Alumno.h"

Alumno* crearAlumno(unsigned int mat, const char* nombre, int sem, float prom)
{
    Alumno *nuevo = malloc(sizeof(Alumno));
    nuevo->matricula = mat;
    strncpy(nuevo->nombre, nombre, 63);
    nuevo->semestres = sem;
    nuevo->promedio = prom;
    return nuevo;
}

void imprimirAlumno(void* a)
{
    Alumno *al = (Alumno*)a;
    printf("Matricula: %u | Nombre: %s | Semestres: %d | Promedio: %.2f\n",
           al->matricula, al->nombre, al->semestres, al->promedio);
}

int compararPorMatricula(void* a, void* b)
{
    Alumno *aa = (Alumno*)a;
    Alumno *ab = (Alumno*)b;

    if (aa->matricula < ab->matricula) return -1;
    if (aa->matricula > ab->matricula) return 1;
    return 0;
}

int compararPorSemestres(void* a, void* b)
{
    Alumno *aa = (Alumno*)a;
    Alumno *ab = (Alumno*)b;

    if (aa->semestres < ab->semestres) return -1;
    if (aa->semestres > ab->semestres) return 1;
    return 0;
}

int compararPorNombre(void* a, void* b)
{
    Alumno *aa = a, *ab = b;
    return strcmp(aa->nombre, ab->nombre);
}


int compararPorPromedio(void* a, void* b)
{
    Alumno *aa = a, *ab = b;
    if (aa->promedio < ab->promedio) return -1;
    else if (aa->promedio > ab->promedio) return 1;
    else return 0;
}