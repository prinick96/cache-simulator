/**
  * Muestra internamente (posición/contenido) el bloque de caché que se esté pidiendo.
  *
  * @param char *cache: Caché entero.
  * @param int begin: Posición de inicio
  * @param int end: Posición final
  * @param int showInternal: (1) Muestra contenido interno, (0) No muestra contenido interno
*/
void showBinaries(char *cache, int begin, int end, int showInternal) {
  for(int i = begin; i >= end; i--) {
    // Si showInternal es 1, se muestra el binario dentro de la posición
    if(1 == showInternal) {
      printf("[%c]",cache[i]);
    // Si no, solo se muestra la posición
    } else {
      printf("[%i]",i);
    }
  }
  p("\n");
}

/**
  * Muestra la estructura interna del caché generado.
  *
  * @param char *cache: Estructrua de caché
  * @param int size: Tamaño de la estructura caché
  * @param int showInternal: (1) Muestra contenido interno, (0) No muestra contenido interno
  * @param int *component: Componentes (TAG,BIC/SIC,WIB)
  * @param int is_sic: Verifica si el componente 1 es un sic o un bic
  * @param int dir: Dirección introducida en el main()
  * @param int showMessages: Dice si va a mostrar mensajes en pantalla o no.
  *
  * @return int position: Retorna la posición del conjunto, si es mapeo directo devuelve 0.
*/
int showCacheStruct(char *cache, int size, int showInternal, const int *component, const int is_sic, int dir, int showMessages) {

  if(1 == showInternal) {
    //Obtengo un binario de "size" bits completado con ceros a la izquierda
    char *mem_dir = bin(dir,size - 1);

    //LLenamos el caché
    for(int i = size - 1; i >= 0; i--) {
      cache[i] = mem_dir[(size - 1) - i];
    }
  }

  // En caso de que se quiera mostrar información en la pantalla
  if(1 == showMessages) {
    //Muestra la parte del TAG
    p("ETIQUETA: ");
    showBinaries(cache,size - 1,size - component[0],showInternal);

    //Muestra la parte del SIC / BIC según tipo de mapeo
    p(is_sic ? "CONJUNTO: " : "BLOQUE: ");
    showBinaries(cache,size - component[0] - 1,component[2],showInternal);

    //Muestra la parte del WIB
    p("BYTE: ");
    showBinaries(cache,component[2] - 1,0,showInternal);
  }

  /** Si showInternal es 1 es porque estamos en el módulo 2,
    * donde hay que indicar la dirección del bloque/conjunto
    * en la memoria caché
  */
  if(1 == showInternal) {
    //Meteremos el binario aquí
    char mem_cache[250];

    //Declaramos los contadores para el for
    int i,y;

    //Obtener la parte del conjunto
    for(i = size - component[0] - 1, y = 0; i >= component[2]; i--, y++) {
      mem_cache[y] = (char) cache[i];
    }

    int position = binToInt(mem_cache, component[1]);

    if(1 == showMessages) {
      printf("\n\nEl bloque se coloca en el %s %i de la memoria cache.\n", is_sic ? "conjunto" : "bloque" , position);
    }

    return position;
  }

  p("\n\n");

  return 0;
}

/**

http://quidel.inele.ufro.cl/~jruiz/est2/Webeo2/MAPEO.html

MAPEO DIRECTO:
TAG = Memoria principal (en bytes) / memoria cache (bytes), en potencia de 2
BIC = Memoria caché (en bytes) / bloques (en bytes), en potencia de 2
WIB = bloques (en bytes), en potencia de 2

Se representa como
TAG | BIC | WIB

MAPEO ASOCIATIVO POR CONJUNTO:
TAG = Memoria caché (en bytes), en potencia de 2
SIC = (Memoria caché (en bytes) / bloques (en bytes)) / tamaño de conjunto, en potencia de 2
WIB = Bloques (en bytes), en potencia de 2

Se representa como
TAG | SIC | WIB

  @param int *config: Configuración pedida en el main()
  @param int showInternal: (1) Muestra contenido interno, (0) No muestra contenido interno

  @return int position: Retorna la posición retornada por showCacheStruct()

*/
int mapeo(int *config, int showInternal, int showMessages) {

  int mP = config[1];
  int mC = config[2];
  int B = config[3];
  int component[3];

  int WIB = expoDos(B), //Byte
      TAG, BIC, SIC, sizeMemory;

  //----------------MAPEO DIRECTO-------------------//
  if(0 == config[0]) {

    TAG = expoDos((int) ceil(mP / mC)); //Etiqueta
    BIC = expoDos((int) ceil(mC / B)); //Línea

    // Tamaño del mapeo
    sizeMemory = TAG + BIC + WIB;

    // Componente 2 (Línea)
    component[1] = BIC;

  //----------------MAPEO ASOCIATIVO POR CONJUNTO-------------------//
  } else {

    TAG = expoDos(mC); //Etiqueta
    //config[4]: Tamaño de conjunto
    SIC = expoDos((int) ceil(((int) ceil(mC / B)) / config[4]) ); //Conjunto

    // Tamaño del mapeo
    sizeMemory = TAG + SIC + WIB;

    // Componente 2 (Conjunto)
    component[1] = SIC;
  }

  component[0] = TAG;
  component[2] = WIB;

  // Crear el caché correspondiente
  char *cache = create_memory_struct(sizeMemory);

  // Mostrar la estructura final
  int position = showCacheStruct(
      cache, //caché creado
      sizeMemory, //tamaño del caché creado
      showInternal, //mostrar internamente config[5] (si existe)
      component, //tag,sic/bic,wib
      config[0], //mapeo directo/asociativo por conjunto
      1 == showInternal ? config[5] : 0, //direccion de búsqueda (si es que se solicitó buscar una dirección),
      showMessages // Si queremos mostrar el mensaje
  );

  //Liberamos memoria
  free(cache);

  if(1 == showMessages) {
    // Verificar si se quiere regresar al menú
    p("\n\n");
    returnMenu();
  }

  //Retornamos la posición del conjunto
  return position;
}
