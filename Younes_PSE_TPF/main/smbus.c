#include "smbus.h"

unsigned int smbus_detectar_dispositivo(unsigned int direccion) {
    direccion <<= 1;  // Hacer espacio al bit de lectura/escritura
    if (i2c_start(direccion + I2C_WRITE)) {  // Dispositivo inaccesible
        i2c_stop();
        delay_ms(100);
        return 0;
    } else {  // Dispositivo accesible
        i2c_write(0x00);
        i2c_rep_start(direccion + I2C_READ);
        i2c_readAck();
        i2c_readNak();
        i2c_stop();
        delay_ms(100);
        return 1;
    }
}

unsigned int smbus_detectar() {
    unsigned int encontrado = 0;
    unsigned int direccion = 0;
    while (encontrado == 0) {
        encontrado = smbus_detectar_dispositivo(direccion);
        direccion = direccion + 1;
        if (direccion == 256) {
            direccion = 0;
        }
    }
    return encontrado;
}

void smbus_escribir_palabra(unsigned int direccion,
                            unsigned int comando,
                            unsigned int dato) {
    i2c_start((direccion << 1) + I2C_WRITE);
    i2c_write(comando);
    i2c_write((unsigned int)dato);
    i2c_write((unsigned int)(dato >> 8));
    i2c_stop();
    return;
}

unsigned long smbus_leer_palabra(unsigned int direccion, unsigned int comando) {
    unsigned long buffer = 0;
    i2c_start((direccion << 1) + I2C_WRITE);
    i2c_write(comando);
    i2c_rep_start((direccion << 1) + I2C_READ);
    buffer = i2c_readAck();
    buffer += i2c_readNak() << 8;
    i2c_stop();
    return buffer;
}

unsigned int smbus_leer_bloque(unsigned int direccion,
                               unsigned int comando,
                               char* bufferBloque,
                               unsigned int lonBufferBloque) {
    unsigned int x, num_bytes;
    i2c_start((direccion << 1) + I2C_WRITE);
    i2c_write(comando);
    i2c_rep_start((direccion << 1) + I2C_READ);
    num_bytes = i2c_readAck();  // Num de bytes, el primer bit será indice 0
    num_bytes = constrain(num_bytes, 0,
                          lonBufferBloque - 2);  // Espacio para null al final
    for (x = 0; x < num_bytes - 1;
         x++) {  // -1 porque x = num_bytes-1 si x < y, el último bit debe ser
                 // nack, x < y-1
        bufferBloque[x] = i2c_readAck();
    }
    bufferBloque[x++] = i2c_readNak();  // nack el último bit y almacenarlo en
                                        // la dirección num_bytes-1 de x
    bufferBloque[x] = 0;  // null en último bit+1 para mostrarlo por LCD
    i2c_stop();
    return num_bytes;
}
