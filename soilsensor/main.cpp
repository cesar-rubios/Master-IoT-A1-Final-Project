#include "mbed.h"

AnalogIn sensor(PA_4);

int main() {
    while (true) {
        float valor = sensor.read();
        int valor_entero = valor * 100;  // Escala para ver el valor como un entero
        printf("Valor en porcentaje aproximado: %d%%\n", valor_entero);


        // Espera 1 segundo
        ThisThread::sleep_for(1000ms);
    }
}