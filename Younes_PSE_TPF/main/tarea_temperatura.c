/*
 * tarea_temperatura: este programa hace sonar el parlante cuando la
 * temperatura de la batería es muy alta e imprime dicha situación
 * en la pantalla LCD.
 *
 * Se desarrolla como un programa aislado del resto.
 */

#include <xinu.h>

#include "main.h"

#define DURACION 1000
#define TONO 150

extern unsigned int pos_cursor;
extern unsigned int temperatura_alarma;

unsigned long temperatura;

int tarea_temperatura(void) {
    while (1) {
        temperatura = 0;
        sync_wait(SEM_TEMPERATURA);
        sleepms(4000);
        sync_wait(SEM_SMBUS);
        if (smbus_detectar_dispositivo(BATERIA))
            temperatura = smbus_leer_palabra(BATERIA, Cmd_Temperature);
        sync_signal(SEM_SMBUS);
        if (temperatura != 0)
            temperatura = ((temperatura / 10) - 273);
        if (temperatura > temperatura_alarma) {
            sync_wait(SEM_LCD);
            liquidCrystal_setCursor(0, 1);
            liquidCrystal_print("TEMP. ALTA!");
            liquidCrystal_setCursor(pos_cursor, 0);
            sync_signal(SEM_LCD);
            sync_wait(SEM_PARLANTE);
            for (unsigned long j = 0; j < DURACION; j++) {
                levantar_pin_portd(PIN_PARLANTE);
                sleepms(1);
                bajar_pin_portd(PIN_PARLANTE);
                sleepms(1);
            }
            sync_signal(SEM_PARLANTE);
        }
        sync_signal(SEM_CAPACIDAD);
    }

    return 0;
}
