#ifndef BME680_SENSOR_H               // Pregunto se ha definido el sensor 
#define BME680_SENSOR_H          // si no se a definido lo defino 

#include <Adafruit_BME680.h>   // Incluye la librería para trabajar con el sensor BME680
#include <InfluxDbClient.h> 
#include <Wire.h>   // Incluye la librería para trabajar con InfluxDB (para enviar datos)

extern Adafruit_BME680 bmeSensor; 
extern bool bme_ok ;
/* Declara una variable global externa llamada bmeSensor (la definición real está en el cpp)
 si no pongo el extern me diria que tengo multiples variables definidas*/
bool iniciarBME680();              // Declara la función que inicializa el sensor BME680
bool leerBME680(Point &sensorPoint); // Declara la función que lee datos del sensor y los añade al objeto Point para InfluxDB

#endif                          // Fin de la protección contra inclusiones múltiples
