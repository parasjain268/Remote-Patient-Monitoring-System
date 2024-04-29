
#define BLYNK_TEMPLATE_ID "TMPL3lNDUgDoA"
#define BLYNK_TEMPLATE_NAME "SMART MEDICAL SYSTEM"

#include <OneWire.h>
#include <DallasTemperature.h>
#include <PulseSensorPlayground.h>

#define BLYNK_FIRMWARE_VERSION "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
#define USE_ESP32_DEV_MODULE
//#define USE_ESP32C3_DEV_MODULE
//#define USE_ESP32S2_DEV_KIT
//#define USE_WROVER_BOARD
//#define USE_TTGO_T7
//#define USE_TTGO_T_OI

#include "BlynkEdgent.h"

#define DS18B20PIN 16
#define SENSOR A6
#define PulseWire 36

float ECGSENSOR;
int Threshold = 550;

/* Create an instance of OneWire */
OneWire oneWire(DS18B20PIN);
DallasTemperature sensor(&oneWire);
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

void setup() {
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();

  sensor.begin();
  pinMode(13, INPUT);  // Setup for leads off detection LO +
  pinMode(15, INPUT);  // Setup for leads off detection LO -

  // Configure the PulseSensor object, by assigning our variables to it.
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);

  // Double-check the "pulseSensor" object was created and "began" seeing a signal.
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");
  }
}

void loop() {
  BlynkEdgent.run();

  sensor.requestTemperatures();
  float tempinC = sensor.getTempCByIndex(0);
  Serial.print("Temperature = ");
  Serial.print(tempinC);
  Serial.println("ºC");
  Blynk.virtualWrite(V0, tempinC);

  // ecg
  if ((digitalRead(15) == 1) || (digitalRead(13) == 1)) {
    Serial.println('!');
  } else {
    // send the value of analog input 0:
    ECGSENSOR = analogRead(A6);
    Serial.println(analogRead(A6));
    Blynk.virtualWrite(V2, ECGSENSOR);
  }

  if (pulseSensor.sawStartOfBeat()) {              // Constantly test to see if "a beat happened".
    int myBPM = pulseSensor.getBeatsPerMinute();   // Calls function on our pulseSensor object that returns BPM as an "int".
    Serial.println("♥  A HeartBeat Happened ! ");  // If test is "true", print a message "a heartbeat happened".
    Serial.print("BPM: ");                         // Print phrase "BPM: "
    Serial.println(myBPM);                         // Print the value inside of myBPM.
    Blynk.virtualWrite(V1, myBPM);
  }

  delay(20);
}
