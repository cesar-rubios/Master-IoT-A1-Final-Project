#ifndef CALCULOS_H
#define CALCULOS_H

#include "mbed.h"
#include "sensors.h"

extern MemoryPool<DataSensors, 3> mpool;
extern Queue<DataSensors, 3> queue;

void calculos();

#endif // CALCULOS_H