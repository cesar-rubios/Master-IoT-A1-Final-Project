#include "mbed.h"
#include "accelerometer.h"

const int mma8451q_addr = 0x1D << 1;  // 0x1D es la dirección de 7 bits del sensor (cambio a 8 bits)

// Registros del MMA8451Q
const char REG_CTRL_REG1 = 0x2A; // Registro de control para activar el modo activo
const char REG_OUT_X_MSB = 0x01; // Registro de salida de datos para X (MSB)

// Definición de la variable global para almacenar los datos
volatile AccelerometerData accelData; // Definida aquí
Mutex dataMutex; // Definido aquí

void accel_measurement() {
    char data[6];
    char reg = REG_OUT_X_MSB;

    // Configurar el acelerómetro en modo de medida
    char config[2];
    config[0] = REG_CTRL_REG1;
    config[1] = 0x01; // Activar el sensor (establece el bit de modo activo)
    i2c.write(mma8451q_addr, config, 2);
    
    while(true){
        // Enviar el registro desde el cual queremos leer
        i2c.write(mma8451q_addr, &reg, 1, true);
        
        // Leer 6 bytes de datos desde el registro de datos
        i2c.read(mma8451q_addr, data, 6);
            
        int16_t x = ((data[0] << 8) | data[1]) >> 2;
        int16_t y = ((data[2] << 8) | data[3]) >> 2;
        int16_t z = ((data[4] << 8) | data[5]) >> 2;

        // Bloquear el mutex antes de actualizar los datos
        dataMutex.lock();
        accelData.x = x;
        accelData.y = y;
        accelData.z = z;
        dataMutex.unlock();// Desbloquear el mutex

        //printf("Aceleración THREAD X: %d, Y: %d, Z: %d\n", accelData.x, accelData.y, accelData.z);
        ThisThread::sleep_for(200ms); // Esperar un poco antes de la siguiente impresión

    }

}