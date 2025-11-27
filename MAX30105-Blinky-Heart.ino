#include <Wire.h>
#include "MAX30105.h" 
#include "heartRate.h" 

MAX30105 particleSensor;

// NASTAVITVE LED DIODE IN PWM
const int LED_PIN = 4; // Varna LED dioda za fade
const int MAX_BRIGHTNESS = 255; 
const int FADE_DURATION_MS = 300; 

// I2C pini 
const int SDA_PIN = 21; 
const int SCL_PIN = 22; 

// NASTAVITVE PBA ALGORITMA
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE]; 
byte rateSpot = 0;
long lastBeat = 0;
float beatsPerMinute;
int beatAvg; 

// SPREMENLJIVKE ZA GLADKO UTRIPANJE
unsigned long lastBlinkTime = 0;
unsigned long lastBlinkUpdate = 0;
const unsigned long UPDATE_INTERVAL_MS = 5000; 
int smoothBPM = 0; 

// SPREMENLJIVKE ZA FADE FUNKCIJO
bool fadeActive = false;
unsigned long fadeStartTime = 0;

void startHeartBeatFade() {
  fadeActive = true;
  fadeStartTime = millis();
}

void processFade() {
  if (!fadeActive) return;
  
  unsigned long timeElapsed = millis() - fadeStartTime;
  
  if (timeElapsed < FADE_DURATION_MS) {
    // Faza zbleditve (Fade Out)
    int brightness = MAX_BRIGHTNESS - (int)(MAX_BRIGHTNESS * timeElapsed / FADE_DURATION_MS);
    
    // Uporaba analogWrite()
    analogWrite(LED_PIN, brightness); 
  } else {
    // Zbleditev končana
    analogWrite(LED_PIN, 0); 
    fadeActive = false;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Inicializacija MAX30102. I2C na 21/22. LED fade aktiviran.");

  // Inicializacija I2C 
  Wire.begin(SDA_PIN, SCL_PIN); 
  Wire.setClock(400000); 

  // Inicializacija LED za PWM
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0); // Začetna ugasnjena LED

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) 
  {
    Serial.println("MAX30102 senzor ni bil najden. PREVERITE FIZIČNE POVEZAVE.");
    while (1);
  }
  
  particleSensor.setup(); 
  particleSensor.setPulseAmplitudeRed(0x0A); 
  particleSensor.setPulseAmplitudeGreen(0); 
  particleSensor.setFIFOAverage(4); 
  
  Serial.println("Položite prst na senzor.");
}

void loop()
{
  long irValue = particleSensor.getIR();
  unsigned long currentTime = millis();

  // Izvajanje logike zbleditve
  processFade(); 

  // 1. PBA ALGORITEM (Izračun hitrega BPM)
  if (checkForBeat(irValue) == true)
  {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 180 && beatsPerMinute > 40)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; 
      rateSpot %= RATE_SIZE; 
      
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  // 2. POSODABLJANJE gladkega BPM (vsakih 5 sekund)
  if (currentTime - lastBlinkUpdate >= UPDATE_INTERVAL_MS) { 
      if (beatAvg > 0) {
          smoothBPM = beatAvg; 
      }
      lastBlinkUpdate = currentTime; 
  }

  // 3. LOGIKA UTRIPANJA LED GLEDE NA SMOOTH BPM
  if (smoothBPM > 0) {
      unsigned long pulseInterval = 60000 / smoothBPM; 
      
      if (!fadeActive && (currentTime - lastBlinkTime >= pulseInterval)) {
           startHeartBeatFade(); 
           lastBlinkTime = currentTime; 
      }
  }


  // 4. LOGIKA PRSTA 
  if (irValue < 50000) {
    if (smoothBPM == 0) {
        analogWrite(LED_PIN, 0); 
    } 

    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print(", Smooth BPM=");
    Serial.print(smoothBPM);
    Serial.print(" **PRST UMAKNJEN**");
  }
  else
  {
    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print(", Smooth BPM=");
    Serial.print(smoothBPM);
  }
  
  Serial.println();
}

