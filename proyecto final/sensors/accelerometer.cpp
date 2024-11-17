#include "accelerometer.h"

// Constructor initializes I2C and sensor address
MMA8451Q::MMA8451Q(I2C &i2c_ref, uint8_t address) : i2c(i2c_ref), device_address(address) {
    init();
}

// Initialize sensor by activating it
void MMA8451Q::init() {
    // uint8_t standby_data[2] = {REG_CTRL_REG_1, 0x00};
    // writeRegs(standby_data, 2);

    // // Configurar el rango a ±8g
    // uint8_t range_data[2] = {REG_XYZ_DATA_CFG, 0x02};
    // writeRegs(range_data, 2);

    //uint8_t active_data[2] = {REG_CTRL_REG_1, 0x03}; // Habilitar el sensor para que esté en modo activo
    //writeRegs(active_data, 2);
    
    //CONFIGURACIÓN PARA ADVANCED MOOD CON DETECCIÓN DE CAIDA

    // Poner el dispositivo en modo de espera (esto se hace para poder configurar correctamente el sensor)
    uint8_t standby_data[2] = {REG_CTRL_REG_1, 0x20};
    writeRegs(standby_data, 2);

    // Configurar detección de caída libre en todos los ejes
    uint8_t freefall_cfg[2] = {0x15, 0xB8}; // 0xB8 es el valor para habilitar detección en todos los ejes
    writeRegs(freefall_cfg, 2);

    // Umbral de caída 
    uint8_t threshold_data[2] = {0x17, 0x03};
    writeRegs(threshold_data, 2);

    // Configuración del conteo de duración (más duración = menos sensibilidad)
    uint8_t count_data[2] = {0x18, 0x18}; // Duración mínima para que se considere caída (en número de muestras)
    writeRegs(count_data, 2);

    // Habilitar interrupciones por caída
    uint8_t interrupt_data[2] = {0x2D, 0x20}; // Habilitar FF_MT (Free Fall Motion Trigger)
    writeRegs(interrupt_data, 2);

    // Enviar la interrupción al pin INT1
    uint8_t route_int[2] = {0x2E, 0x00}; // Enviar interrupción al pin INT1
    writeRegs(route_int, 2);

    // Activar el sensor en modo activo
    uint8_t active_data[2] = {REG_CTRL_REG_1, 0x03}; // Habilitar el sensor para que esté en modo activo
    writeRegs(active_data, 2);
    // Poner el sensor en modo activo con el bit F_READ si deseas modo de lectura rápida
    

    
}

// Get 'Who Am I' ID
uint8_t MMA8451Q::getWhoAmI() {
    uint8_t who_am_i = 0;
    readRegs(REG_WHO_AM_I, &who_am_i, 1);
    return who_am_i;
}

// Read 8 MSB bits and convert to m/s²
float MMA8451Q::read8BitAxis(uint8_t addr) {
    uint8_t msb;
    readRegs(addr, &msb, 1);
    int8_t acc = (int8_t)msb;  // Use signed 8-bit for handling negative values
    float g_value = (float(acc) / 64.0) * 9.81;  // Convert to m/s² (2g mode, 64 counts per g)
    return g_value;
}

// Read 14-bit axis data, apply 2's complement, and convert to m/s²
int16_t MMA8451Q::read16BitAxis(uint8_t addr) {
    int16_t acc;
    uint8_t res[2];
    readRegs(addr, res, 2);

    acc = (res[0] << 6) | (res[1] >> 2);
    if (acc > UINT14_MAX / 2) {
        acc -= UINT14_MAX;
    }
    return acc;
}

// Retrieve 8-bit or 16-bit X axis acceleration in m/s²
float MMA8451Q::getAccX(bool msbOnly) {
    return msbOnly ? read8BitAxis(REG_OUT_X_MSB) : (float(read16BitAxis(REG_OUT_X_MSB)) / 4096.0 * 9.81);
}

// Retrieve 8-bit or 16-bit Y axis acceleration in m/s²
float MMA8451Q::getAccY(bool msbOnly) {
    return msbOnly ? read8BitAxis(REG_OUT_Y_MSB) : (float(read16BitAxis(REG_OUT_Y_MSB)) / 4096.0 * 9.81);
}

// Retrieve 8-bit or 16-bit Z axis acceleration in m/s²
float MMA8451Q::getAccZ(bool msbOnly) {
    return msbOnly ? read8BitAxis(REG_OUT_Z_MSB) : (float(read16BitAxis(REG_OUT_Z_MSB)) / 4096.0 * 9.81);
}

// Read multiple registers from the sensor
void MMA8451Q::readRegs(int addr, uint8_t *data, int len) {
    char t[1] = {(char)addr};
    i2c.write(device_address, t, 1, true);
    i2c.read(device_address, (char *)data, len);
}

// Write to sensor registers
void MMA8451Q::writeRegs(uint8_t *data, int len) {
    i2c.write(device_address, (char *)data, len);
}


void MMA8451Q::enableFallDetection(){
    
}