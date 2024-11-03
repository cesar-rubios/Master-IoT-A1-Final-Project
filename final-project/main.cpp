/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

// ZONA DE INCLUDES PARA LIBRERIAS DEL SISTEMA
#include "mbed.h"
// ZONA DE INCLUDES PARA OTROS ARCHIVOS
#include "accelerometer.h"
#include "temp_humid.h"
#include "soil.h"

// Definición de la variable I2C como global
I2C i2c(PB_9, PB_8); // Definido aquí, solo una vez

// Definición de LEDs
DigitalOut red(D10);
DigitalOut green(D11);
DigitalOut blue(D12);

// Definición de hilos
Thread accelerometer_thread;
Thread temp_humid_thread;
Thread soil_thread;

int main() {
    // Pone el LED en color rojo para asegurar inicio
    red = 0;    green = 1;    blue = 1;
    printf("INICIO DEL PROGRAMA:\n");

    // Iniciar los hilos para los sensores
    //accelerometer_thread.start(accel_measurement);
    //temp_humid_thread.start(temp_humid_measurement);
    soil_thread.start(soil_measurement);
        
    // Bucle principal que imprime los valores del acelerómetro
    while (true) {
        
        dataMutex.lock();
        int16_t x = accelData.x;
        int16_t y = accelData.y;
        int16_t z = accelData.z;
        dataMutex.unlock();

        tempHumidMutex.lock();
        int temp = tempHumidData.temperature;
        int humid = tempHumidData.humidity;
        tempHumidMutex.unlock();    

        soilMutex.lock();
        int soil = soilData; 
        printf("Soil: %d\n", soilData);  
        soilMutex.unlock();      

        printf("He leído valores:\n"
            "  Aceleración X: %d, Y: %d, Z: %d\n"
            "  Temperatura: %d ºC\n"
            "  Humedad: %d % \n"
            "  Soil Humidity : %d %\n\n",
            x, y, z, temp, humid, soil);

        ThisThread::sleep_for(1000ms); // Esperar un poco antes de la siguiente impresión
    }
}