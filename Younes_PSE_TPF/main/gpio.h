#include "utils.h"

void salida_pin_portb(unsigned char numero);
void entrada_pin_portb(unsigned char numero);
void levantar_pin_portb(unsigned char numero);
void bajar_pin_portb(unsigned char numero);
unsigned int leer_pin_portb(unsigned char numero);

void salida_pin_portc(unsigned char numero);
void entrada_pin_portc(unsigned char numero);
void levantar_pin_portc(unsigned char numero);
void bajar_pin_portc(unsigned char numero);
unsigned int leer_pin_portc(unsigned char numero);

void salida_pin_portd(unsigned char numero);
void entrada_pin_portd(unsigned char numero);
void levantar_pin_portd(unsigned char numero);
void bajar_pin_portd(unsigned char numero);
unsigned int leer_pin_portd(unsigned char numero);

unsigned int portd_bit_es_1(unsigned int bit);
unsigned int portd_bit_es_0(unsigned char bit);
unsigned int portd_boton_pulsado(unsigned char bit);
