#ifndef CALCULOS_H
#define CALCULOS_H

#include "mbed.h"
#include "sensors.h"

struct message_t{
    float soil;   
    float brightness;  
           
};

extern MemoryPool<DataSensors, 5> mpool;
extern Queue<DataSensors, 5> queue;

void calculos();

#endif // CALCULOS_H