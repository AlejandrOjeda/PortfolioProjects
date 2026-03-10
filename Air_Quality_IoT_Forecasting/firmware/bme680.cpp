#include "bme680.h"

Adafruit_BME680 bmeSensor;
bool bme_ok = false;

// Config parámetros de la lectura
static void configurarBME() {
  bmeSensor.setTemperatureOversampling(BME680_OS_8X);
  bmeSensor.setHumidityOversampling(BME680_OS_2X);
  bmeSensor.setPressureOversampling(BME680_OS_4X);
  bmeSensor.setGasHeater(320, 150);  // 320 ºC durante 150 ms
}

bool iniciarBME680() {
  if (!bmeSensor.begin(0x77, &Wire)) {
    Serial.println("Error iniciando BME680 en 0x77");
    bme_ok = false;
    return false;
  }

  configurarBME();
  bme_ok = true;
  Serial.println("BME680 inicializado en direccion 0x77");
  return true;
}

bool leerBME680(Point &sensorPoint) {
  if (!bme_ok) {
    if (!iniciarBME680()) return false;
    Serial.println("BME680 detectado tras reintento");
  }

  if (!bmeSensor.performReading()) {
    Serial.println("Error lectura BME680 en 0x77");
    return false;
  }

  // Añadir campos al Point (para InfluxDB)
  sensorPoint.addField("temp", bmeSensor.temperature);
  sensorPoint.addField("hum",  bmeSensor.humidity);
  sensorPoint.addField("pres", bmeSensor.pressure / 100.0);
  sensorPoint.addField("gas",  bmeSensor.gas_resistance / 1000.0);

  // Log de depuración
  Serial.print("BME680 T="); Serial.print(bmeSensor.temperature);
  Serial.print(" C  RH=");   Serial.print(bmeSensor.humidity);
  Serial.print(" %  P=");    Serial.print(bmeSensor.pressure / 100.0);
  Serial.print(" hPa  Gas(kΩ)=");
  Serial.println(bmeSensor.gas_resistance / 1000.0);

  return true;
}
