#include "mbed.h"
#include "MMA8451Q.h"
#include "TCS.h"
#include "analog.h"
#include "temp_humid.h"

I2C i2c(PB_9, PB_8);  // Shared I2C object
Thread analog;
Thread temp_humid_thread;

int main() {
    
    MMA8451Q sensor(i2c, 0x1D << 1);  // Sensor with specific address
    TCS34725 rgb(i2c);
    rgb.init();
    
    analog.start(medir);
    temp_humid_thread.start(temp_humid_measurement);

    while (true) {
        ThisThread::sleep_for(3000ms);

        // Set integration time and gain
        rgb.setIntegrationTime(0xD5);  // Example integration time
        rgb.setGain(0x01);             // Example gain


            if (rgb.dataReady()) {
                uint16_t r, g, b, c;
                rgb.getRawData(r, g, b, c);

                printf("R: %d, G: %d, B: %d, C: %d\n", r, g, b, c);
            }
            ThisThread::sleep_for(500ms);

        // Read 8-bit values
        printf("8-bit X: %.2f Y: %.2f Z: %.2f\n", sensor.getAccX(true), sensor.getAccY(true), sensor.getAccZ(true));
        
        // Read 16-bit values
        printf("16-bit X: %.2f Y: %.2f Z: %.2f\n", sensor.getAccX(false), sensor.getAccY(false), sensor.getAccZ(false));

        // información sensores analógicos
        printf("Valor leido de luz: %.2f\n", datos_hilo_analogico.brillo);
        printf("Soil humidity : %.2f %\n", datos_hilo_analogico.soil);  

        //temp_humid
        printf(         
            "  Temperatura: %.2f ºC\n"
            "  Humedad: %.2f % \n\n",
           tempHumidData.temperature, tempHumidData.humidity);


    }
}
