#include "sensor_lux.h"

// Bus I2C #1 para el BH1750 (separado del Wire principal)
TwoWire WireBH(1);
BH1750  lightMeter;
static bool bh_ok = false; // estado del BH1750

void iniciarBH1750() {

  WireBH.begin(19, 18);  // BH1750 en SDA=19, SCL=18 a 100 kHz
  WireBH.setClock(100000);

  // Ajusta la dirección según tu pin ADDR:
  // 0x23 si ADDR a GND, 0x5C si ADDR a VCC
  const uint8_t bh_addr = 0x23;

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, bh_addr, &WireBH)){
  // Lectura de descarte y pequeño tiempo para estabilizar 
  (void)lightMeter.readLightLevel();
    delay(200);

    bh_ok= true;

    Serial.println("BH1750 inicializado en bus WireBH (SDA=19, SCL=18)");
  } else {
    Serial.println("Error al inicializar BH1750 en bus WireBH");
  }
}

bool leerBH1750(Point &punto) {
  if (!bh_ok) {
    Serial.println("BH1750 no inicializado: omitiendo lectura");
    return false;
  }

  float lux = lightMeter.readLightLevel();

  if (lux >= 0) {
    punto.addField("lux", lux);
    Serial.print("Luminosidad: ");
    Serial.print(lux);
    Serial.println(" lx");
    return true;
  } else {
    Serial.println("Error al leer BH1750");
    return false;
  }
}