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
#define TEMPERATURA_ALARMA 25

extern unsigned int pos_cursor;

int tarea_temperatura(void) {
    while (1) {
        sync_wait(SEM_TEMPERATURA);
        sleepms(4000);
        sync_wait(SEM_SMBUS);
        unsigned long temperatura =
            smbus_leer_palabra(BATERIA, Cmd_Temperature);
        sync_signal(SEM_SMBUS);
        temperatura = ((temperatura / 10) - 273);
        if (temperatura > TEMPERATURA_ALARMA) {
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
