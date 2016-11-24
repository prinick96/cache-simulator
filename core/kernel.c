#include <stdlib.h>
#include <stdio.h>

//Macros
#define TRUE 1
#define FALSE 0

//Instance
char **create_cache_struct(int fil, int col);
char *create_memory_struct(int N);
short int in_array(int e, int *vector, int N);
char *bin(int n, int BITS);
void p(char *string);
int main();

/**
  * Crea una estructura de tipo caché (matriz fil*col) en tiempo de ejecución.
  * Retorna una matriz NO dinámica, con fil y col de tamaño.
*/
char **create_cache_struct(int fil, int col){
    char **m = NULL;

    // Se crea la malla
    m = (char**) calloc(fil, sizeof(char *));

    //Se reserva memoria para la tabla de punteros intermedia
    if (NULL != m){
        for (int i= 0 ; i < fil; i++){
          m[i] = (char *) calloc(col, sizeof(char));
        }
    }

    return m;
}

/**
  * Crea una estructura de memoria principal (vector N) en tiempo de ejecución.
  * Retorna un vector de tamaño N.
*/
char *create_memory_struct(int N) {
  char *v = NULL;
  v = (char*) malloc(N*sizeof(char));

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

/**
  * Transforma un número decimal a un número binario.
  * @param int n: Entero a transformar
  * @param int BITS: Cantidad de bits de resultado (32 bits por ejemplo)
  * Retorna un long long en binario
*/
char *bin(int n, int BITS) {
   int c, d, count;
   char *pointer;

   count = 0;
   pointer = (char*)malloc(BITS+1);

   // Por si no hay memoria
   if ( NULL == pointer ) {
     exit(EXIT_FAILURE);
   }

   for ( c = BITS ; c >= 0 ; c-- )  {
      d = n >> c;

      if ( d & 1 ) {
        *(pointer+count) = 1 + '0';
      } else {
        *(pointer+count) = 0 + '0';
      }

      count++;
   }

   *(pointer+count) = '\0';

   return  pointer;
}

//Reducción de printf(STRING)
void p(char *string) {
  printf("%s",string);
}

//Inclusión de archivos
#include "mapeo.c"
#include "bloques.c"
#include "simulador.c"
