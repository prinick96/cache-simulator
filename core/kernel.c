#include <stdlib.h>
#include <stdio.h>

//Macros
#define TRUE 1
#define FALSE 0

//Instance
int **create_cache_struct(int fil, int col);
int *create_memory_struct(int N);
short int in_array(int e, int *vector, int N);
void p(char *string);
int main();

/**
  * Crea una estructura de tipo caché (matriz fil*col) en tiempo de ejecución.
  * Retorna una matriz NO dinámica, con fil y col de tamaño.
*/
int **create_cache_struct(int fil, int col){
    int **m = NULL;

    // Se crea la malla
    m = (int**) calloc(fil, sizeof(int *));

    //Se reserva memoria para la tabla de punteros intermedia
    if (NULL != m){
        for (int i= 0 ; i < fil; i++){
          m[i] = (int *) calloc(col, sizeof(int));
        }
    }

    return m;
}

/**
  * Crea una estructura de memoria principal (vector N) en tiempo de ejecución.
  * Retorna un vector de tamaño N.
*/
int *create_memory_struct(int N) {
  int *v = NULL;
  v = (int*) malloc(N*sizeof(int));

  return v;
}

/**
  * Define si un elemento entero está dentro de un vector que también es de tipo entero.
  * Devuelve el macro TRUE en caso de ser cierto, y el macro FALSE en caso de no estar.
*/
short int in_array(int e, int *vector, int N) {
  for(int i = 0; i < N; i++) {
    if(e == vector[i]) {
      return TRUE;
    }
  }

  return FALSE;
}

//Reducción de printf(STRING)
void p(char *string) {
  printf("%s",string);
}

//Inclusión de archivos
#include "mapeo.c"
#include "bloques.c"
#include "simulador.c"
