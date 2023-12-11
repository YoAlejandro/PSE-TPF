#include <xinu.h>

#include "gpio.h"
#include "lcd.h"
#include "smbus.h"
#include "utils.h"

/* semaforos */
#define SEM_CAPACIDAD 0
#define SEM_TEMPERATURA 1

#define PIN_PARLANTE 0b10000000

#define PIN_SDA 0b00010000  // PC4 como SDA
#define PIN_SCL 0b00100000  // PC5 como SCL
#define BATERIA 0b0001011   // dirección dispositivo SMBus (BQ2040)

#define BTN_ARR 0b00001000  // PD3
#define BTN_ABA 0b00000100  // PD2
#define BTN_IZQ 0b00100000  // PD5
#define BTN_DER 0b00010000  // PD4

void dibujar_menu();
void imprimir_numero(unsigned int numero, unsigned int pad);
void esperar_boton();
unsigned int leer_boton();
void mostrar_palabra_por_lcd(unsigned int comando, unsigned int unidad);
void mostrar_bloque_por_lcd(unsigned int comando, unsigned int unidad);

// Tipos de datos definidos en SBS
#define BAT_BITFIELD 0
#define BAT_DEC 1
#define BAT_HEX 2
#define BAT_MAH 3
#define BAT_MA 4
#define BAT_MV 5
#define BAT_MINUTES 6
#define BAT_PERCENT 7
#define BAT_TENTH_K 8
#define BAT_DATE 9
#define BAT_STRING 16
#define BAT_RESERVED 99

// Comandos definidos en SBS
#define Cmd_ManufacturerAccess 0x00      // Personalizado por fabricante
#define Cmd_RemainingCapacityAlarm 0x01  // Alarma de baja capacidad
#define Cmd_RemainingTimeAlarm 0x02      // Alarma de pocos minutos
#define Cmd_BatteryMode 0x03             // Modo de operación
#define Cmd_AtRate 0x04                  // Tasa de consumo
#define Cmd_AtRateTimeToFull 0x05        // Minutos para carga completa
#define Cmd_AtRateTimeToEmpty 0x06       // Minutos de uso restantes
#define Cmd_AtRateOK 0x07        // Puede funcionar 10 segundos (booleano)
#define Cmd_Temperature 0x08     // Temperatura actual en ºK (1 = 0.1 ºK)
#define Cmd_Voltage 0x09         // Voltaje actual en mV
#define Cmd_Current 0x0A         // Corriente actual en mA
#define Cmd_AverageCurrent 0x0B  // Corriente promedio en mA
#define Cmd_MaxError 0x0C        // Margen de error en capacidad restante
#define Cmd_RelativeStateOfCharge 0x0D  // Capacidad restante (capacidad carga)
#define Cmd_AbsoluteStateOfCharge 0x0E  // Capacidad restante (capacidad diseño)
#define Cmd_RemainingCapacity 0x0F      // Capacidad restante en mAH
#define Cmd_FullChargeCapacity 0x10     // Capacidad de carga en mAH
#define Cmd_RunTimeToEmpty 0x11         // Minutos restantes (consumo actual)
#define Cmd_AverageTimeToEmpty 0x12     // Minutos restantes (consumo promedio)
#define Cmd_AverageTimeToFull 0x13      // Minutos carga completa
#define Cmd_ChargingCurrent 0x14        // Corriente deseada de carga
#define Cmd_ChargingVoltage 0x15        // Voltaje deseado de carga
#define Cmd_BatteryStatus 0x16          // Registro de estado de la batería
#define Cmd_CycleCount 0x17             // Ciclos de carga
#define Cmd_DesignCapacity 0x18         // Capacidad de diseño
#define Cmd_DesignVoltage 0x19          // Voltaje de diseño
#define Cmd_SpecificationInfo 0x1A      // Versión de SBS soportada
#define Cmd_ManufactureDate 0x1B        // Fecha de fabricación
#define Cmd_SerialNumber 0x1C           // Número de serie
#define Cmd_Reserved1 0x1D
#define Cmd_Reserved2 0x1E
#define Cmd_Reserved3 0x1F
#define Cmd_ManufacturerName 0x20  // Fabricante
#define Cmd_DeviceName 0x21        // Modelo
#define Cmd_DeviceChemistry 0x22   // Quimica
#define Cmd_ManufacturerData 0x23  // Datos personalizados
