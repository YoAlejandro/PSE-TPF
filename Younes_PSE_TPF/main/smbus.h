#include "i2cmaster.h"
#include "utils.h"

unsigned int smbus_detectar_dispositivo(unsigned int direccion);
unsigned int smbus_detectar();
void smbus_escribir_palabra(unsigned int direccion,
                            unsigned int comando,
                            unsigned int dato);
unsigned long smbus_leer_palabra(unsigned int direccion, unsigned int comando);
unsigned int smbus_leer_bloque(unsigned int direccion,
                               unsigned int comando,
                               char* bufferBloque,
                               unsigned int lonBufferBloque);
