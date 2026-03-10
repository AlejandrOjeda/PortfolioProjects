#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <WiFiMulti.h>
#include <Wire.h>
#include <time.h>
#include <Arduino.h>
#include "bme680.h"
#include "sensor_lux.h"
#include "pms7003.h"


#define DEVICE "ESP32"

// Credenciales WiFi
#define WIFI_SSID "your_wifi"
#define WIFI_PASSWORD "your_password"

// Parámetros para InfluxDB Cloud
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "your_token"
#define INFLUXDB_ORG "946d01148ddbf751"
#define INFLUXDB_BUCKET "Ambiental"
#define TZ_INFO "CET-1CEST,M3.5.0/2,M10.5.0/3"

WiFiMulti wifiMulti;
InfluxDBClient client(
  INFLUXDB_URL, 
  INFLUXDB_ORG, 
  INFLUXDB_BUCKET, 
  INFLUXDB_TOKEN
);

Point sensorPoint("ambiente");// Point es la manera de enviarle un paquete de datos a la base de datos, creo ambiente que es una tabla dentro de mi bucket ambiental 

void conectarWiFi() {
  Serial.print("Conectando a WiFi...");
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);  // Aquí se añade la red WiFi
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}



void setup() {
  Serial.begin(115200); //Me conecto con el serial monitor a 115200 bautios  importante que el serial Monitor tambien este a los mismo bautios 
  Wire.begin(21, 22);  // aqui le digo que pines GPIO utilizo de mi ESP32 
  client.setInsecure(); //InfluxDB Cloud funciona sobre HTTPS (con cifrado y certificados).El ESP32 no tiene por defecto el certificado raíz que se necesita para verificar que el servidor de InfluxDB es legítimo.
  //Entonces, si no usas setInsecure(), la conexión se corta por seguridad (como si el navegador dijera “certificado no válido”).
  
 
 conectarWiFi(); // ejecuto la funcion creada arriba para conectarme con el WiFi 

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");  //  Le  permite al ESP32 conectarse serivdor NTP Estoy en horario de Europa Central (España, Francia...). Ajusta automáticamente el horario de verano e invierno.”
  iniciarBME680(); // inicio el sensor BME680 
  iniciarBH1750();// inicio el sensor BH1750 
  iniciarPMS(); // inicio el sensor pms7003

}

void loop() {
  leerBME680(sensorPoint); //Aqui las lecturas del BME680 se van al objeto sensorpoint 
  leerBH1750(sensorPoint); //Aqui las lecturas del BME680 se van al objeto sensorpoint 
  leerPMS(sensorPoint); // Aqui las lecturas del pms 7003 se van al objeto sensor point 


  if (!client.writePoint(sensorPoint)) {
  Serial.print("Error al enviar datos a InfluxDB: ");
  Serial.println(client.getLastErrorMessage());
} else {
  Serial.println("Datos enviados correctamente"); // cliente si no consigue enviar el paquete de datos me dice he no puedo enviar si no todo correcto.
}




  delay(300000);  //
}
