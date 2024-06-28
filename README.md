<h1>Health Monitoring System</h1>

This project is a health monitoring and tracking system built on NodeMCU (ESP8266). It uses various sensors to monitor temperature and heart rate and integrates with the Blynk platform for IoT connectivity. The system also features GPS tracking and sends email alerts when certain conditions are met.

## Features

- **Temperature Monitoring**: Uses DallasTemperature library to read temperature from DS18B20 sensors.
- **Heart Rate Monitoring**: Reads analog input to determine heart rate.
- **LCD Display**: Displays temperature and heart rate on a 16x2 I2C LCD.
- **WiFi Connectivity**: Connects to WiFi using ESP8266 and sends data to Blynk.
- **GPS Tracking**: Uses TinyGPS++ to get GPS coordinates and sends email alerts with location.
- **Blynk Integration**: Sends sensor data to Blynk and receives commands from the Blynk app.
- **Email Alerts**: Sends email alerts when certain conditions are met.

## Components

- NodeMCU (ESP8266)
- DS18B20 Temperature Sensor
- LCD 16x2 I2C Display
- GPS Module
- Push Button
- LEDs and Resistors
- Connecting Wires
- Breadboard

## Libraries Used

- LiquidCrystal_I2C
- OneWire
- DallasTemperature
- ESP8266WiFi
- BlynkSimpleEsp8266
- SimpleTimer
- TinyGPS++
