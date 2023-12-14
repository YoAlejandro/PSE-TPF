/*
 * tarea_capacidad: este programa hace sonar el parlante cuando la
 * capacidad de la batería es muy baja e imprime dicha situación
 * en la pantalla LCD.
 *
 * Se desarrolla como un programa aislado del resto.
 */

#include <xinu.h>

#include "main.h"

#define DURACION 500
#define TONO 300

extern unsigned int pos_cursor;

unsigned long capacidad_restante;
unsigned long capacidad_alarma;

int tarea_capacidad(void) {
    while (1) {
        capacidad_restante = 0;
        capacidad_alarma = 0;
        sync_wait(SEM_CAPACIDAD);
        sleepms(4000);
        sync_wait(SEM_SMBUS);
        if (smbus_detectar_dispositivo(BATERIA)) {
            capacidad_restante =
                smbus_leer_palabra(BATERIA, Cmd_RemainingCapacity);
            capacidad_alarma =
                smbus_leer_palabra(BATERIA, Cmd_RemainingCapacityAlarm);
        }
        sync_signal(SEM_SMBUS);
        if (capacidad_restante < capacidad_alarma) {
            sync_wait(SEM_LCD);
            liquidCrystal_setCursor(0, 1);
            liquidCrystal_print("CAPA. BAJA!");
            liquidCrystal_setCursor(pos_cursor, 0);
            sync_signal(SEM_LCD);
            sync_wait(SEM_PARLANTE);
            for (unsigned long j = 0; j < DURACION; j++) {
                levantar_pin_portd(PIN_PARLANTE);
                sleepms(2);
                bajar_pin_portd(PIN_PARLANTE);
                sleepms(2);
            }
            sync_signal(SEM_PARLANTE);
        }
        sync_signal(SEM_TEMPERATURA);
    }

    return 0;
}
