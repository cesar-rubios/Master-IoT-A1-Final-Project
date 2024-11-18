#ifndef CALCULOS_H
#define CALCULOS_H

#include "mbed.h"
#include "sensors.h"

extern MemoryPool<DataSensors, 30> mpool;
extern Queue<DataSensors, 30> queue;

void calculos();

#endif // CALCULOS_H