#include <xinu.h>

#include "main.h"

int tarea_control(void) {
    while (1) {
        sleepms(4000);
        sync_signal(SEM_TEMPERATURA);
        sleepms(4000);
        sync_signal(SEM_CAPACIDAD);
    }

    return 0;
}
