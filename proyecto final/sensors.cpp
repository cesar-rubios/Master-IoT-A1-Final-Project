#include "sensors.h"
#include "mbed.h"
#include "mbed_power_mgmt.h"
#include "sensors/RGB.h"
#include "sensors/accelerometer.h"
#include "sensors/analog.h"
#include "sensors/gps.h"
#include "sensors/temp_humid.h"


DataSensors sensor_data; // Variable definidad aquí para almacenar los datos
Mutex sensorDataMutex; // mutex para evitar accesos del hilo principal antes de
                       // terminar de escribir los valores leidos

// flag para el modo avanzado y detectar la caida o el tap
volatile bool fall_detected = false;
volatile bool tap_detected = false;

// pines digitales donde están los leds
DigitalOut led_green(D11);
DigitalOut led_blue(D12);
DigitalOut led_red(D13);
// función que enciende los leds
void color_led(int _red, int _green, int _blue) {
  led_red = _red;
  led_green = _green;
  led_blue = _blue;
}

// para el GPS
#define GPS_TX_PIN PA_9
#define GPS_RX_PIN PA_10
#define GPS_ENABLE_PIN PA_12

// intercambio de calculos
MemoryPool<DataSensors, 30> mpool;
Queue<DataSensors, 30> queue;

// Función para determinar el color dominante en el sensor RGB
// también sirve en modo TEST de representar el color dominante en el LED
const char *max_color(uint16_t r, uint16_t g, uint16_t b) {
  if (r > g && r > b) {
    if (modo_actual == TEST) {
      color_led(0, 1, 1);
    } // color rojo en el led
    return "RED";
  } else if (g > b) {
    if (modo_actual == TEST) {
      color_led(1, 0, 1);
    } // color verde en el led
    return "GREEN";
  } else {
    if (modo_actual == TEST) {
      color_led(1, 1, 0);
    } // color azul en el led
    return "BLUE";
  }
}

void data_limits() {

  if (sensor_data.temperature <= 10 || sensor_data.temperature >= 30) {
    color_led(0, 0, 0);
  } // blanco
  else if (sensor_data.humidity <= 20 || sensor_data.humidity >= 80) {
    color_led(0, 0, 1);
  } // amarillo
  else if (sensor_data.brightness <= 10 || sensor_data.brightness >= 75) {
    color_led(0, 1, 0);
  } // magenta
  else if (sensor_data.soil <= 10 || sensor_data.soil >= 90) {
    color_led(1, 0, 0);
  } // cyan

  else if (sensor_data.x <= -10 || sensor_data.x <= 10 ||
           sensor_data.y <= -10 || sensor_data.y <= 10 ||
           sensor_data.y <= -10 || sensor_data.y <= 10) {
    color_led(1, 0, 1);
  } // verde
  else if (sensor_data.r <= 75 || sensor_data.r >= 400 || sensor_data.g <= 75 ||
           sensor_data.g >= 400 || sensor_data.b <= 75 ||
           sensor_data.b >= 400) {
    color_led(1, 1, 0);
  } // azul

  else {
    color_led(1, 1, 1);
  }
}

void obtener_datos_sensores() {

  led_red = 1;
  led_green = 1;
  led_blue = 1; // led apagado

  MMA8451Q accelerometer(
      i2c, 0x1D << 1); // definición de un objeto de tipo acelerómetro
  ThisThread::sleep_for(10s);
  //    fall_detected = true;
  //    accelerometer.enableFallDetection();        // Activar modo de detección
  //    de caída

  TCS34725 rgb(i2c); // definición de un objeto de tipo sensor RGB
  rgb.init();

  SI7021 temperature_humidity(
      i2c); // definición de un objeto de tipo sensor humedad y temperatura

  AnalogSensor infrared_soil(PA_0, PA_4);

//   GPS gps(GPS_TX_PIN, GPS_RX_PIN, GPS_ENABLE_PIN);
//   gps.initialize();

  while (1) {

    sensorDataMutex.lock();

    // lectura de los datos del acelerómetro
    sensor_data.x = accelerometer.getAccX(true);
    sensor_data.y = accelerometer.getAccY(true);
    sensor_data.z = accelerometer.getAccZ(true);

    // Set integration time and gain
    rgb.setIntegrationTime(0xD5); // Example integration time
    rgb.setGain(0x01);            // Example gain
    rgb.getRawData(sensor_data.r, sensor_data.g, sensor_data.b, sensor_data.c);
    sensor_data.dominant =
        max_color(sensor_data.r, sensor_data.g, sensor_data.b);

    temperature_humidity.measure(); // medimos los valores de temp y humid
    TempHumidData data_th =
        temperature_humidity
            .get_data(); // obtenemos los datos medidos por el sensor
    sensor_data.humidity = data_th.humidity;
    sensor_data.temperature = data_th.temperature;

    infrared_soil.measure();
    AnalogData data_an = infrared_soil.get_data();
    sensor_data.brightness = data_an.brightness;
    sensor_data.soil = data_an.soilMoisture;

    // gps.readAndProcessGPSData();
    // sensor_data.sats = gps.getNumSatellites();
    // sensor_data.lat = gps.getLatitude();
    // sensor_data.longi = gps.getLongitude();
    // sensor_data.alt = gps.getAltitude();
    // sensor_data.time = gps.getGPSTime();

    if (modo_actual == NORMAL) {
      data_limits();
    }

    DataSensors *mensaje = mpool.alloc();
    if (mensaje != nullptr) {
      if (queue.put(mensaje) != osOK) {
        mpool.free(mensaje); // Libera el bloque si no puedes ponerlo en la cola
        printf("Error: No se pudo poner el mensaje en la cola\n");
      } else {

        mensaje->x = sensor_data.x;
        mensaje->y = sensor_data.y;
        mensaje->z = sensor_data.z;
        mensaje->r = sensor_data.r;
        mensaje->g = sensor_data.g;
        mensaje->b = sensor_data.b;
        mensaje->c = sensor_data.c;
        mensaje->dominant = sensor_data.dominant;
        mensaje->temperature = sensor_data.temperature;
        mensaje->humidity = sensor_data.humidity;
        mensaje->brightness = sensor_data.brightness;
        mensaje->soil = sensor_data.soil;
      }

    } else {
      printf("Error: MemoryPool lleno\n");
    }

    sensorDataMutex.unlock();

    while (fall_detected) {
      led_red = 1;
      led_green = 1;
      led_blue = 1;
      sleep();
    }
    ThisThread::sleep_for(SLEEP_TIME);
  }
}