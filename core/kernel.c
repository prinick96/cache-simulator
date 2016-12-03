#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h> //para meter en el random

//Macros
#define TRUE 1
#define FALSE 0
#define CLS "cls"

//Instance
void getUnity(int *unity, int *uOptions, char *type);
int setUnity(int N, int U);
int expoDos(int N);
int *iReplace(int *config, int *options, int D, int N, int nOptions);
int **create_cache_struct(int fil, int col);
char *create_memory_struct(int N);
int in_array_position(int e, int *vector, int N);
short int in_array(int e, int *vector, int N);
char *bin(int n, int BITS);
int binToInt(char *bin, int size);
void p(char *string);
int main();
void returnMenu();
int randomize(int cantidad);

/**
  * Obtiene el exponente de un número que se puede expresar en base 2
  *
  * @param int N: Recibe un número que se puede expresar en base de 2^n
  *
  * @return devuelve el exponente n
*/
int expoDos(int N) {
  int n = 0;

  for(N = N; N > 0; N = (int) ceil(N / 2)) {
    n++;
  }

  return n - 1;
}

/**
  * Crea una estructura de tipo caché (matriz fil*col) en tiempo de ejecución.
  *
  * @param int fil: Tamaño de las filas
  * @param int col: Tamaño de las columnas
  *
  * @return int **: Retorna una matriz NO dinámica, con fil y col de tamaño.
*/
int **create_cache_struct(int fil, int col) {
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
  * Crea una estructura de memoria (vector N) en tiempo de ejecución.
  *
  * @param int N: N es el tamaño del vector a crear.
  *
  * @return char *: Retorna un vector de tamaño N.
*/
char *create_memory_struct(int N) {
  char *v = NULL;
  v = (char*) malloc(N*sizeof(char));

  return v;
}

/**
  * Define si un elemento entero está dentro de un vector que también es de tipo entero.
  *
  * @param int e: Elemento a buscar
  * @param int *vector: Vector en el que se va a buscar
  * @param int N: Tamaño del vector pasado por int *vector
  *
  * @return int: Devuelve la posición en la que se encuentra, en caso de que no se encuentre devuelve -1.
*/
int in_array_position(int e, int *vector, int N) {
  for(int i = 0; i < N; i++) {
    if(e == vector[i]) {
      return i;
    }
  }

  return -1;
}

/**
  * Define si un elemento entero está dentro de un vector que también es de tipo entero.
  *
  * @param int e: Elemento a buscar
  * @param int *vector: Vector en el que se va a buscar
  * @param int N: Tamaño del vector pasado por int *vector
  *
  * @return int: Devuelve el macro TRUE en caso de ser cierto, y el macro FALSE en caso de no estar.
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
  * Convierte un número binario en un entero.
  *
  * @param char *bin: Binario a convertir
  * @param int size: Tamaño del binario
  *
  * @return decimal resultante.
*/
int binToInt(char *bin, int size) {
  int dec = 0;

  for(int i = 0; i < size; i++) {
    if(bin[i] == '1') {
      dec = dec * 2 + 1;
    } else {
      dec *= 2;
    }
  }

  return dec;
}

/**
  * Transforma un número decimal a un número binario.
  *
  * @param int n: Entero a transformar
  * @param int BITS: Cantidad de bits de resultado (32 bits por ejemplo)
  *
  * @return char *: Retorna un char * de tamaño BITS + 1 y el binario con ceros a la izquierda
*/
char *bin(int n, int BITS) {
   int c, d, count = 0;
   char *pointer = (char*)malloc(sizeof(char *)*(BITS+1));

   // Por si no hay memoria
   if ( NULL == pointer ) {
     p("\nNo hay memoria para reservar el numero binario bin(int n, int BITS).");
     exit(EXIT_FAILURE);
   }

   for ( c = BITS ; c >= 0 ; c-- )  {
      // desplazamos bits hacia la derecha
      d = n >> c;

      /*
        Operación "AND" lógica, hace con los bits de "d" y "1" lo que pasaría si los colocamos en
        una tabla AND de lógica, sólo cuando sea 1 y 1 cada bit, dará 1.
      */
      if ( d & 1 ) {
        *(pointer + count) = 1 + '0';
      } else {
        *(pointer + count) = 0 + '0';
      }

      count++;
   }

   // Cerramos la cadena con el caracter null
   *(pointer + count) = '\0';

   return pointer;
}

/**
  * Reemplaza un valor no permitido por uno de defecto.
  *
  * @param int *config: Vector de configuraciones
  * @param int *options: Opciones posibles
  * @param int D: Valor por defecto
  * @param int N: Posición en *config
  * @param int nOptions: Tamaño del vector options
*/
int *iReplace(int *config, int *options, int D, int N, int nOptions) {
  if(!in_array(config[N],options,nOptions)) {
    printf("Los %i bytes no son una cantidad permitida, se ha colocado %i bytes\n",config[N],D);
    config[N] = D;
  }

  return config;
}

/**
  * Transforma un número a la unidad correspondiente.
  *
  * @param int N: Número
  * @param int N: Unidad a transformar
  *
  * @return Retorna un entero transformado a la unidad
*/
int setUnity(int N, int U) {
  switch (U) {
    case 1:
      N = N * 1024;
    break;
    case 2:
      N = N * 1024 * 1024;
    break;
  }

  return N;
}

/**
  * Pregunta por la unidad y la coloca.
  *
  * @param int unity: Unidad (por referencia)
  * @param int *uOptions: Opciones de unidad
  * @param char *type: Mensaje delcarado
*/
void getUnity(int *unity, int *uOptions, char *type) {
  printf("Seleccionar unidad de %s: \n (0) B, (1) KB, (2) MB: ",type);
  scanf("%i",unity);
  if(!in_array(*unity,uOptions,3)) {
    *unity = 0;
  }
}

/**
  * Reducción de printf(STRING)
  *
  * @param char *string: String a escribir
*/
void p(char *string) {
  printf("%s",string);
}

/**
  * Retorna de forma recursiva al menú inicial
*/
void returnMenu(){
  p("Para volver al menu introducir (1), para salir introducir (0): ");
  short int o;
  scanf("%i",&o);

  if(o == 1) {
    system(CLS);
    main();
  } else {
    exit(1);
  }
}

/**
  * Genera un número random entre 0 y cantidad-1
  *
  * @param int cantidad: Cantidad hasta donde se desea generar el random
  *
  * @return retorna un entero con el número random
*/
int randomize(int cantidad) {
  srand(time(NULL));
  return rand() % cantidad;
}

//Inclusión de archivos
#include "mapeo.c"
#include "simulador.c"
