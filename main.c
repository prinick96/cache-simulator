#include "core/kernel.c"

/**
  * Pide aspectos básicos para el módulo 1 y para el módulo 2.
  * @param int *config: Array de configuración vacío.
  *
  * @return arreglo de configuración en @param, lleno.
*/
int *basics(int *config) {

  int unity = 0;
  // Opciones posibles de los conjuntos
  int cOptions[3] = {2,4,8};
  // Opciones posibles de las unidades
  int uOptions[3] = {0,1,2};
  // Opciones posibles del tipo de mapeo
  int mOptions[2] = {0,1};
  // Opciones posibles de la memoria principal
  int mPOptions[10] = {
    32*1024, //32kb
    64*1024, //64kb
    256*1024, //256kb
    512*1024, //512kb
    1024*1024, //1Mb
    2*1024*1024, //2Mb
    4*1024*1024, //4Mb
    8*1024*1024, //8Mb
    16*1024*1024, //16Mb
    32*1024*1024 //32Mb
  };
  // Opciones posibles de la memoria caché
  int mCOptions[5] = {64,128,256,512,1024};
  //int mCOptions[5] = {32,128,256,512,1024};
  // Opciones posibles para los bloques
  int BOptions[5] = {2,4,8,16,32};
  //int BOptions[5] = {1,4,8,16,32};

  p("Tipo de mapeo (0) Directo (1) Asociativo: ");
  scanf("%i",&config[0]);
  //Chequeo de parámetros de entrada
  config = iReplace(config,mOptions,0,0,2);
  p("\n");

  //Revisar la unidad y asignarla
  getUnity(&unity,uOptions,"Memoria Principal");

  p("Memoria Principal: ");
  scanf("%i",&config[1]);
  //Transformo a la unidad elegida
  config[1] = setUnity(config[1],unity);

  //Chequeo de parámetros de entrada
  config = iReplace(config,mPOptions,256*1024,1,10);
  p("\n");

  //Revisar la unidad y asignarla
  getUnity(&unity,uOptions,"Memoria Cache");

  p("Memoria Cache: ");
  scanf("%i",&config[2]);
  //Transformo a la unidad elegida
  config[2] = setUnity(config[2],unity);
  //Chequeo de parámetros de entrada
  config = iReplace(config,mCOptions,64,2,5);
  p("\n");

  //Revisar la unidad y asignarla
  getUnity(&unity,uOptions,"Tamanio de Bloque");

  p("Tamanio del Bloque: ");
  scanf("%i",&config[3]);
  //Transformo a la unidad elegida
  config[3] = setUnity(config[3],unity);
  //Chequeo de parámetros de entrada
  config = iReplace(config,BOptions,2,3,5);
  p("\n");

  if(1 == config[0]) {
    p("Tamanio de conjuntos (2,4 u 8): ");
    scanf("%i",&config[4]);
    config = iReplace(config,cOptions,2,4,3);
  }

  return config;
}

/**
  * Solicita información básica y de dirección para el módulo 2.
  * @param int *config: Array de configuración vacío.
*/
void m2(int *config) {

  p("\nDireccion (en decimal): ");
  scanf("%i",&config[5]);
  p("\n");

  mapeo(config,1,1);

}

/**
  * Solicita información básica para el módulo 3.
  * @param int *config: Array de configuración vacío.
*/
void m3(int *config) {

  //Valores posibles de Memoria Caché (en Bytes)
  int mCOptions[6] = {4,8,16,32,64,128};
  // Conjuntos posibles
  int cOptions[4] = {2,4,8,16};
  // Políticas de reemplazo
  int rOptions[3] = {0,1,2};

  p("Memoria Cache (en bloques): ");
  scanf("%i",&config[0]);
  //Chequeo de parámetros de entrada
  config = iReplace(config,mCOptions,4,0,6);
  p("\n");

  p("Tamanio de conjuntos (2,4, 8 u 16): ");
  scanf("%i",&config[1]);
  config = iReplace(config,cOptions,2,1,4);
  p("\n");

  p("Politica de reemplazo (0) LRU (1) FIFO (2) RAND: ");
  scanf("%i",&config[2]);
  config = iReplace(config,rOptions,0,2,3);
  p("\n");

  simulador(config);

}

// __ini__
int main() {
  system(CLS);
  // Variable para capturar la opción del menú
  short int op;
  // Opciones posibles
  int options[3] = {1,2,3};
  // Configuración
  int config[6];

  // Estructura repetitiva del menú
  do {
    // Solicitar el módulo
    p(" (0) Modulo de mapeo \n (1) Modulo de bloques \n (2) Simulador de reemplazo\n Seleccionar: ");
    scanf("%i",&op);

    system(CLS);
    p("-----Indicar la configuracion-----\n\n");

    // Selector del módulo
    switch (op) {
      case 0:
        // Función de mapeo
        mapeo( basics(config) , 0 , 1);
      break;
      case 1:
        // Ubicación de bloques
        m2( basics(config) );
/*        config[0] = 1;
        config[1] = 64;
        config[2] = 64;
        config[3] = 1;
        config[4] = 16;
        config[5] = 33;
        mapeo( config, 1, 1);
*/
      break;
      case 2:
        // Simulador de reemplazo de bloques
        m3(config);
      break;
      default:
        // No existe el módulo solicitado a través de "short int op"
        system(CLS);
        p("Modulo inexistente. \n\n");
      break;
    }
  // Si lo elegido no está en el arreglo de opciones posibles {1,2,3}
  } while(!in_array(op,options,3));
  // PD: in_array() se define en core/kernel.c

  return 1;
}
