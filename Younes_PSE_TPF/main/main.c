#include "main.h"

/* definicion de tareas (cuyo codigo fuente es externo) */
extern int tarea_capacidad(void);
extern int tarea_temperatura(void);

unsigned int CMD_CON[][2] = {{Cmd_RemainingCapacityAlarm, BAT_MAH},
                             {Cmd_RemainingTimeAlarm, BAT_MINUTES},
                             {Cmd_Temperature, BAT_TENTH_K},
                             {Cmd_Voltage, BAT_MV},
                             {Cmd_Current, BAT_MA},
                             {Cmd_AverageCurrent, BAT_MA},
                             {Cmd_MaxError, BAT_PERCENT},
                             {Cmd_RelativeStateOfCharge, BAT_PERCENT},
                             {Cmd_AbsoluteStateOfCharge, BAT_PERCENT},
                             {Cmd_RemainingCapacity, BAT_MAH},
                             {Cmd_FullChargeCapacity, BAT_MAH},
                             {Cmd_RunTimeToEmpty, BAT_MINUTES},
                             {Cmd_AverageTimeToEmpty, BAT_MINUTES},
                             {Cmd_AverageTimeToFull, BAT_MINUTES},
                             {Cmd_ChargingCurrent, BAT_MA},
                             {Cmd_ChargingVoltage, BAT_MV},
                             {Cmd_CycleCount, BAT_DEC},
                             {Cmd_DesignCapacity, BAT_MAH},
                             {Cmd_DesignVoltage, BAT_MV},
                             {Cmd_SpecificationInfo, BAT_HEX},
                             {Cmd_ManufactureDate, BAT_DATE},
                             {Cmd_SerialNumber, BAT_DEC},
                             {Cmd_ManufacturerName, BAT_STRING},
                             {Cmd_DeviceName, BAT_STRING},
                             {Cmd_DeviceChemistry, BAT_STRING}};

unsigned int CMD_ESC[][2] = {{Cmd_RemainingCapacityAlarm, 4},
                             {Cmd_RemainingTimeAlarm, 3},
                             {Cmd_CycleCount, 1}};

#define CMDS_CONS 0
#define CMDS_ESCR 1
#define NINGUNO -1

#define i2cBufferLen 32
#define lcdBufferLen 17

char i2cBuffer[i2cBufferLen];
char lcdBuffer[lcdBufferLen];

char TXT_MEN_NV1[][17] = {"COMANDO CONSULTA", "COMANDO ESCRITU."};

char TXT_CMD_CON[][17] = {
    "ALARMA CAP. RES.", "ALARMA TIE. RES.", "TEMPERAT. ACTUAL",
    "VOLTAJE ACTUAL  ", "CORRIENTE ACTUAL", "CORRIE. PROMEDIO",
    "ERROR MAXIMO    ", "CARGA RELATIVA  ", "CARGA ABSOLUTA  ",
    "CAPACI. RESTANTE", "CAPACIDAD TOTAL ", "TIEMPO RESTANTE ",
    "TIE. PROM. VACIA", "TIE. PROM. LLENA", "CORRIENTE CARGA ",
    "VOLTAJE CARGA   ", "CICLOS DE CARGA ", "CAPACIDAD DISENO",
    "VOLTAJE DISENO  ", "INFO ESPECIFICA.", "FECHA FABRICACI.",
    "NUMERO DE SERIE ", "NOMB. FABRICANTE", "NOM. DISPOSITIVO",
    "QUIMICA         "};

char TXT_CMD_ESC[][17] = {"ALARMA CAP. RES.", "ALARMA TIE. RES.",
                          "CICLOS DE CARGA "};

int men_nv1_act = 0;
int men_nv2_act = NINGUNO;

#define TAM_NV1 (sizeof(TXT_MEN_NV1) / sizeof(TXT_MEN_NV1[0]))
#define TAM_CON (sizeof(TXT_CMD_CON) / sizeof(TXT_CMD_CON[0]))
#define TAM_ESC (sizeof(TXT_CMD_ESC) / sizeof(TXT_CMD_ESC[0]))

unsigned int pos_cursor = 0;

/**
 * Procedimiento principal que dibuja la IU,
 * muestra los menús, espera pulsación de cualquier botón
 * y realiza la acción asociada al menú
 */
void dibujar_menu() {
    if (men_nv2_act == NINGUNO) {
        liquidCrystal_setCursor(0, 0);
        liquidCrystal_print(TXT_MEN_NV1[men_nv1_act]);
        liquidCrystal_setCursor(11, 1);
        imprimir_numero(men_nv1_act + 1, 2);
        liquidCrystal_setCursor(13, 1);
        liquidCrystal_print("/");
        imprimir_numero(TAM_NV1, 2);
    } else {
        if (men_nv1_act == CMDS_CONS) {
            liquidCrystal_setCursor(0, 0);
            liquidCrystal_print(TXT_CMD_CON[men_nv2_act]);
            liquidCrystal_setCursor(11, 1);
            imprimir_numero(men_nv2_act + 1, 2);
            liquidCrystal_setCursor(13, 1);
            liquidCrystal_print("/");
            imprimir_numero(TAM_CON, 2);
        } else {
            liquidCrystal_setCursor(0, 0);
            liquidCrystal_print(TXT_CMD_ESC[men_nv2_act]);
            liquidCrystal_setCursor(11, 1);
            imprimir_numero(men_nv2_act + 1, 2);
            liquidCrystal_setCursor(13, 1);
            liquidCrystal_print("/");
            imprimir_numero(TAM_ESC, 2);
        }
    }
    unsigned int accion = leer_boton();
    switch (accion) {
        case BTN_ABA:  // cada menú tiene una acción o nivel inferior
            if (men_nv2_act == NINGUNO) {
                if (men_nv1_act == CMDS_CONS || men_nv1_act == CMDS_ESCR)
                    men_nv2_act = 0;
            } else {
                if (men_nv1_act == CMDS_CONS) {
                    if (men_nv2_act < 22)
                        mostrar_palabra_por_lcd(CMD_CON[men_nv2_act][0],
                                                CMD_CON[men_nv2_act][1]);
                    else if (men_nv2_act < 25)
                        mostrar_bloque_por_lcd(CMD_CON[men_nv2_act][0],
                                               CMD_CON[men_nv2_act][1]);
                } else {
                    ingresar_digitos_por_lcd(CMD_ESC[men_nv2_act][0],
                                             CMD_ESC[men_nv2_act][1]);
                }
            }
            break;
        case BTN_IZQ:
            if (men_nv2_act == NINGUNO) {
                if (men_nv1_act > 0) {
                    men_nv1_act = men_nv1_act - 1;
                }
            } else {
                if (men_nv2_act > 0) {
                    men_nv2_act = men_nv2_act - 1;
                }
            }
            break;
        case BTN_DER:
            if (men_nv2_act == NINGUNO) {
                if (men_nv1_act < TAM_NV1 - 1) {
                    men_nv1_act = men_nv1_act + 1;
                }
            } else {
                if (men_nv1_act == CMDS_CONS && men_nv2_act < TAM_CON - 1 ||
                    men_nv1_act == CMDS_ESCR && men_nv2_act < TAM_ESC - 1) {
                    men_nv2_act = men_nv2_act + 1;
                }
            }
            break;
        case BTN_ARR:
            if (men_nv2_act != NINGUNO) {
                men_nv2_act = NINGUNO;
            }
            break;
    }
}

/* Esperar que se pulse cualquier botón y retornar el pulsado */
unsigned int leer_boton() {
    unsigned int pulsado;
    esperar_boton();
    if (portd_bit_es_0(BTN_ABA))
        pulsado = BTN_ABA;
    else if (portd_bit_es_0(BTN_ARR))
        pulsado = BTN_ARR;
    else if (portd_bit_es_0(BTN_DER))
        pulsado = BTN_DER;
    else
        pulsado = BTN_IZQ;
    return pulsado;
}

unsigned int estado_anterior_aba = 0;
unsigned int estado_anterior_arr = 0;
unsigned int estado_anterior_der = 0;
unsigned int estado_anterior_izq = 0;
unsigned int timer_semaforo = 200;
unsigned int turno_parlante = SEM_CAPACIDAD;

/* Esperar que se pulse cualquier botón */
void esperar_boton() {
    unsigned int alguno_pulsado = 0;
    unsigned int estado_aba = 0;
    unsigned int estado_arr = 0;
    unsigned int estado_der = 0;
    unsigned int estado_izq = 0;

    while (!alguno_pulsado) {
        estado_aba = portd_boton_pulsado(BTN_ABA);
        estado_arr = portd_boton_pulsado(BTN_ARR);
        estado_der = portd_boton_pulsado(BTN_DER);
        estado_izq = portd_boton_pulsado(BTN_IZQ);
        if (estado_aba != estado_anterior_aba) {
            if (estado_aba == 1)
                alguno_pulsado = 1;
            else
                alguno_pulsado = 0;
            estado_anterior_aba = estado_aba;
        }
        if (estado_arr != estado_anterior_arr) {
            if (estado_arr == 1)
                alguno_pulsado = 1;
            else
                alguno_pulsado = 0;
            estado_anterior_arr = estado_arr;
        }
        if (estado_der != estado_anterior_der) {
            if (estado_der == 1)
                alguno_pulsado = 1;
            else
                alguno_pulsado = 0;
            estado_anterior_der = estado_der;
        }
        if (estado_izq != estado_anterior_izq) {
            if (estado_izq == 1)
                alguno_pulsado = 1;
            else
                alguno_pulsado = 0;
            estado_anterior_izq = estado_izq;
        }
        timer_semaforo = timer_semaforo - 1;
        if (timer_semaforo == 0) {
            timer_semaforo = 500;
            if (turno_parlante == SEM_CAPACIDAD) {
                turno_parlante = SEM_TEMPERATURA;
                sync_signal(SEM_CAPACIDAD);
            } else {
                turno_parlante = SEM_CAPACIDAD;
                sync_signal(SEM_TEMPERATURA);
            }
        }
    }
}

/**
 * imprimir número en la posición actual del LCD
 * @param numero: número a imprimir
 * @param pad: espacio reservado para el número
 */
void imprimir_numero(unsigned int numero, unsigned int pad) {
    char lcdchar[4];
    sprintf(lcdchar, "%d", numero);
    unsigned int lon = strlen(lcdchar);
    for (unsigned int i = 0; i < pad - lon; i++)
        liquidCrystal_print(" ");
    liquidCrystal_print(lcdchar);
}

/**
 * mostrar por LCD la respuesta al comando SMBus de lectura por palabra
 * @param comando: comando SMBus a enviar
 * @param unidad: unidad de la respuesta
 */
void mostrar_palabra_por_lcd(unsigned int comando, unsigned int unidad) {
    liquidCrystal_clear();
    liquidCrystal_setCursor(0, 0);
    unsigned long respuesta = smbus_leer_palabra(BATERIA, comando);
    char salida[17] = {0};
    switch (unidad) {
        case BAT_PERCENT:
            sprintf(salida, "%d \xFD", respuesta);
            break;
        case BAT_MV:
            sprintf(salida, "%d mV", respuesta);
            break;
        case BAT_MAH:
            sprintf(salida, "%d mAH", respuesta);
            break;
        case BAT_MA:
            sprintf(salida, "%d mA", respuesta);
            break;
        case BAT_MINUTES:
            sprintf(salida, "%d minutos", respuesta);
            break;
        case BAT_DATE:
            sprintf(salida, "%02d/%02d/%04d",
                    (unsigned int)(respuesta) & 0b00001111,
                    (unsigned int)(respuesta >> 5) & 0b00001111,
                    (respuesta >> 9) + 1980);
            break;
        case BAT_TENTH_K:
            sprintf(salida,
                    "%d \xDF"
                    "C",
                    ((respuesta / 10) - 273));
            break;
        default:
            sprintf(salida, "%d", respuesta);
            break;
    }
    liquidCrystal_print(salida);
    leer_boton();
}

/**
 * mostrar por LCD la respuesta al comando SMBus de lectura por bloque
 * @param comando: comando SMBus a enviar
 * @param unidad: unidad de la respuesta
 */
void mostrar_bloque_por_lcd(unsigned int comando, unsigned int unidad) {
    liquidCrystal_clear();
    liquidCrystal_setCursor(0, 0);
    smbus_leer_bloque(BATERIA, comando, i2cBuffer, i2cBufferLen);
    char salida[17] = {0};
    sprintf(salida, "%s", i2cBuffer);
    liquidCrystal_print(salida);
    leer_boton();
}

char argumento[8];

/**
 * ingresar por LCD el argumento del comando SMBus de escritura por palabra
 * @param comando: comando SMBus a enviar
 * @param num_digitos: número de digitos a ingresar
 */
void ingresar_digitos_por_lcd(unsigned int comando, unsigned int num_digitos) {
    unsigned int dig_act;
    for (dig_act = 0; dig_act < num_digitos; dig_act++)
        argumento[dig_act] = '0';
    argumento[dig_act] = '\0';
    dig_act = 0;
    int accion = -1;
    liquidCrystal_clear();
    while (accion != BTN_ABA) {
        switch (accion) {
            case BTN_IZQ:  // ir a digito de la izquierda
                if (dig_act > 0)
                    dig_act = dig_act - 1;
                break;
            case BTN_DER:  // ir a digito de la derecha
                if (dig_act < num_digitos - 1)
                    dig_act = dig_act + 1;
                break;
            case BTN_ARR:  // cambiar digito
                if (argumento[dig_act] == '9')
                    argumento[dig_act] = '0';
                else
                    argumento[dig_act] = argumento[dig_act] + 1;
                break;
        }
        liquidCrystal_setCursor(0, 0);
        liquidCrystal_print(argumento);
        liquidCrystal_setCursor(dig_act, 0);
        pos_cursor = dig_act;
        accion = leer_boton();
    }
    unsigned long entrada = myAtoi(argumento);
    smbus_escribir_palabra(BATERIA, comando, entrada);
    sprintf(argumento, "%d", entrada);
    liquidCrystal_clear();
    liquidCrystal_setCursor(0, 0);
    liquidCrystal_print(argumento);
    leer_boton();
}

/* main es una tarea independiente y se la utiliza como tal */
int main(void) {
    /* inicializar semáforos */
    sync_set(SEM_CAPACIDAD, 0);
    sync_set(SEM_TEMPERATURA, 0);

    /* inicializar GPIO */
    inicializar_portc();
    inicializar_portd();

    /* inicializar I2C */
    levantar_pin_portc(PIN_SDA);
    levantar_pin_portc(PIN_SCL);
    i2c_init();

    /* inicializar LCD */
    liquidCrystal_init();

    /* inicializar parlante */
    salida_pin_portd(PIN_PARLANTE);

    /* inicializar botones */
    levantar_pin_portd(BTN_ARR);
    levantar_pin_portd(BTN_ABA);
    levantar_pin_portd(BTN_DER);
    levantar_pin_portd(BTN_IZQ);

    /* creamos y ponemos a ejecutar las tareas */
    resume(create(tarea_capacidad, 64, 30, "capa", 0));
    resume(create(tarea_temperatura, 64, 30, "temp", 0));

    while (1)
        dibujar_menu();

    return 0;
}
