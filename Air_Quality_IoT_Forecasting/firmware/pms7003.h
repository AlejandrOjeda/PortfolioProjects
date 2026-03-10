#ifndef PMS7003_SENSOR_H // Pregunto se ha definido el sensor 
#define PMS7003_SENSOR_H // si no se a definido lo defino 

// Incluyo bibliotecas necesarias para trabajar 
#include <InfluxDbClient.h> //Incluye la librería para trabajar con InfluxDB (para enviar datos)
#include <PMS.h>
#include <Arduino.h>
#include <PMS.h>


void iniciarPMS();
void leerPMS(Point &sensorPoint);

#endif
