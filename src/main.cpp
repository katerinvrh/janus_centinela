#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Enumeración de estados de la FSM
enum EstadoSistema {
    ESTADO_REPOSO,
    ESTADO_MUESTREO_CORRIENTE,
    ESTADO_CALCULO_FFT,
    ESTADO_ERROR
};

// Variables de estado del sistema para seguimiento de transiciones
static EstadoSistema estadoActual = ESTADO_REPOSO;

// Función auxiliar para obtener el nombre textual del estado
const char* estadoAString(EstadoSistema estado) {
    switch (estado) {
        case ESTADO_REPOSO:             return "ESTADO_REPOSO";
        case ESTADO_MUESTREO_CORRIENTE: return "ESTADO_MUESTREO_CORRIENTE";
        case ESTADO_CALCULO_FFT:        return "ESTADO_CALCULO_FFT";
        case ESTADO_ERROR:              return "ESTADO_ERROR";
        default:                        return "ESTADO_DESCONOCIDO";
    }
}

// Función para cambiar de estado e imprimir la transición en consola
void cambiarEstado(EstadoSistema nuevoEstado) {
    if (estadoActual != nuevoEstado) {
        Serial.printf("[FSM] Transición de estado: %s -> %s\n", 
                      estadoAString(estadoActual), 
                      estadoAString(nuevoEstado));
        estadoActual = nuevoEstado;
    }
}

void setup() {
    // Configuración del puerto serie a 115200 baudios
    Serial.begin(115200);

    // Espera no bloqueante para estabilizar la conexión serie
    vTaskDelay(pdMS_TO_TICKS(1000));

    Serial.println("==================================================");
    Serial.println("   Proyecto Janus - Centinela ESP32 (FreeRTOS)    ");
    Serial.println("==================================================");
    Serial.printf("[FSM] Estado inicial: %s\n", estadoAString(estadoActual));
}

void loop() {
    // Máquina de Estados Finitos (FSM) no bloqueante
    switch (estadoActual) {
        case ESTADO_REPOSO:
            // Operaciones en reposo o espera de intervalo
            vTaskDelay(pdMS_TO_TICKS(1000));
            cambiarEstado(ESTADO_MUESTREO_CORRIENTE);
            break;

        case ESTADO_MUESTREO_CORRIENTE:
            // Proceso de muestreo de señal del sensor de corriente
            vTaskDelay(pdMS_TO_TICKS(500));
            cambiarEstado(ESTADO_CALCULO_FFT);
            break;

        case ESTADO_CALCULO_FFT:
            // Ejecución de procesamiento FFT
            vTaskDelay(pdMS_TO_TICKS(500));
            cambiarEstado(ESTADO_REPOSO);
            break;

        case ESTADO_ERROR:
            // Estado de manejo y recuperación ante fallos
            Serial.println("[FSM] Estado de error alcanzado. Reiniciando ciclo...");
            vTaskDelay(pdMS_TO_TICKS(2000));
            cambiarEstado(ESTADO_REPOSO);
            break;

        default:
            cambiarEstado(ESTADO_ERROR);
            break;
    }

    // Ceder tiempo al planificador de tareas de FreeRTOS
    vTaskDelay(pdMS_TO_TICKS(10));
}