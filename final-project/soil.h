#ifndef SOIL_H
#define SOIL_H

#include "mbed.h"

// Declaración de las variables globales
extern volatile float soilData; // Variable externa para almacenar los datos
extern Mutex soilMutex; // Mutex externo para proteger el acceso a soilData

// Declaración de la función de medición
void soil_measurement();

#endif // SOIL_H