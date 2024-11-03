#include "mbed.h"
#include "soil.h"

AnalogIn sensor(PA_4);

// Definición de la variable global para almacenar los datos
volatile int soilData;
Mutex soilMutex; // Definido aquí

void soil_measurement() {

    printf("soil iniciado\n");

    while(true){

        soilMutex.lock();
        float valor = sensor.read();
        int soilData = valor * 100;  // Escala para ver el valor como un entero
        printf("Soil: %d\n", soilData);
        soilMutex.unlock();

        ThisThread::sleep_for(5000ms);

    }

}