#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "mbed.h"

// Estructura para almacenar los datos del acelerómetro
struct AccelerometerData {
    int16_t x;
    int16_t y;
    int16_t z;
};

// Declaración de las variables globales
extern volatile AccelerometerData accelData; // Variable externa para almacenar los datos
extern Mutex dataMutex; // Mutex externo para proteger el acceso a accelData
extern I2C i2c; // Declarar el objeto I2C como extern

// Declaración de la función de medición
void accel_measurement();

#endif // ACCELEROMETER_H
