#ifndef TEMP_HUMID_H
#define TEMP_HUMID_H

#include "mbed.h"

// Estructura para almacenar los datos de temperatura y humedad
struct TempHumidData {
    float temperature; // Temperatura en grados Celsius
    float humidity;    // Humedad en porcentaje
};

// Declaración de las variables globales
extern volatile TempHumidData tempHumidData; // Variable externa para almacenar los datos
extern Mutex tempHumidMutex; // Mutex externo para proteger el acceso a tempHumidData
extern I2C i2c; // Declarar el objeto I2C como extern

// Declaración de la función de medición
void temp_humid_measurement();

#endif // TEMP_HUMID_H