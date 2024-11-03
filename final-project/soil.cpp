#include "mbed.h"
#include "soil.h"

AnalogIn sensor(PA_4);

// Definición de la variable global para almacenar los datos
volatile float soilData; // Definida aquí
Mutex soilMutex; // Definido aquí


void soil_measurement() {

    float value = sensor.read();
    float voltage = value * 3.3;  // Cambia esto si es necesario para tu sensor
    printf("Valor leído: %.2f V\n", voltage);

    printf("hola soy el soil sensor \n");

    while (true) { // Agregar bucle infinito
        // Bloquear el mutex antes de actualizar los datos
        soilMutex.lock();
        soilData = sensor.read();
        printf("Raw soil sensor value: %.2f\n", soilData);
        soilMutex.unlock(); // Desbloquear el mutex

        ThisThread::sleep_for(1000ms); // Esperar un poco antes de la siguiente lectura
    }
}
