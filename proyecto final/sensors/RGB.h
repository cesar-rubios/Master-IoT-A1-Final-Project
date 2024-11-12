#ifndef RGB_H
#define RGB_H

#include "mbed.h"

class TCS34725 {
public:
    // Dirección I2C del sensor TCS34725
    static const int I2C_ADDR = 0x29 << 1;

    // Direcciones de registros
    enum Register {
        ENABLE = 0x00,
        ATIME = 0x01,
        WTIME = 0x03,
        CONTROL = 0x0F,
        STATUS = 0x13,
        CDATAL = 0x14,
        CDATAH = 0x15,
        RDATAL = 0x16,
        RDATAH = 0x17,
        GDATAL = 0x18,
        GDATAH = 0x19,
        BDATAL = 0x1A,
        BDATAH = 0x1B
    };

    TCS34725(I2C &i2c_ref);
    
    bool init();
    void setIntegrationTime(uint8_t atime);
    void setGain(uint8_t gain);
    bool dataReady();
    void getRawData(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c);

private:
    I2C &i2c;;

    void write8(Register reg, uint8_t value);
    uint8_t read8(Register reg);
    uint16_t read16(Register reg);
};

#endif // RGB_H
