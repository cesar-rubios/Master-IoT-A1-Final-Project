#include "analog.h"

AnalogSensor::AnalogSensor(PinName brightnessPin, PinName soilPin) : brightnessSensor(brightnessPin), soilSensor(soilPin){
}

// Método para medir el brillo y la humedad, y actualizar el color del LED
void AnalogSensor::measure() {
    // Leer y almacenar la luminosidad
    float brightness = brightnessSensor.read();
    analogData.brightness = brightness *100;

    // Leer y almacenar el valor del sensor de suelo, escalado a porcentaje
    float soilValue = soilSensor.read();
    analogData.soilMoisture = soilValue * 100;

}

// Método para obtener los datos de brillo y humedad
AnalogData AnalogSensor::get_data() {
    return analogData;
}
