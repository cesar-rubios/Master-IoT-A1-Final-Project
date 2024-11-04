#include "mbed.h"
#include "analog.h"

// Definición de los modos de operación
enum Modo { NORMAL, ADVANCED, TEST };
Modo modo_actual = NORMAL;

volatile int SLEEP_TIME;
Ticker print_ticker;

// Configuración de los pines 
InterruptIn boton_usuario(PB_2);
DigitalOut led1(PA_5);  // LED1 de la placa  
DigitalOut led2(PB_6);  // LED2 de la placa
DigitalOut led3(PB_7);  // LED3 de la placa

// Definimos un objeto debounce para eliminar rebotes sin usar threads
Ticker debounce_ticker;
volatile bool debounce_flag = false;  // flag para confirmar pulsación del botón
volatile bool modo_cambiado = false;  // flag para indicar que se debe mostrar el modo

//definición de threads
Thread analog;

//función que representa los valores en la pantalla
void info_pantalla(){
    //mutex_datos_hilo.lock();
    printf("Valor leido de luz: %.2f\n", datos_hilo_analogico.brillo);
    //mutex_datos_hilo.unlock();
}

// Función que alterna el modo de operación
void cambiarModo() {
    if (!debounce_flag) {  // Comprobación anti-rebote
        debounce_flag = true;

        // Cambiar al siguiente modo de operación
        switch (modo_actual) {
            case NORMAL:
                modo_actual = ADVANCED;
                led1 = 0; led2 = 0; led3 = 1;   //leds indicativos en la placa
                printf("Modo actual: ADVANCED\n");
                SLEEP_TIME = 100; //este tiempo no hace falta
                print_ticker.detach();
                print_ticker.attach(&info_pantalla, 0.1);
                break;
            case ADVANCED:
                modo_actual = TEST;
                led1 = 1; led2 = 0; led3 = 0;   //leds indicativos en la placa
                printf("Modo actual: TEST\n");
                SLEEP_TIME = 2000;
                print_ticker.detach();
                print_ticker.attach(&info_pantalla, 2.0);
                break;
            case TEST:
                modo_actual = NORMAL;
                led1 = 0; led2 = 1; led3 = 0;   //leds indicativos en la placa
                printf("Modo actual: NORMAL\n");
                SLEEP_TIME = 30000;
                print_ticker.detach();
                print_ticker.attach(&info_pantalla, 30);
                break;
        }

        // Marcar que el modo ha cambiado para mostrarlo en el main loop
        modo_cambiado = true;

        // Activar ticker para restablecer debounce_flag después de un breve periodo
        debounce_ticker.attach(callback([] { debounce_flag = false; }), 0.3);  // Tiempo de debounce de 300 ms
    }
}

int main() {
    // Modo inicial
    led1 = 0; led2 = 1; led3 = 0;   //leds indicativos en la placa
    printf("Modo inicial: NORMAL\n");

    //hilos
    analog.start(medir_brillo);
    SLEEP_TIME = 30000;
    print_ticker.attach(&info_pantalla, 30);

    // Configuración inicial del botón para detectar pulsación
    boton_usuario.fall(&cambiarModo);  // Detecta flanco descendente (pulsación)
        
    while (true) {
        
        // Si el modo ha cambiado, se muestra el nuevo modo
        if (modo_cambiado) {
            modo_cambiado = false;  // Restablece la bandera

            // Mostrar el modo actual en consola
            switch (modo_actual) {
                case NORMAL:
                    //modo normal
                    break;
                case ADVANCED:
                    //modo advanced
                    break;
                case TEST:
                    //modo test
                    break;
            }
        }
        
        // Permitir que el microcontrolador entre en bajo consumo hasta la próxima interrupción
        //ThisThread::sleep_for(SLEEP_TIME);
    }
}
