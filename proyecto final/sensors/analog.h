#ifndef ANALOG_H
#define ANALOG_H

#include "mbed.h"

// Estructura para almacenar los datos de brillo y humedad del suelo
struct AnalogData {
    float brightness;
    float soilMoisture;
};

class AnalogSensor {
public:
    // Constructor, recibe pines para los sensores analógicos
    AnalogSensor(PinName brightnessPin, PinName soilPin);

    // Método para realizar la medición y actualizar el LED
    void measure();

    // Método para obtener los datos de brillo y humedad
    AnalogData get_data();

private:
    AnalogIn brightnessSensor;   // Sensor de brillo
    AnalogIn soilSensor;         // Sensor de humedad del suelo

    AnalogData analogData;       // Datos de brillo y humedad

};

#endif // ANALOG_H