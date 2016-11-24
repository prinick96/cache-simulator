#include "core/kernel.c"

// Módulo de mapeo (inicializador)
void m1() {

  int config[4];

  p("Tipo de mapeo (0) Directo (1) Asociativo: ");
  scanf("%i",&config[0]);
  p("\n");

  p("Memoria Principal en KB: ");
  scanf("%i",&config[1]);
  p("\n");

  p("Memoria Cache en Bytes: ");
  scanf("%i",&config[2]);
  p("\n");

  p("Tamanio del Bloque en Bytes: ");
  scanf("%i",&config[3]);
  p("\n");

}

// Módulo de bloques (inicializador)
void m2() {

  int config[5];

  p("Tipo de mapeo (0) Directo (1) Asociativo: ");
  scanf("%i",&config[0]);
  p("\n");

  p("Memoria Principal en KB: ");
  scanf("%i",&config[1]);
  p("\n");

  p("Memoria Cache en Bytes: ");
  scanf("%i",&config[2]);
  p("\n");

  p("Tamanio del Bloque en Bytes: ");
  scanf("%i",&config[3]);
  p("\n");

  p("Direccion: ");
  scanf("%i",&config[4]);
  p("\n");

}

// Simulador de reemplazo (inicializador)
void m3() {
  int config[3];
  char *secuencias;

  p("Memoria Cache en Bytes: ");
  scanf("%i",&config[0]);
  p("\n");

  p("Cantidad de conjuntos: ");
  scanf("%i",&config[1]);
  p("\n");

  p("Política de reemplazo (0) LRU (1) FIFO (2) RAND: ");
  scanf("%i",&config[2]);
  p("\n");

  p("Secuencia de direcciones (separar por comas): ");
  scanf("%s",&secuencias);
  p("\n");

}

// __ini__
int main() {

  system("cls");
  short int op;
  int options[3] = {1,2,3};

  do {
    p(" (0) Modulo de mapeo \n (1) Modulo de bloques \n (2) Simulador de reemplazo\n Seleccionar: ");
    scanf("%i",&op);

    system("cls");
    p("-----Indicar la configuración-----\n\n");

    switch (op) {
      case 0:
        m1();
      break;
      case 1:
        m2();
      break;
      case 2:
        m3();
      break;
      default:
        system("cls");
        p("Módulo inexistente. \n\n");
      break;
    }
  } while(!in_array(op,options,3));

  return 1;
}
