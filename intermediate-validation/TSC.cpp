// TCS34725.cpp

#include "TCS.h"

TCS34725::TCS34725(I2C &i2c_ref) : i2c(i2c_ref) {
    i2c.frequency(400000);  // Establece la velocidad del bus I2C
}

bool TCS34725::init() {
    // Activa el sensor escribiendo en el registro ENABLE
    write8(ENABLE, 0x01);  // PON: Power ON, activa el oscilador
    ThisThread::sleep_for(3ms);  // Espera 3 ms para que el sensor se encienda
    write8(ENABLE, 0x03);  // AEN: Activa el ADC
    return true;
}

void TCS34725::setIntegrationTime(uint8_t atime) {
    write8(ATIME, atime);
}

void TCS34725::setGain(uint8_t gain) {
    write8(CONTROL, gain);
}

bool TCS34725::dataReady() {
    uint8_t status = read8(STATUS);
    return (status & 0x01);  // Verifica el bit AVALID en STATUS
}

void TCS34725::getRawData(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c) {
    c = read16(CDATAL);
    r = read16(RDATAL);
    g = read16(GDATAL);
    b = read16(BDATAL);
}

void TCS34725::write8(Register reg, uint8_t value) {
    char data[2] = {(char)(0x80 | reg), (char)value};  // 0x80 para indicar escritura de comando
    i2c.write(I2C_ADDR, data, 2);
}

uint8_t TCS34725::read8(Register reg) {
    char data[1] = {(char)(0x80 | reg)};
    i2c.write(I2C_ADDR, data, 1);
    i2c.read(I2C_ADDR, data, 1);
    return data[0];
}

uint16_t TCS34725::read16(Register reg) {
    char data[2];
    char regAddr = (char)(0x80 | reg);
    i2c.write(I2C_ADDR, &regAddr, 1);
    i2c.read(I2C_ADDR, data, 2);
    return (data[1] << 8) | data[0];
}
