💖 ESP32 Heart Rate LED Pulse
This repository contains the Arduino sketch for an ESP32 project that reads heart rate data from the MAX30105 Pulse Oximeter Sensor and translates the live beats per minute (BPM) into a smooth, rhythmic pulse on an LED.

✨ Features
Heart Rate Sensing: Uses the MAX30105 sensor to detect blood flow and calculate BPM.

I2C Communication: Configured for the default ESP32 I2C pins (SDA 21, SCL 22).

Smooth Visual Pulse: The LED fades up and down over a 300ms duration on every detected heartbeat.

BPM Synchronization: The blinking interval is dynamically adjusted to synchronize with the calculated average heart rate.

No Finger Detection: Logs "No finger?" to the Serial Monitor and turns off the LED if the IR reading is too low.

🛠️ Hardware Requirements
Component	Description
Microcontroller	ESP32 Development Board (e.g., ESP32-DevKitC)
Sensor	MAX30105 Pulse Oximeter and Heart Rate Sensor
LED	5mm LED (any color)
Resistor	Current-limiting Resistor (e.g., 220 Ohm)
Cables	Jumper Wires

🔌 Wiring Guide
The sketch uses the specified I2C pins for the MAX30105 and GPIO 4 for the LED.
MAX30105 VCC	3.3V	Power supply
MAX30105 GND	GND	Ground
MAX30105 SDA	GPIO 21	I2C Data Pin
MAX30105 SCL	GPIO 22	I2C Clock Pin
LED Anode (+)	GPIO 4 (via Resistor)	PWM Output
LED Cathode (-)	GND	Ground

⚙️ Software Setup
Arduino IDE & Libraries
Install the ESP32 Board Manager: Ensure you have the ESP32 core installed in the Arduino IDE.

Install Libraries: Open the Arduino Library Manager and install the following dependencies:

SparkFun MAX30105 (by SparkFun Electronics)

SparkFun Heart Rate Algorithm (or similar accompanying library used for heartRate.h)

▶️ Usage
Open HeartRateLED.ino in the Arduino IDE.

Select your ESP32 board and the correct COM port.

Upload the code.

Open the Serial Monitor at 115200 baud to view real-time BPM and sensor data.

Place your finger gently but firmly over the sensor. After a few seconds, the LED will begin to pulse at your current heart rate.
