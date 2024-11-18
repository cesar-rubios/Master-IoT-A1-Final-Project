#include "mbed.h"
#include "sensors.h"
#include "calculos.h"

// modo inicial normal
Modo modo_actual = NORMAL;
int SLEEP_TIME = 1000;
int tap_count = 0;

// Configuración del botón en el pin PB_2
InterruptIn boton_usuario(PB_2);
// Configuración de los LEDs de la propia placa internos
DigitalOut led1(PB_5); // LED1 en el pin PB_5
DigitalOut led2(PA_5); // LED2 en el pin PA_5
DigitalOut led3(PB_6); // LED3 en el pin PB_6
I2C i2c(PB_9, PB_8);  // objeto I2C compartido

// Definimos un objeto debounce para eliminar rebotes sin usar threads
Ticker debounce_ticker;
volatile bool debounce_flag = false;  // flag para confirmar pulsación del botón
volatile bool modo_cambiado = false;  // flag para indicar que se debe mostrar el modo

// Definimos el ticker para mostrar los datos por pantalla
Ticker print_ticker;
volatile bool print_flag = false;  // flag para indicar que se debe mostrar info en pantalla
// Función que activa el flag para mostrar información en pantalla
void set_print_flag() {
    print_flag = true;
}

//definición de los hilos
Thread hilo_sensores(osPriorityNormal, 2*1024);
Thread hilo_calculos(osPriorityNormal, 4*1024);

// pin de interrupcion INT1 del acelerómetro para advanced mode
InterruptIn accel_interrupt(PA_8); 
void detect_fall() {
    fall_detected = true;
}
// pin de interrupcion INT2 del acelerómetro para advanced mode
InterruptIn accel_tap(PA_11);
void detect_tap() {
    tap_detected = true;
}

// Función que alterna el modo de operación
void cambiarModo() {
    if (!debounce_flag) {  // Comprobación anti-rebote
        debounce_flag = true;
        // Cambiar al siguiente modo de operación
        switch (modo_actual) {
            case NORMAL:
                modo_actual = TEST;
                led1=1; led2=0; led3=0;
                print_ticker.detach();
                print_ticker.attach(&set_print_flag, 2);
                SLEEP_TIME = 2000;
                break;
            case TEST:
                modo_actual = NORMAL;
                led1=0; led2=1; led3=0;
                print_ticker.detach();
                print_ticker.attach(&set_print_flag, 10);
                SLEEP_TIME = 10000;
                break;
        }

        // Marcar que el modo ha cambiado para mostrarlo en el main loop
        modo_cambiado = true;

        // Activar ticker para restablecer debounce_flag después de un breve periodo
        debounce_ticker.attach(callback([] { debounce_flag = false; }), 0.3);  // Tiempo de debounce de 300 ms
    }
}

void info_pantalla() {

    // Función para mostrar datos
    printf(
        "\nNumer of taps: %d \n"
        "SOIL MOISTURE: %.1f%% \n"
        "LIGHT: %.1f%% \n"
        "GPS: #Sats: %d Lat(UTC): %.6f N Long(UTC): %.6f W Altitude: %.0f GPS time: %s \n"
        "COLOR SENSOR: Clear: %d Red: %d Green: %d Blue: %d -- Dominant color: %s \n"
        "ACCELEROMETERS: X_axis: %.2f m/s², Y_axis: %.2f m/s², Z_axis: %.2f m/s² \n"
        "TEMP/HUM: Temperature: %.1f ºC,    Relative Humidity: %.1f%% \n"
        "--------------------------\n\n",
        tap_count,
        sensor_data.soil,
        sensor_data.brightness,
        sensor_data.sats, sensor_data.lat, sensor_data.longi, sensor_data.alt, sensor_data.time,
        sensor_data.c, sensor_data.r, sensor_data.g, sensor_data.b, sensor_data.dominant,
        sensor_data.x, sensor_data.y, sensor_data.z,
        sensor_data.temperature, sensor_data.humidity

   );
   
}

int main() {

    printf("\n\n\n\n\n\n\n");

    hilo_sensores.start(obtener_datos_sensores);
    hilo_calculos.start(calculos);
    
    // Configuración inicial del botón para detectar pulsación
    boton_usuario.fall(&cambiarModo);  // Detecta borde de caída (pulsación)
    led1=0; led2=1; led3=0; // modo inicial NORMAL
    print_ticker.attach(&set_print_flag, 10); // mostramos los datos cada 30 segundos (modo inicial: normal)

    accel_interrupt.rise(&detect_fall); //si se interrumpe desde el acelerómetro, ejecutamos la ISR detect_fall
    accel_tap.rise(&detect_tap); //si se interrumpe desde el acelerómetro, ejecutamos la ISR detect_tap


    while (true) {

        if (fall_detected) {    //si se detecta una caida 
            // modo AVANZADO
            modo_actual = ADVANCED;
            boton_usuario.disable_irq();
            led1=0; led2=0;
            led3 = !led3;
            ThisThread::sleep_for(500);
        }
        else{

            if (tap_detected){  //si se detecta un tap
                tap_count++;
            }
            
            // Si el flag print_flag está activa, mostramos los datos en pantalla
            if (print_flag) {
                print_flag = false;  // Restablece el flag
                info_pantalla();     // Llama a la función de impresión
            }

            // Si el modo ha cambiado, se muestra el nuevo modo
            if (modo_cambiado) {
                modo_cambiado = false;  // Restablece la bandera
                switch (modo_actual) {
                    case NORMAL:
                        //printf("Modo actual: NORMAL\n");
                        break;
                    case ADVANCED:
                        //printf("Modo actual: ADVANCED\n");
                        break;
                    case TEST:
                        //printf("Modo actual: TEST\n");
                        break;
                }
            }

            // Permitir que el microcontrolador entre en bajo consumo hasta la próxima interrupción
            sleep();
            }
        
    }
}
