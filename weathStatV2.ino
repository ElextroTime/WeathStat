#define BLYNK_TEMPLATE_ID "TMPL3Ja3JCmbW"
#define BLYNK_TEMPLATE_NAME "Weather Station"
#define BLYNK_AUTH_TOKEN "6EB5mUP8HB37mGrFdsQtlm51XyPBcm0Z"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Define the I2C address for the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define DHT11 pin and type
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

// Your WiFi credentials
char ssid[] = "Karthig";
char pass[] = "gkn__f14";

// Threshold values for alerts
float temperatureThreshold = 30.0; // Example: 30°C
float humidityThreshold = 70.0; // Example: 70%

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize I2C for LCD on new pins
  Wire.begin(0, 14); // SDA to GPIO0 (D3), SCL to GPIO14 (D5)

  // Initialize the DHT11 sensor
  dht.begin();

  // Initialize the BMP180 sensor
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  // Initialize the LCD
  lcd.begin(16, 2); // Specify the dimensions of the LCD
  lcd.backlight();
  lcd.clear();

  // Display a startup message
  lcd.setCursor(0, 0);
  lcd.print("Weather Station");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  sendSensorData();
  delay(2000); // Update every 2 seconds
}

void sendSensorData() {
  // Read temperature and humidity from DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Read pressure from BMP180
  float p = bmp.readPressure() / 100.0F; // Convert Pa to hPa

  // Print values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("Pressure: ");
  Serial.print(p);
  Serial.println(" hPa");

  // Send values to Blynk
  Blynk.virtualWrite(V1, t); // Temperature
  Blynk.virtualWrite(V2, h); // Humidity
  Blynk.virtualWrite(V3, p); // Pressure

  // Display values on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(h);
  lcd.print(" % Pr: ");
  lcd.print(p);
  lcd.print("hPa");

  // Check conditions for alerts
  
}
