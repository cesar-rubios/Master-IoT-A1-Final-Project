// gps.h
#include "mbed.h"
#include <cstring>
#include <cstdlib>

class GPS {

    private:
    // componentes GPS
    BufferedSerial gpsSerial;
    BufferedSerial pc_serial;
    DigitalOut gpsEnable;

    // datos GPS
    int num_satellites;
    float latitude;
    float longitude;
    char meridian;
    char parallel;
    float altitude;
    char measurement;
    char gps_time[10];

    // Búfer para datos GPS
    char gps_data[256];

public:
    // constructor
    GPS(PinName tx, PinName rx, PinName enablePin);

    // inicializar módulo GPS
    void initialize();

    void parseData(char* nmea_sentence);

    int getNumSatellites();
    float getLatitude();
    char getParallel();
    float getLongitude();
    char getMeridian();
    float getAltitude();
    char* getGPSTime();
    char getMeasurement();


    void readAndProcessGPSData();

    int calculateSpainOffset();
    void convertToLocalTimeSpain();
};