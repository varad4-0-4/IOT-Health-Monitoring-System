#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <TinyGPS++.h>

// LCD settings
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Blynk settings
#define BLYNK_PRINT Serial
char auth[] = "UFrVIYMCHq5ZYgSiQNDbHvYJ6M7x_E9m";
char ssid[] = "ADMIN";
char pass[] = "ADMIN";

TinyGPSPlus gps;
SimpleTimer timer;

float latitude, longitude;
String lat_str, lng_str;
int hr;

void sendSensor() {
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C) {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  }

  int sensorValue = analogRead(A0);
  if ((sensorValue > 800) && (sensorValue < 1050)) {
    if (sensorValue > 900) {
      hr = countHeartRate(); // Ensure you have a function called countHeartRate()
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempC);
  lcd.setCursor(8, 0);
  lcd.print("HR:");
  lcd.print(hr);

  Blynk.virtualWrite(V1, tempC);
  Blynk.virtualWrite(V2, hr);

  if (tempC > 50) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(16, HIGH);
      delay(100);
      digitalWrite(16, LOW);
      delay(100);
    }
  }

  if (digitalRead(12) == 0) {
    digitalWrite(16, HIGH);
    delay(2000);
    if (digitalRead(12) == 0) {
      digitalWrite(14, LOW);
      Blynk.virtualWrite(V0, 0);
      digitalWrite(16, LOW);
      while (digitalRead(12) == 0);
    } else if (digitalRead(12) == 1) {
      digitalWrite(14, HIGH);
      Blynk.virtualWrite(V0, 255);
      int count = 1;
      while ((Serial.available() > 0) && (count != 0)) {
        if (gps.encode(Serial.read())) {
          if (gps.location.isValid()) {
            latitude = gps.location.lat();
            lat_str = String(latitude, 6);
            longitude = gps.location.lng();
            lng_str = String(longitude, 6);
            count = 0;
          }
          String body = "https://www.google.com/maps/place/" + String(latitude, 6) + "," + String(longitude, 6) + "/@" + String(latitude, 6) + "," + String(longitude, 6) + "/data=!3m1!1e3 Location Of Patient";
          Blynk.email("yourmail@gmail.com", "Emergency", body);
        }
      }
      count = 0;
      digitalWrite(16, LOW);
    }
  }
}

BLYNK_WRITE(V14) {
  int adata = param.asInt();
  if (adata == 1) {
    digitalWrite(14, HIGH);
    digitalWrite(16, HIGH);
    Blynk.virtualWrite(V0, 255);
    int count = 1;
    while ((Serial.available() > 0) && (count != 0)) {
      if (gps.encode(Serial.read())) {
        if (gps.location.isValid()) {
          latitude = gps.location.lat();
          lat_str = String(latitude, 6);
          longitude = gps.location.lng();
          lng_str = String(longitude, 6);
          count = 0;
        }
        String body = "https://www.google.com/maps/place/" + String(latitude, 6) + "," + String(longitude, 6) + "/@" + String(latitude, 6) + "," + String(longitude, 6) + "/data=!3m1!1e3 Location Of Patient";
        Blynk.email("yourmail@gmail.com", "Emergency", body);
      }
    }
    count = 0;
    digitalWrite(16, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Health Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("System");
  sensors.begin();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  pinMode(16, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, INPUT);
  digitalWrite(16, LOW);
  digitalWrite(14, LOW);
}

void loop() {
  Blynk.run();
  timer.run();
}
