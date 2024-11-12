#include "mbed.h"
#include "sensors.h"
#include "sensors/accelerometer.h"
#include "sensors/RGB.h"
#include "sensors/temp_humid.h"

DataSensors sensor_data;   // Variable definidad aquí para almacenar los datos
Mutex sensorDataMutex;              //mutex para evitar accesos del hilo principal antes de terminar de escribir los valores leidos


//pines digitales donde están los leds
DigitalOut led_green(D11);
DigitalOut led_blue(D12);
DigitalOut led_red(D13);
//función que enciende los leds
void color_led(int _red, int _green, int _blue) {
  led_red = _red;
  led_green = _green;
  led_blue = _blue;
}
// Función para determinar el color dominante en el sensor RGB
// también sirve en modo TEST de representar el color dominante en el LED
const char* max_color(uint16_t r, uint16_t g, uint16_t b) {
    if (r > g && r > b) {
        if(modo_actual==TEST) {color_led(0, 1, 1);} //color rojo en el led
        return "RED";
    } else if (g > b) {
        if(modo_actual==TEST){color_led(1, 0, 1);} //color verde en el led
        return "GREEN";
    } else {
        if(modo_actual==TEST){color_led(1, 1, 0);} //color azul en el led
        return "BLUE";
    }
}



void obtener_datos_sensores(){

    led_red = 1; led_green = 1; led_blue = 1;   // led apagado

    MMA8451Q accelerometer(i2c, 0x1D << 1);     // definición de un objeto de tipo acelerómetro

    TCS34725 rgb(i2c);                          // definición de un objeto de tipo sensor RGB
    rgb.init();

    SI7021 temperature_humidity(i2c);           // definición de un objeto de tipo sensor humedad y temperatura
    

    while (1) {
    
        sensorDataMutex.lock();

        //lectura de los datos del acelerómetro
        sensor_data.x = accelerometer.getAccX(true);
        sensor_data.y = accelerometer.getAccY(true);
        sensor_data.z = accelerometer.getAccZ(true);

        // Set integration time and gain
        rgb.setIntegrationTime(0xD5);  // Example integration time
        rgb.setGain(0x01);             // Example gain
        rgb.getRawData(sensor_data.r, sensor_data.g, sensor_data.b, sensor_data.c);
        sensor_data.dominant = max_color(sensor_data.r, sensor_data.g, sensor_data.b);

        temperature_humidity.measure();                         //medimos los valores de temp y humid
        TempHumidData data = temperature_humidity.get_data();   //obtenemos los datos medidos por el sensor
        sensor_data.humidity = data.humidity;                   
        sensor_data.temperature = data.temperature;

        sensorDataMutex.unlock();

        ThisThread::sleep_for(SLEEP_TIME);

    }

}