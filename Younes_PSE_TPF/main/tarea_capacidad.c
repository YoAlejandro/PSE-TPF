/*
 * tarea_capacidad: este programa hace sonar el parlante cuando la
 * capacidad de la bateria es muy baja.
 *
 * Se desarrolla como un programa aislado del resto.
 */

#include <xinu.h>

#include "main.h"

#define DURACION 200
#define TONO 200

extern unsigned int pos_cursor;

int tarea_capacidad(void) {
    while (1) {
        sync_wait(SEM_CAPACIDAD);
        unsigned long capacidad_restante =
            smbus_leer_palabra(BATERIA, Cmd_RemainingCapacity);
        unsigned long capacidad_alarma =
            smbus_leer_palabra(BATERIA, Cmd_RemainingCapacityAlarm);
        if (capacidad_restante < capacidad_alarma) {
            liquidCrystal_setCursor(0, 1);
            liquidCrystal_print("CAPA. BAJA!");
            portd_pitido(PIN_PARLANTE, DURACION, TONO);
            liquidCrystal_setCursor(pos_cursor, 0);
        }
    }

    return 0;
}
