# Firmware – Environmental Monitoring Station

This folder contains the firmware developed for the ESP32-based environmental monitoring station.

The firmware is responsible for:

- Reading environmental data from the sensors
- Packaging the measurements
- Sending the data to InfluxDB Cloud

## Sensors used

The station integrates the following sensors:

- **BME680**  
  Measures temperature, humidity, pressure and gas resistance (VOC indicator)

- **BH1750**  
  Measures ambient light intensity (lux)

- **PMS7003**  
  Measures particulate matter concentration (PM1.0, PM2.5, PM10)

## Communication

- **I2C** → BME680 and BH1750  
- **UART** → PMS7003

The ESP32 collects the sensor readings and periodically sends the measurements to the time-series database for storage and visualization.

## Files

- `Estacion_ambiental_v2.ino`  
  Main firmware file. Handles WiFi connection, sensor initialization and data transmission.

- `bme680.*`  
  Functions for initialization and reading of the BME680 sensor.

- `sensor_lux.*`  
  Functions for reading light intensity from the BH1750 sensor.

- `pms7003.*`  
  Functions for communication and data acquisition from the PMS7003 particle sensor.
