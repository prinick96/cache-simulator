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
      if(cache[i][j] != 0) {
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

  p("\n\nESTADISTICAS\n");
  printf("  -Total de solicitudes de cache: %i\n",solicitudes);
  printf("  -Fallas: %i\n",fallasTotal);
  printf("    +Fallas forzosas: %i\n",fallas[0]);
  printf("    +Fallas por capacidad: %i\n",fallas[1]);
  printf("    +Fallas por conflicto: %i\n",fallas[2]);
  printf("    +Tasa de fallas: %f\n", ((fallasTotal/solicitudes)*100));
  printf("  -Aciertos:\n");
  printf("    +Total aciertos: %i\n",aciertos);
  printf("    +Tasa de aciertos: %f\n", ((aciertos/solicitudes)*100));
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
    Se elimina el primero en entrar y se corre el puntero en caso de que haya falla por conflicto.
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
      else if (cache[conjunto][i] == 0) {
        printf("\n%i falla forzosa",n);
        cache[conjunto][i] = n;
        fallas[0]++;
        break;
      }
      //Contamos las vueltas del bucle en la búsqueda
      counter++;

    }

    /*
      Si recorrió toda la memoria caché, no hubo coincidencias y tampoco hubo espacio vacío.
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

void LRU() {

}

void RAND() {

}

void simulador(int *config) {

  //Tamaño del caché (en bloques)
  int mC = expoDos(config[0]);
  //Conjuntos
  int C = config[1];
  //Caché (Conjuntos x Memoria) (inicializa todas las posiciones con el valor cero por defecto)
  int **cache = create_cache_struct(C,mC);

  //Número de la secuencia
  int number = -1;

  //Puntero de FIFO (vector con la altura de la cantidad de conjuntos)
  int *punteroFifo = NULL;
  punteroFifo = (int *) malloc(sizeof(int)*C);

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
  p("A continuacion introducir las secuencias, escribir la secuencia \"0\" para finalizar la simulacion.\n\n");

  // hacer secuencias
  while (number != 0) {

    printf("Secuencia %i: ", solicitudes + 1);
    scanf("%d",&number);
    //Número
    mapConfig[5] = number;

    //Obtengo la posición en el conjunto que corresponda al número haciendo el mapeo del mismo
    conjunto_position = mapeo(mapConfig,1,0);

    if(0 != number) {

      //Política de reemplazo
      switch (config[2]) {
        case 0:
          LRU();
        break;
        case 1:
          FIFO(cache,punteroFifo,number,conjunto_position,mC,fallas,&aciertos);
        break;
        default:
          RAND();
        break;
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

  //Fin
  p("\nTermino la simulacion");

}
