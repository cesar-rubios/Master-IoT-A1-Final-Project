#ifndef ANALOG_H
#define ANALOG_H

#include "mbed.h"

extern volatile int SLEEP_TIME;

struct datos_analogicos{
    float brillo;
    float soil;
};

extern volatile datos_analogicos datos_hilo_analogico;
extern Mutex mutex_datos_hilo;

void color_led(int _red, int _green, int _blue);
void medir();

#endif // ANALOG_H