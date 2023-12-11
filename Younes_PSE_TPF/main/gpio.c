#include "gpio.h"

volatile unsigned char* PINB = (unsigned char*)0x23;
volatile unsigned char* DDRB = (unsigned char*)0x24;
volatile unsigned char* PORTB = (unsigned char*)0x25;

volatile unsigned char* PINC = (unsigned char*)0x26;
volatile unsigned char* DDRC = (unsigned char*)0x27;
volatile unsigned char* PORTC = (unsigned char*)0x28;

volatile unsigned char* PIND = (unsigned char*)0x29;
volatile unsigned char* DDRD = (unsigned char*)0x2A;
volatile unsigned char* PORTD = (unsigned char*)0x2B;

void inicializar_portb() {
    *PORTB = 0b00000000;
    *DDRB = 0b00000000;
}

void salida_pin_portb(unsigned char numero) {
    *DDRB = *DDRB | numero;
}

void entrada_pin_portb(unsigned char numero) {
    *DDRB = *DDRB & ~numero;
}

void levantar_pin_portb(unsigned char numero) {
    *PORTB = *PORTB | numero;
}

void bajar_pin_portb(unsigned char numero) {
    *PORTB = *PORTB & ~numero;
}

unsigned int leer_pin_portb(unsigned char numero) {
    return *PINB & numero;
}

void inicializar_portc() {
    *PORTC = 0b00000000;
    *DDRC = 0b00000000;
}

void salida_pin_portc(unsigned char numero) {
    *DDRC = *DDRC | numero;
}

void entrada_pin_portc(unsigned char numero) {
    *DDRC = *DDRC & ~numero;
}

void levantar_pin_portc(unsigned char numero) {
    *PORTC = *PORTC | numero;
}

void bajar_pin_portc(unsigned char numero) {
    *PORTC = *PORTC & ~numero;
}

unsigned int leer_pin_portc(unsigned char numero) {
    return *PINC & numero;
}

void inicializar_portd() {
    *PORTD = 0b00000000;
    *DDRD = 0b00000000;
}

void salida_pin_portd(unsigned char numero) {
    *DDRD = *DDRD | numero;
}

void entrada_pin_portd(unsigned char numero) {
    *DDRD = *DDRD & ~numero;
}

void levantar_pin_portd(unsigned char numero) {
    *PORTD = *PORTD | numero;
}

void bajar_pin_portd(unsigned char numero) {
    *PORTD = *PORTD & ~numero;
}

unsigned int leer_pin_portd(unsigned char numero) {
    return *PIND & numero;
}

unsigned int portd_bit_es_1(unsigned int bit) {
    return *PIND & bit;
}

unsigned int portd_bit_es_0(unsigned char bit) {
    if ((*PIND & bit) == 0) {
        return 1;
    } else {
        return 0;
    }
}

unsigned int portd_boton_pulsado(unsigned char bit) {
    volatile unsigned int i;
    unsigned int pulsado;

    for (i = 0; i < 1000U; i++)         // botón debe pulsarse un ratito
        pulsado = portd_bit_es_0(bit);  // señal baja es pulsado

    return pulsado;
}

void portd_pitido(unsigned char bit,
                  unsigned long duracion,
                  unsigned int tono) {
    // señal levantada dura lo mismo que bajada para hacer un ciclo
    for (unsigned long j = 0; j < duracion; j++) {
        levantar_pin_portd(bit);
        for (unsigned int i = 0; i < tono; i++)
            delay_10us();
        bajar_pin_portd(bit);
        for (unsigned int i = 0; i < tono; i++)
            delay_10us();
    }
}
