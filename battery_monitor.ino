#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h> // LCD library for ESP8266
#include <DHT.h>

// WiFi Credentials
const char* ssid = "IOT_BATTERY";
const char* password = "12345678";

// Web Server on Port 80
ESP8266WebServer server(80);

// DHT Sensor Configuration
#define DHTPIN D4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Battery Voltage Sensor (Analog Pin)
#define BATTERY_PIN A0

// LCD Configuration
LiquidCrystal_PCF8574 lcd(0x27); // I2C LCD Address (0x27 or 0x3F)

// Buzzer & Load Pins
#define BUZZER_PIN D5
#define LOAD_PIN D6

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // LCD Setup
    lcd.begin(16, 2);
    lcd.setBacklight(255);
    lcd.clear();
    lcd.print("IOT Battery");

    // DHT Sensor Setup
    dht.begin();

    // Buzzer & Load Setup
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LOAD_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LOAD_PIN, HIGH); // Load is initially ON

    // Web Server Route
    server.on("/", handleWebpage);
    server.begin();
    Serial.println("Web Server Started!");
}

void loop() {
    server.handleClient(); // Handle Web Requests

    // Read DHT Sensor Values
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Read Battery Voltage
    float voltage = (analogRead(BATTERY_PIN) * 3.3 * 2) / 1023.0;

    // Display on Serial Monitor
    Serial.print("Voltage: ");
    Serial.print(voltage, 2);
    Serial.println("V");
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.println("C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    // Display on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("V: ");
    lcd.print(voltage, 2);
    lcd.print("V");
    lcd.setCursor(0, 1);
    lcd.print("T: ");
    lcd.print(temperature, 1);
    lcd.print("C");
    lcd.setCursor(9, 1);
    lcd.print("H: ");
    lcd.print(humidity, 0);
    lcd.print("%");

    // Trigger Buzzer if Temp ≥ 37°C
    if (temperature >= 37) {
        digitalWrite(BUZZER_PIN, HIGH);
    } else {
        digitalWrite(BUZZER_PIN, LOW);
    }

    // Turn OFF Load if Temp ≥ 40°C
    if (temperature >= 40) {
        digitalWrite(LOAD_PIN, LOW);
    } else {
        digitalWrite(LOAD_PIN, HIGH);
    }

    delay(2000);
}

// Web Page Handler
void handleWebpage() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    float voltage = (analogRead(BATTERY_PIN) * 3.3 * 2) / 1023.0;

    String html = "<html><body><h2>IOT Battery Monitor</h2>";
    html += "<p><b>Voltage:</b> " + String(voltage, 2) + "V</p>";
    html += "<p><b>Temperature:</b> " + String(temperature, 1) + "C</p>";
    html += "<p><b>Humidity:</b> " + String(humidity, 0) + "%</p>";

    if (temperature >= 37) {
        html += "<p><b>Buzzer:</b> ON</p>";
    } else {
        html += "<p><b>Buzzer:</b> OFF</p>";
    }

    if (temperature >= 40) {
        html += "<p><b>Load:</b> OFF (Overheating)</p>";
    } else {
        html += "<p><b>Load:</b> ON</p>";
    }

    html += "</body></html>";
    server.send(200, "text/html", html);
}