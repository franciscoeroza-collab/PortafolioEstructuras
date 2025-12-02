#ifndef ALUMNO_H
#define ALUMNO_H

typedef struct
{
    unsigned int matricula;
    char nombre[64];
    int semestres;
    float promedio;
} Alumno;

Alumno* crearAlumno(unsigned int mat, const char* nombre, int sem, float prom);

// Funciones de impresión
void imprimirAlumno(void* a);

// Comparadores
int compararPorMatricula(void* a, void* b);
int compararPorNombre(void* a, void* b);
int compararPorSemestres(void* a, void* b);
int compararPorPromedio(void* a, void* b);

#endif