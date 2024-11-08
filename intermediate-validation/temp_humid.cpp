#include "mbed.h"
#include "temp_humid.h"

// Definición de la variable global para almacenar los datos
volatile TempHumidData tempHumidData; // Definida aquí
Mutex tempHumidMutex; // Definido aquí

// Dirección del sensor Si7021
const int si7021_addr = 0x40 << 1; // Ajusta la dirección a 8 bits

void temp_humid_measurement() {
    // Comando para medir la temperatura (hold master)
    char temp_cmd[1] = {0xE3}; // Comando para medir temperatura
    char humidity_cmd[1] = {0xE5}; // Comando para medir humedad

    while(true){
        // Leer la temperatura
        i2c.write(si7021_addr, temp_cmd, 1); // Enviar el comando para medir temperatura
        ThisThread::sleep_for(100ms); // Esperar el tiempo de conversión
        char temp_data[2];
        i2c.read(si7021_addr, temp_data, 2); // Leer 2 bytes de datos de temperatura

        // Convertir a temperatura en Celsius
        uint16_t temp_raw = (temp_data[0] << 8) | temp_data[1];
        float temperature = (temp_raw * 175.72 / 65536) - 46.85;

        // Leer la humedad
        i2c.write(si7021_addr, humidity_cmd, 1); // Enviar el comando para medir humedad
        ThisThread::sleep_for(50ms); // Esperar el tiempo de conversión
        char humidity_data[2];
        i2c.read(si7021_addr, humidity_data, 2); // Leer 2 bytes de datos de humedad

        // Convertir a humedad relativa
        uint16_t humidity_raw = (humidity_data[0] << 8) | humidity_data[1];
        float humidity = (humidity_raw * 125 / 65536) - 6;

        // Bloquear el mutex antes de actualizar los datos
        tempHumidMutex.lock();
        tempHumidData.temperature = temperature;
        tempHumidData.humidity = humidity;
        tempHumidMutex.unlock();
    }
   
} 