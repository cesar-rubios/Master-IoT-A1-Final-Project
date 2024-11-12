#include "temp_humid.h"
#include "mbed.h"

SI7021::SI7021(I2C &i2c) : i2c(i2c) {}

// Función para medir la temperatura y la humedad
void SI7021::measure() {
    char temp_cmd[1] = {TEMP_CMD};
    char humidity_cmd[1] = {HUMIDITY_CMD};

    // Leer la temperatura
    i2c.write(si7021_addr, temp_cmd, 1); // Enviar el comando de temperatura
    ThisThread::sleep_for(100ms);        // Esperar tiempo de conversión
    char temp_data[2];
    i2c.read(si7021_addr, temp_data, 2); // Leer 2 bytes de temperatura

    // Convertir a temperatura en Celsius
    uint16_t temp_raw = (temp_data[0] << 8) | temp_data[1];
    float temperature = (temp_raw * 175.72 / 65536) - 46.85;

    // Leer la humedad
    i2c.write(si7021_addr, humidity_cmd, 1); // Enviar el comando de humedad
    ThisThread::sleep_for(50ms);             // Esperar tiempo de conversión
    char humidity_data[2];
    i2c.read(si7021_addr, humidity_data, 2); // Leer 2 bytes de humedad

    // Convertir a humedad relativa
    uint16_t humidity_raw = (humidity_data[0] << 8) | humidity_data[1];
    float humidity = (humidity_raw * 125 / 65536) - 6;

    // Actualizar los datos
    tempHumidData.temperature = temperature;
    tempHumidData.humidity = humidity;
}

// Función para obtener los datos de temperatura y humedad
TempHumidData SI7021::get_data() {
    return tempHumidData;
}