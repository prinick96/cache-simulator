/**

http://quidel.inele.ufro.cl/~jruiz/est2/Webeo2/MAPEO.html

MAPEO DIRECTO:
TAG = Memoria principal (en bytes) / memoria cache (bytes), en potencia de 2
BIC = Memoria caché (en bytes), en potencia de 2
WIB = bloques (en bytes), en potencia de 2

*/
void mapeo(int *config) {
  // Tipo de mapeo
  int tipoMapeo = config[0];
  // Cantidad de Memoria Principal
  int memP = config[1];
  // Cantidad de Memoria Caché
  int memC = config[2];
  // Bloques
  int Bloq = config[3];
}
