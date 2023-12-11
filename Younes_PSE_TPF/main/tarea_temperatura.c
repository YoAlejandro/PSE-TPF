/*
 * tarea_temperatura: este programa hace sonar el parlante cuando la
 * temperatura de la bateria es muy alta.
 *
 * Se desarrolla como un programa aislado del resto.
 */

#include <xinu.h>

#include "main.h"

#define DURACION 200
#define TONO 100
#define TEMPERATURA_ALARMA 27

extern unsigned int pos_cursor;

int tarea_temperatura(void) {
    while (1) {
        sync_wait(SEM_TEMPERATURA);
        unsigned long temperatura =
            smbus_leer_palabra(BATERIA, Cmd_Temperature);
        temperatura = ((temperatura / 10) - 273);
        if (temperatura > TEMPERATURA_ALARMA) {
            liquidCrystal_setCursor(0, 1);
            liquidCrystal_print("TEMP. ALTA!");
            portd_pitido(PIN_PARLANTE, DURACION, TONO);
            liquidCrystal_setCursor(pos_cursor, 0);
        }
    }

    return 0;
}
