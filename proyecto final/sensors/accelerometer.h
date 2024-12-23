#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "mbed.h"

// MMA8451Q Registers
#define REG_WHO_AM_I    0x0D
#define REG_CTRL_REG_1  0x2A
#define REG_CTRL_REG_2    0x2B
#define REG_CTRL_REG_3    0x2C
#define REG_CTRL_REG_4    0x2D
#define REG_CTRL_REG_5    0x2E
#define REG_OUT_X_MSB   0x01
#define REG_OUT_Y_MSB   0x03
#define REG_OUT_Z_MSB   0x05
#define REG_XYZ_DATA_CFG 0x0E // 8g

#define CTRL_REG1    0x2A
#define CTRL_REG2    0x2B
#define CTRL_REG3    0x2C
#define CTRL_REG4    0x2D
#define CTRL_REG5    0x2E
#define FF_MT_CFG    0x15
#define FF_MT_THS    0x17
#define FF_MT_COUNT  0x18
#define PULSE_CFG    0x21
#define PULSE_THSX   0x23
#define PULSE_THSY   0x24
#define PULSE_THSZ   0x25
#define PULSE_TMLT   0x26
#define PULSE_LTCY   0x27


#define UINT14_MAX      16383  // 14-bit unsigned max value for 2's complement conversion

class MMA8451Q {
public:
    // Constructor to accept a shared I2C object and unique sensor address
    MMA8451Q(I2C &i2c_ref, uint8_t address);
    
    // Initialize sensor by setting control register to active mode
    void init();

    // Read sensor ID
    uint8_t getWhoAmI();

    // Read acceleration values in m/s^2 for each axis (8-bit or 16-bit options)
    float getAccX(bool msbOnly = false);
    float getAccY(bool msbOnly = false);
    float getAccZ(bool msbOnly = false);

    //función para el modo avanzado y detectar caidas
    void enableFallDetection();

private:
    I2C &i2c;
    uint8_t device_address;

    // Read multiple registers
    void readRegs(int addr, uint8_t *data, int len);

    // Write to registers
    void writeRegs(uint8_t *data, int len);
    void write_register(uint8_t reg, uint8_t value);
    char read_register(uint8_t reg);

    // Read axis data in 14-bit or 8-bit resolution
    int16_t read16BitAxis(uint8_t addr);
    float read8BitAxis(uint8_t addr);
};

#endif // ACCELEROMETER_H
