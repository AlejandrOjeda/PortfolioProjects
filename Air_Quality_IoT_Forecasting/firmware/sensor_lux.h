#ifndef SENSOR_LUX_H
#define SENSOR_LUX_H

#include <Wire.h>
#include <BH1750.h>
#include <InfluxDbClient.h>

extern TwoWire WireBH;      // Bus I2C dedicado al BH1750
extern BH1750  lightMeter;  // Instancia del sensor

void iniciarBH1750();              // Inicializa el bus y el sensor
bool leerBH1750(Point &punto);     // Lee lux y añade el campo al Point

#endif // SENSOR_LUX_H
