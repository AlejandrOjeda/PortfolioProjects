
#include "pms7003.h"

PMS pms(Serial2); // creo objeto pms que usara el puerto de comunicacion serial 2
PMS::DATA data; // creo una estructura donde se van a guardar los datos(contenedor) esto es por como funciona la libreria 

void iniciarPMS() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX=16 (al TX del sensor), TX=17 (al RX del sensor)
  pms.passiveMode();  // Para evitar que mande datos todo el rato
  Serial.println("PMS7003 inicializado en modo pasivo");
}

void leerPMS(Point &sensorPoint) {
  pms.wakeUp();
  delay(30000);  // Esperar un poco para que se estabilice 30 seg segun fabricante 

  pms.requestRead();
  if (pms.readUntil(data)) {
    sensorPoint.addField("pm1_0", data.PM_AE_UG_1_0);
    sensorPoint.addField("pm2_5", data.PM_AE_UG_2_5);
    sensorPoint.addField("pm10_0", data.PM_AE_UG_10_0);


    Serial.print("PM1.0: "); Serial.print(data.PM_AE_UG_1_0);
    Serial.print("  PM2.5: "); Serial.print(data.PM_AE_UG_2_5);
    Serial.print("  PM10: "); Serial.println(data.PM_AE_UG_10_0);
  } else {
    Serial.println("Error al leer datos del PMS7003");
  }

  pms.sleep();
}