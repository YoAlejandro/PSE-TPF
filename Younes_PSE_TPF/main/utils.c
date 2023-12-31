#include "utils.h"

// espera n milisegundos
void delay_ms(unsigned int n) {
    // 1000 ms = 376470 iteraciones vacias (328P a 16MHz)
    volatile unsigned long i;
    // volatile unsigned long m = (n * 188235UL) / 1000UL;
    volatile unsigned long m = (n * 376470UL) / 1000UL;

    for (i = 1UL; i <= m; i++)
        ;
}

// espera 5 microsegundos
void delay_5us() {
    // 1000 us = 376 iteraciones vacias (328P a 16MHz)
    // 10 us * 376 iteraciones / 1000 us =~ 4 iteraciones
    volatile unsigned long i;
    // volatile unsigned long m = 1UL;
    volatile unsigned long m = 2UL;

    for (i = 1UL; i <= m; i++)
        ;
}

// espera 10 microsegundos
void delay_10us() {
    // 1000 us = 376 iteraciones vacias (328P a 16MHz)
    // 10 us * 376 iteraciones / 1000 us =~ 4 iteraciones
    volatile unsigned long i;
    // volatile unsigned long m = 2UL;
    volatile unsigned long m = 4UL;

    for (i = 1UL; i <= m; i++)
        ;
}

// espera n veces 10 microsegundos
void delay_10us_n(unsigned int n) {
    // 1000 us = 376 iteraciones vacias (328P a 16MHz)
    // 10 us * 376 iteraciones / 1000 us =~ 4 iteraciones
    volatile unsigned long i;
    // volatile unsigned long m = 2UL * n;
    volatile unsigned long m = 4UL * n;

    for (i = 1UL; i <= m; i++)
        ;
}
