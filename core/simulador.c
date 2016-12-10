/**
  * Muestra en pantalla el estado de la caché de forma gráfica, dibujando la matriz.
  *
  * @param int **cache: Estructura de caché para mostrarla.
  * @param int C: Cantidad de conjuntos
  * @param int mC: Memoria del caché
*/
void showCache(int **cache, int C, int mC) {
  printf("            CACHE\n");
  for(int i = 0; i < C; i++) {
    printf("CONJUNTO %i:",i);
    for(int j = 0; j < mC; j++) {
      //if(cache[i][j] != 0) {
      //El -1 es el que especifica el vacío ahora
      if(cache[i][j] != -1) {
        printf("[%i]",cache[i][j]);
      } else {
        p("[  ]");
      }
    }
    p("\n");
  }
}

/**
  * Muestra en pantalla las estadísticas de la simulación.
  *
  * @param int *fallas: Arreglo con conteo de fallas (los tres tipos).
  * @param int solicitudes: Solicitudes realizadas en la simulacion
  * @param int aciertos (por referencia): Integer con conteo de aciertos.
*/
void showStats(int *fallas, int solicitudes, int aciertos) {

  int fallasTotal = fallas[0] + fallas[1] + fallas[2];
  float tasaFallas = (fallasTotal * 100) / solicitudes;
  float tasaAciertos = (aciertos * 100) / solicitudes;

  p("\n\nESTADISTICAS\n");
  printf("  -Total de solicitudes de cache: %i\n",solicitudes);
  printf("  -Fallas: %i\n",fallasTotal);
  printf("    +Fallas forzosas: %i\n",fallas[0]);
  printf("    +Fallas por capacidad: %i\n",fallas[1]);
  printf("    +Fallas por conflicto: %i\n",fallas[2]);
  printf("    +Tasa de fallas: %f \n", tasaFallas);
  printf("  -Aciertos:\n");
  printf("    +Total aciertos: %i\n",aciertos);
  printf("    +Tasa de aciertos: %f \n", tasaAciertos);
  p("\n\n");
}

/**
  * Política de reemplazo en CACHÉ, primero en entrar primero en salir.
  *
  * @param int **cache: Estructura de caché entera.
  * @param int *puntero: Arreglo de punteros para controlar el FIFO en cada conjunto.
  * @param int n: El número a buscar en caché.
  * @param int conjunto: Conjunto en el que pertenece n.
  * @param int mC: Memoria caché (para saber el extremo)
  * @param int *fallas: Arreglo con conteo de fallas (los tres tipos).
  * @param int *aciertos (por referencia): Integer con conteo de aciertos.
  *
*/
void FIFO(int **cache, int *puntero, int n, int conjunto, int mC, int *fallas, int *aciertos) {

  /*
    Si el puntero del conjunto correspondiente para el número es -1, cache[conjunto][0] está vacío
    y por tanto existe una falla forzosa para "n" en "cache"
  */
  if(puntero[conjunto] == -1) {
    //Incrementamos la falla forzosa
    fallas[0]++;
    printf("\n%i falla forzosa",n);

    //Metemos "n" en el caché en el conjunto que le corresponde (primer posición)
    cache[conjunto][0] = n;
    //Incrementamos el puntero para saber quien fue el primero que entró
    puntero[conjunto]++;
  }

  /*
    Si el puntero no es -1, entonces hay que verificar si "n" está en el caché.
    Aquí se identificará la falla forzosa y falla por conflicto.
    También se determina si hubo un acierto.
    Se coloca el elemento "n" en el caché.
    Se elimina el primero en entrar si el conjunto está full y se
    corre el puntero en caso de que haya falla por conflicto.
  */
  else {

    int counter = 0;

    //Buscamos en el conjunto desde 0 hasta mC - 1
    for(int i = 0; i < mC; i++) {

      //En caso de conseguirlo en el caché, incrementamos los aciertos y detemos la ejecución entera
      if(cache[conjunto][i] == n) {
        printf("\n%i acierto",n);
        *aciertos = *(aciertos) + 1;
        break;
      }

      //En caso de que no se consiga (se llega a alguna posición vacía), se introduce en caché y genera una falla forzosa
      //else if (cache[conjunto][i] == 0) {
      //Cambiamos a -1 la condición vacía
      else if (cache[conjunto][i] == -1) {
        printf("\n%i falla forzosa",n);
        cache[conjunto][i] = n;
        fallas[0]++;
        break;
      }
      //Contamos las vueltas del bucle en la búsqueda
      counter++;

    }

    /*
      Si recorrió toda la memoria caché en el conjunto, no hubo coincidencias y tampoco hubo espacio vacío.
      Entonces se genera una falla por conflicto y hay que sustituir al primero que entró y correr
      el puntero al que entro depués.
    */
    if(counter == mC) {
      //Generamos la falla por conflicto
      fallas[2]++;
      printf("\n%i falla por conflicto (-%i)",n,cache[conjunto][puntero[conjunto]]);

      //Sustituyo al primero que entró en el conjunto
      cache[conjunto][puntero[conjunto]] = n;
      //Corro el puntero hacia el siguiente
      puntero[conjunto]++;

      /*
        Si el puntero se sale de la memoria caché, lo regreso a la posición 0.
        Donde estará el primero que entró después de todos los otros primeros que
        cubrían la caché entera en el conjunto.
      */
      if(puntero[conjunto] >= mC) {
        puntero[conjunto] = 0;
      }
    }

  }

}

/**
  * Política de reemplazo en CACHÉ, se asigna de forma aleatoria una entrada.
  *
  * @param int **cache: Estructura de caché entera.
  * @param int n: El número a buscar en caché.
  * @param int conjunto: Conjunto en el que pertenece n.
  * @param int mC: Memoria caché (para saber el extremo)
  * @param int *fallas: Arreglo con conteo de fallas (los tres tipos).
  * @param int *aciertos (por referencia): Integer con conteo de aciertos.
*/
void RANDOM(int **cache, int n, int conjunto, int mC, int *fallas, int *aciertos) {
  int counter = 0;
  /*
    Recorremos la memoria caché en el conjunto indicado, buscando espacios vacíos.
    Aquí se identificará la falla forzosa y falla por conflicto.
    También se determina si hubo un acierto.
    Se coloca el elemento "n" en el caché.
    Se elimina de forma aleatoria un elemento, si el conjunto está full y no hay espacio para el nuevo elemento.
  */
  for(int i = 0; i < mC; i++) {
    /*
      Si la posición está vacía, entonces se asigna el elemento a esa posición,
      genera un falla forzosa y detenemos el recorrido del caché.
    */
    //if(cache[conjunto][i] == 0) {
    //Cambiamos el -1 la condición vacía
    if(cache[conjunto][i] == -1) {
      printf("\n%i falla forzosa",n);
      cache[conjunto][i] = n;
      fallas[0]++;
      break;
    }

    /*
      Si el elemento ya está en el caché, entonces generamos un acierto y detenemos
      el recorrido del caché.
    */
    else if(cache[conjunto][i] == n) {
      printf("\n%i acierto",n);
      *aciertos = *(aciertos) + 1;
      break;
    }

    counter++;
  }

  /*
    Si recorrió toda la memoria caché en el conjunto, no hubo coincidencias y tampoco hubo espacio vacío.
    Entonces se genera una falla por conflicto y hay que sustituir de forma aleatoria algún elemento por n.
  */
  if(counter == mC) {
    //Obtenemos una posición aleatoria
    int position = randomize(mC);

    //Generamos la falla por conflicto
    fallas[2]++;
    printf("\n%i falla por conflicto (-%i)",n,cache[conjunto][position]);

    cache[conjunto][position] = n;
  }

}

/**
  * Política de reemplazo en CACHÉ, se asigna en la posición del más reciente usado.
  *
  * @param int replace: Número a ser reemplazado
  * @param int **cache: Estructura de caché entera.
  * @param int n: El número a buscar en caché.
  * @param int conjunto: Conjunto en el que pertenece n.
  * @param int mC: Memoria caché (para saber el extremo)
  * @param int *fallas: Arreglo con conteo de fallas (los tres tipos).
  * @param int *aciertos (por referencia): Integer con conteo de aciertos.
*/
void LRU(int replace, int **cache, int n, int conjunto, int mC, int *fallas, int *aciertos) {
  int counter = 0;
  /*
    Recorremos la memoria caché en el conjunto indicado, buscando espacios vacíos.
    Aquí se identificará la falla forzosa y falla por conflicto.
    También se determina si hubo un acierto.
    Se coloca el elemento "n" en el caché.
    Se elimina, de acuerdo a LRU un elemento si el caché está full.
  */
  for(int i = 0; i < mC; i++) {
    /*
      Si la posición está vacía, entonces se asigna el elemento a esa posición,
      genera un falla forzosa y detenemos el recorrido del caché.
    */
    //if(cache[conjunto][i] == 0) {
    //Cambiamos por -1 la condición vacía
    if(cache[conjunto][i] == -1) {
      printf("\n%i falla forzosa",n);
      cache[conjunto][i] = n;
      fallas[0]++;
      break;
    }

    /*
      Si el elemento ya está en el caché, entonces generamos un acierto y detenemos
      el recorrido del caché.
    */
    else if(cache[conjunto][i] == n) {
      printf("\n%i acierto",n);
      *aciertos = *(aciertos) + 1;
      break;
    }

    counter++;
  }

  /*
    Si recorrió toda la memoria caché en el conjunto, no hubo coincidencias y tampoco hubo espacio vacío.
    Entonces se genera una falla por conflicto y hay que sustituir de forma aleatoria algún elemento por n.
  */
  if(counter == mC) {
    //Obtenemos la posición del número "getLRU()", en el conjunto
    int position = in_array_position(replace,cache[conjunto],mC);

    //Generamos la falla por conflicto
    fallas[2]++;
    printf("\n%i falla por conflicto (-%i)",n,cache[conjunto][position]);


    cache[conjunto][position] = n;
  }
}

/**
  * Reacomoda colocando el más reciente en entrar, en el tope y corre los que estén arriba
  * hacia abajo de el.
  *
  * @param int *vector: Vector parte del conjunto a ordenar.
  * @param int position: Posición del elemento a poner en tope.
  * @param int top: Tope a donde se desea mover.
*/
void reacomodar(int *vector, int position, int top) {
  //Tomamos el elemento a reacomodar
  int n = vector[position], i,j;
  //Si ya está en el tope no hacemos nada, pero si no está, hay que colocarlo
  if(position < top) {
    //Creamos un arreglo temporal
    int *tmp = (int *) malloc(sizeof(int) * top);
    //Tomamos los elementos que estén desde el hacia el tope.
    for(i = (position + 1), j = 0; i <= top; i++, j++) {
      tmp[j] = vector[i];
    }
    //Ahora los reemplazamos todos
    for(i = position, j = 0; i < top; i++, j++) {
      vector[i] = tmp[j];
    }
    //Colocamos de tope al elemento que tomamos
    vector[top] = n;
  }
}

/**
  * Devuelve el número que debe reemplazar LRU en el caché
  *
  * @paran int **puntero: Puntero que lleva el control de la pila. (se asume 0 como el fondo de la pila)
  * @param int C: Cantidad de conjuntos - 1
  * @param int conjunto: Conjunto en donde se ha de analizar.
  * @param int n: Número entrando en la secuencia actual.
  *
  * @return Devuelve el número que se debe reemplazar en el caché.
*/
int getLRU(int **puntero, int C, int conjunto, int n) {
  int existe = 0, position = 0;
  //Revisamos si está vacío o hay espacios vacíos
  for(int i = 0; i <= C; i++, position++) {
    //if(puntero[conjunto][i] == 0) {
    //Cambiamos a -1 la condición vacía
    if(puntero[conjunto][i] == -1) {
      //Si está vacío, la posición de inserción será i=position y detenemos el bucle;
      break;
    }
  }

  //Determinamos la existencia del elemento en el vector del conjunto de la matriz
  existe = in_array_position(n,puntero[conjunto],C + 1);
  //Si es cierto, no está lleno y el elemento NO está en el vector
  if(position < (C + 1) && -1 == existe) {
    //Por tanto simplemente colocamos n en el tope
    puntero[conjunto][position] = n;
  }

  //Si está lleno, o simplemente existe el elemento hay que reordenar el vector del conjunto
  else {

    //Si está lleno, position tiene un exceso en 1.
    if(position == C + 1) {
      position = C;
    }

    /*
      Si está lleno, y el elemento NO existe, hay que ELIMINAR el que está en la posición 0
      ya que este es el que será retornado y colocar allí el que se está introduciendo en
      la secuencia, para que así, este pase a estar de primero y se retorn aquí mismo el
      que estaba en la posición 0.
    */
    if(position == C && -1 == existe) {
      //Tomamos el que está en el fondo
      int tmp = puntero[conjunto][0];
      //Reemplazamos el del fondo por el que debe estar en el tope (el que entra en la secuencia)
      puntero[conjunto][0] = n;
      //Reacomodamos
      reacomodar(puntero[conjunto],0,position);

      //Termina el flujo y se retorna el elemento que se va a cambiar
      return tmp;
    }

    reacomodar(puntero[conjunto],existe,position);
  }

  //Al final siempre estará el elemento a reemplazar en la posición 0 (el fondo)
  return puntero[conjunto][0];
}

// Simulador
void simulador(int *config) {

  //Tamaño del caché (en bloques)
  int mC = expoDos(config[0]);
  //Conjuntos
  //int C = (int) ceil(config[0] / config[1]);
  int C = config[1];

  // Si el tamaño de conjuntos es mayor a cero, es porque se puede crear la memoria caché
  if(C > 0) {
    //Caché (Conjuntos x Memoria) (inicializa todas las posiciones con el valor cero por defecto)
    int **cache = create_cache_struct(C,mC);

    //Número de la secuencia
    //int number = -1;
    int number = 0;

    //Puntero de FIFO (vector con la altura de la cantidad de conjuntos)
    int *punteroFifo = NULL;
    punteroFifo = (int *) malloc(sizeof(int)*C);

    //Puntero de LRU (matriz de Cojunto x Conjunto)
    int **punteroLRU = create_cache_struct(C,mC);

    //Chequeamos el puntero LRU
    if(NULL == punteroLRU) {
      p("\n NO SE PUDO RESERVAR MEMORIA PARA COMENZAR CON LRU()\n");
      exit(EXIT_FAILURE);
    } else {
      //Rellenamos el puntero.
      for(int i = 0; i < C; i++) {
        for(int j = 0; j < mC; j++) {
          punteroLRU[i][j] = -1;
          //Aprovechamos y llenamos el caché de -1 en este mismo bucle
          cache[i][j] = -1;
        }
      }
    }

    //Chequeamos el puntero FIFO
    if(NULL == punteroFifo && 1 == config[2]) {
      p("\n NO SE PUDO RESERVAR MEMORIA PARA COMENZAR CON FIFO()\n");
      exit(EXIT_FAILURE);
    } else {
      //Rellenamos el puntero.
      for(int i = 0; i < C; i++) {
        punteroFifo[i] = -1;
      }
    }

    /** FALLAS:
      * [0]: Fallas forzosas
      * [1]: Fallas por capacidad
      * [2]: Fallas por conflicto
    */
    int fallas[3] = {0,0,0};
    int solicitudes = 0;
    int aciertos = 0;
    int conjunto_position = 0;
    int mapConfig[6] = {
      1, //mapeo asociativo por conjunto
      256, //memoria principal (de relleno nada más, no usable para el algoritmo)
      config[0], //tamaño de memoria cache
      1, //tamaño de cada bloque (porque la memoria caché está definida como tamaño en bloques)
      config[1], //tamaño del conjunto
    };

    system(CLS);

    char algoritmos[3][10] = {"LRU", "FIFO", "RAND"};

    printf("ALGORITMO DE REEMPLAZO: %s\n\n",algoritmos[config[2]]);
    //p("A continuacion introducir las secuencias, escribir la secuencia \"0\" para finalizar la simulacion.\n\n");
    p("A continuacion introducir las secuencias, escribir la secuencia \"-1\" para finalizar la simulacion.\n\n");

    // hacer secuencias
    //while (number != 0) {
    // Cambiamos la condición de salida a -1
    while (number != -1) {

      printf("Secuencia %i: ", solicitudes + 1);
      scanf("%d",&number);
      //Número
      mapConfig[5] = number;

      //Obtengo la posición en el conjunto que corresponda al número haciendo el mapeo del mismo
      conjunto_position = mapeo(mapConfig,1,0);

      //if(0 != number) {
      // Cambiamos la condición de salida a -1
      if(-1 != number) {

        if(conjunto_position < C) {
          //Política de reemplazo
          switch (config[2]) {
            case 0:
              LRU(
                getLRU(punteroLRU, mC - 1, conjunto_position, number),
                cache,
                number,
                conjunto_position,
                mC,
                fallas,
                &aciertos
              );
            break;
            case 1:
              FIFO(
                cache,
                punteroFifo,
                number,
                conjunto_position,
                mC,
                fallas,
                &aciertos
              );
            break;
            default:
              RANDOM(
                cache,
                number,
                conjunto_position,
                mC,
                fallas,
                &aciertos
              );
            break;
          }
        }

        //Si no existe conjunto posible para el bloque, generamos falla por capacidad
        else {
          printf("\n%i falla por capacidad",number);
          fallas[1]++;
        }


        p("\n\n");
        showCache(cache,C,mC);
        p("\n------------------------------------------------------------\n");

        solicitudes++;
      }

    }

    //Estadísticas
    showStats(fallas,solicitudes,aciertos);

    //Liberamos memoria
    free(cache);
    free(punteroFifo);
    free(punteroLRU);
  }

  // Si C == 0 entonces, la capacidad del caché es insuficiente para esa configuración de conjuntos
  else {
    p("\nLa memoria cache es muy pequenia para los conjuntos asignados.\n");
  }

  //Fin
  p("\nTermino la simulacion\n");

  returnMenu();

}
