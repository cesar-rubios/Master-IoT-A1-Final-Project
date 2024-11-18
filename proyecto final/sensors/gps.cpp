#include "gps.h"


GPS::GPS(PinName tx, PinName rx, PinName enablePin)

    : gpsSerial(tx, rx, 9600), pc_serial(USBTX, USBRX, 115200), gpsEnable(enablePin) {
    num_satellites = 0;
    latitude = 0.0;
    longitude = 0.0;
    altitude = 0.0;
    meridian = ' ';
    parallel = ' ';
    measurement = ' ';
    memset(gps_time, 0, sizeof(gps_time));
    memset(gps_data, 0, sizeof(gps_data));
}


void GPS::initialize() {
    //printf("Starting GPS\n");
    gpsEnable = 1; // GPS-Module active
}


void GPS::parseData(char* nmea_sentence) {
    if (strstr(nmea_sentence, "$GPGGA")) { // Búsquedas del conjunto de datos GPGGA
        char* token = strtok(nmea_sentence, ","); // Descompone el string
        int fieldIndex = 0;
        while (token != NULL) {
            switch (fieldIndex) {
                case 1:
                    snprintf(gps_time, sizeof(gps_time), "%.2s:%.2s:%.2s", token, token + 2, token + 4); // timestamp
                    break;
                case 2:
                    latitude = atof(token) / 100;
                    break;
                case 3:
                    parallel = token[0];
                    break;
                case 4:
                    longitude = atof(token) / 100;
                    break;
                case 5:
                    meridian = token[0];
                    break;
                case 7:
                    num_satellites = atoi(token);
                    break;
                case 9:
                    altitude = atof(token);
                    break;
                case 10:
                    measurement = token[0] + 32;
                    break;
            }
            token = strtok(NULL, ",");
            fieldIndex++;
        }
        //sprintf(gps_data, "GPS: #Sats: %d Lat(UTC): %.6f %c Long(UTC): %.6f %c Altitude: %.1f %c GPT time: %s",
        //        num_satellites, latitude, parallel, longitude, meridian, altitude, measurement, gps_time);
        //printf("%s\n\n", gps_data);
        
        //printf("GPS: #Sats: %d Lat(UTC): %.6f %c Long(UTC): %.6f %c Altitude: %.1f %c GPT time: %s\n\n",
       //num_satellites, latitude, parallel, longitude, meridian, altitude, measurement, gps_time);

    }
}

// Método para leer y procesar datos GPS
void GPS::readAndProcessGPSData() {
    char buffer[256];
    //while (true) {
        if (gpsSerial.readable()) {
            int bytesRead = gpsSerial.read(buffer, sizeof(buffer));
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                parseData(buffer); // Procesa el buffer de datos GPS recibido
            }
        //}
        //ThisThread::sleep_for(2000ms); //Pequeña suspensión para reducir la carga de la CPU
    }
}

// Métodos get para devolver los datos del GPS
int GPS::getNumSatellites()  { return num_satellites; }
float GPS::getLatitude()  { return latitude; }
char GPS::getParallel()  { return parallel; }
float GPS::getLongitude()  { return longitude; }
char GPS::getMeridian()  { return meridian; }
float GPS::getAltitude()  { return altitude; }
char* GPS::getGPSTime()  { return gps_time; }
char GPS::getMeasurement() {return measurement;}


#include <ctime>

//Función para determinar el desfase horario en función del horario de verano/invierno en España
int GPS::calculateSpainOffset() {
    // Obtener fecha y mes actuales
    time_t now = time(NULL);
    struct tm *currentTime = gmtime(&now);
    int month = currentTime->tm_mon + 1; // Meses del 0 al 11, por lo tanto +1
    int day = currentTime->tm_mday;

    // Horario de verano: desde el último domingo de marzo hasta el último domingo de octubre
    if ((month > 3 && month < 10) ||  // Entre abril y septiembre
        (month == 3 && day >= 25) ||  // Última semana de marzo (alrededor del 25)
        (month == 10 && day < 25)) {  // Penúltima semana de octubre (hasta el 24 aprox.)
        return 2; // Horario de verano (UTC+2)
    } else {
        return 1; // Horario de invierno (UTC+1)
    }
}

// Función para convertir la hora GPS (UTC) a hora local de España
void GPS::convertToLocalTimeSpain() {
    int offset_hours = calculateSpainOffset();

    // Dividir el tiempo del GPS en horas, minutos y segundos
    int hours, minutes, seconds;
    sscanf(gps_time, "%2d:%2d:%2d", &hours, &minutes, &seconds);

    // aplicar offset
    hours += offset_hours;

    // Ajustar horas al formato de 24 horas
    if (hours >= 24) {
        hours -= 24; // dia siguiente
    } else if (hours < 0) {
        hours += 24; // dia anterior
    }

    // Guarda la hora local en gps_time
    snprintf(gps_time, sizeof(gps_time), "%02d:%02d:%02d", hours, minutes, seconds);
}