#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lista.h"
#include "Alumno.h"

int main(void)
{
    Lista lista;
    lista.inicio = NULL;
    lista.cant = 0;
    lista.imprimir = &imprimirAlumno;
    lista.comparar = &compararPorMatricula; // default: por matrícula

    int opcion;
    do
    {
            printf("\n--- MENU ---\n");
            printf("1. Registrar Alumno\n");
            printf("2. Desplegar Alumnos\n");
            printf("3. Reordenar Lista\n");
            printf("4. Buscar Alumno\n");
            printf("5. Borrar Alumno\n");
            printf("6. Salir\n");
            printf("Opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpiar buffer

        if (opcion == 1)
        {
            unsigned int mat;
            char nombre[64];
            int sem;
            float prom;

            printf("Matricula: "); 
            scanf("%u", &mat);
            getchar();
            printf("Nombre: "); 
            fgets(nombre, 64, stdin);
            nombre[strcspn(nombre, "\n")] = '\0';
            printf("Semestres: "); 
            scanf("%d", &sem);
            printf("Promedio: "); 
            scanf("%f", &prom);

            Alumno *nuevo = crearAlumno(mat, nombre, sem, prom);

            // Validar matrícula única
            if (buscarDato(lista, nuevo) == NULL)
                insertarOrdenado(&lista, nuevo);
            else
            {
                printf("Error: matrícula ya registrada.\n");
                free(nuevo);
            }
        }
        else if (opcion == 2)
        {
            mostrarLista(lista);
        }
        else if (opcion == 3)
        {
            int criterio;
            printf("Reordenar por: 1.Matricula 2.Nombre 3.Semestres 4.Promedio: ");
            scanf("%d", &criterio);

            switch (criterio)
            {
                case 1: lista.comparar = &compararPorMatricula; break;
                case 2: lista.comparar = &compararPorNombre; break;
                case 3: lista.comparar = &compararPorSemestres; break;
                case 4: lista.comparar = &compararPorPromedio; break;
                default: printf("Opcion invalida\n"); continue;
            }

            // reconstruir la lista con el nuevo orden
            Lista aux = {NULL, lista.comparar, lista.imprimir, 0};
            Nodo *p;
            for (p = lista.inicio; p != NULL; p = p->sig)
            {
                Alumno *al = (Alumno*)p->dato;
                Alumno *copia = crearAlumno(al->matricula, al->nombre, al->semestres, al->promedio);
                insertarOrdenado(&aux, copia);
            }
            borrarLista(&lista);
            lista = aux;
        }
        else if (opcion == 4)
        {
            int criterio;
            printf("Buscar por: 1.Matricula 2.Nombre 3.Semestres 4.Promedio: ");
            scanf("%d", &criterio);
            getchar();

            Alumno key;
            key.matricula = 0;
            key.nombre[0] = '\0';
            key.semestres = 0;
            key.promedio = 0.0f;

            switch (criterio)
            {
                case 1:
                    lista.comparar = &compararPorMatricula;
                    printf("Matricula: "); scanf("%u", &key.matricula);
                    break;
                case 2:
                    lista.comparar = &compararPorNombre;
                    printf("Nombre: "); fgets(key.nombre, 64, stdin);
                    key.nombre[strcspn(key.nombre, "\n")] = '\0';
                    break;
                case 3:
                    lista.comparar = &compararPorSemestres;
                    printf("Semestres: "); scanf("%d", &key.semestres);
                    break;
                case 4:
                    lista.comparar = &compararPorPromedio;
                    printf("Promedio: "); scanf("%f", &key.promedio);
                    break;
                default:
                    printf("Opcion invalida\n");
                    continue;
            }

            Alumno *res = buscarDato(lista, &key);
            if (res)
                imprimirAlumno(res);
            else
                printf("Alumno no encontrado\n");
        }
        else if (opcion == 5)
        {
            unsigned int mat;
            printf("Matricula a eliminar: ");
            scanf("%u", &mat);
            Alumno key = {mat, "", 0, 0};
            lista.comparar = &compararPorMatricula;
            borrarDato(&lista, &key);
        }

    } while (opcion != 6); 

    borrarLista(&lista);
    return 0;
}