#ifndef TEMP_HUMID_H
#define TEMP_HUMID_H

#include "mbed.h"

// Estructura para almacenar los datos de temperatura y humedad
struct TempHumidData {
    float temperature;
    float humidity;
};

class SI7021 {
public:
    // Constructor, recibe una instancia de I2C
    SI7021(I2C &i2c);

    // Función para medir la temperatura y la humedad
    void measure();

    // Obtener los datos de temperatura y humedad
    TempHumidData get_data();

private:
    I2C &i2c;                    // Referencia a la instancia de I2C
    const int si7021_addr = 0x40 << 1; // Dirección I2C del SI7021 (8 bits)
    TempHumidData tempHumidData; // Datos de temperatura y humedad

    // Comandos para medir temperatura y humedad
    const char TEMP_CMD = 0xE3;
    const char HUMIDITY_CMD = 0xE5;
};

#endif // TEMP_HUMID_H