#include "analog.h"

//pin analógico para leer los valores del sensor de infrarojos
AnalogIn sensor_brillo(PA_0);
volatile datos_analogicos datos_hilo_analogico;
Mutex mutex_datos_hilo;
float brillo_local;
float soil_local;

//pines digitales donde están los leds
DigitalOut red(D10);
DigitalOut green(D11);
DigitalOut blue(D12);

//pin que lee los valores analógicos del soil sensor
AnalogIn sensor(PA_4);

// Definición de la variable global para almacenar los datos de soil
volatile int soilData;
Mutex soilMutex; // Definido aquí

//función que lee los valores del sensor infrarojos
void medir(){
    while(1){

        brillo_local = sensor_brillo.read();
        float valor = sensor.read();
        float soilData = valor * 100;  // Escala para ver el valor como un entero
        
        mutex_datos_hilo.lock();
        datos_hilo_analogico.brillo = brillo_local;
        datos_hilo_analogico.soil = soilData;
        mutex_datos_hilo.unlock();


    if (brillo_local < 0.33) {
        color_led(0, 1, 1); // Rojo 
        //printf("Low brightness: RED\n");
        } else if (brillo_local < 0.66) {
        color_led(0, 0, 1); // Amarillo 
        //printf("Medium brightness: YELLOW\n");
        } else {
        color_led(1, 0, 1); // Verde 
        //printf("High brightness: GREEN\n");
    }

    ThisThread::sleep_for(100ms);

    }
}


//función que enciende los leds
void color_led(int _red, int _green, int _blue) {
  red = _red;
  green = _green;
  blue = _blue;
}
