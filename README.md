# 🔋 IoT Battery Monitor using ESP8266

This project monitors battery voltage, temperature, and humidity using:
- 🌡️ DHT11 sensor
- 🔋 Analog battery input
- 📟 16x2 I2C LCD
- 🌐 ESP8266 Web Server
- 🔔 Buzzer alert at high temperature
- ⚠️ Load cut-off at critical temperature

## 📷 Web Interface
Shows real-time values:
- Voltage
- Temperature
- Humidity
- Buzzer & Load status

## 📡 Circuit Diagram
See `circuit_diagram.png` for hardware connections.

## 📂 Files Included
- `battery_monitor.ino`: Main source code
- `circuit_diagram.png`: Circuit schematic
- `images/`: Screenshots (optional)

## 🛠️ Tools Used
- Arduino IDE / VS Code
- ESP8266 Core
- DHT11 Library
- LiquidCrystal_PCF8574 library
