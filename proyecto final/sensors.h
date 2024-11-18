#ifndef SENSORS_H
#define SENSORS_H

#include "mbed.h"
#include <cstdint>

// Estructura para almacenar los datos de los sensores
struct DataSensors {

    //datos del sensor acelerómetro
    float x; // aceleración en el eje x
    float y; // aceleración en el eje y
    float z; // aceleración en el eje z

    //datos del sensor RGB
    uint16_t c; // luminosidad o claridad
    uint16_t r; // rojo
    uint16_t g; // verde
    uint16_t b; // azul
    const char* dominant; // color dominante
    
    //datos del sensor temperatura y humedad
    float temperature; // Temperatura en grados Celsius
    float humidity;    // Humedad en porcentaje

    //datos de los sensores analógicos
    float soil;         // Humedad en % de la tierra
    float brightness;   // Luminosidad 0-100
/*
    //datos GPS
    int sats;           // número de satélites
    float lat;          // latitud
    float longi;        // longitud
    float alt;          // altura
    char* time;         // hora
*/
};

//definición de los modos de operación
enum Modo { NORMAL, ADVANCED, TEST };
extern Modo modo_actual;

// Declaración de las variables globales
extern DataSensors sensor_data; // Variable externa para almacenar los datos
extern Mutex sensorDataMutex; // Mutex externo para proteger el acceso a los datos de los sensores
extern I2C i2c; // Declarar el objeto I2C como extern
extern int SLEEP_TIME; //tiempo de espera entre lecturas de los sensores

extern volatile bool fall_detected; //para la detección de caidas del modo advanced
extern volatile bool tap_detected; //para la detección de taps del modo advanced

extern MemoryPool<DataSensors, 30> mpool;
extern Queue<DataSensors, 30> queue;

// Declaración de la función de medición
void obtener_datos_sensores();
void calculos();

#endif // SENSORS_H