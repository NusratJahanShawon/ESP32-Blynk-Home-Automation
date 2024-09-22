#define BLYNK_TEMPLATE_ID "TMPL6NnVqMC1Y"
#define BLYNK_TEMPLATE_NAME "Digital Home Automation"
#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "99GicCZW0yIzEK5vuJiBTV0yRFTja7dX"; // Replace with your Blynk authentication token
char ssid[] = "Fab Lab IUB";         // Replace with your WiFi SSID
char pass[] = "fabbers@iub";     // Replace with your WiFi password

int mq_02 = 34;
int mq_05 = 35;
int ldrpin = 32;
int flamepin = 25;
int pirmotionpin = 33;

int relayOutput_mq_5 = 13;
int relayOutput_mq_2 = 12;
int relayOutput_ldrpin = 14;
int relayOutput_flamepin = 15;
int relayOutput_pirmotionpin = 18;

BLYNK_WRITE(V0) {
  bool value1 = param.asInt();
  // Check these values and turn the relay1 ON and OFF
  if (value1 == 1) {
    digitalWrite(relayOutput_pirmotionpin, LOW);
  } else {
    digitalWrite(relayOutput_pirmotionpin, HIGH);
  }
}
BlynkTimer timer;

void setup() {
    Serial.begin(115200);
    pinMode(flamepin, INPUT);
    pinMode(ldrpin, INPUT);
    pinMode(mq_02, INPUT);
    pinMode(mq_05, INPUT);
    pinMode(pirmotionpin, INPUT);
    pinMode(relayOutput_mq_5, OUTPUT);
    pinMode(relayOutput_mq_2, OUTPUT);
    pinMode(relayOutput_ldrpin, OUTPUT);
    pinMode(relayOutput_flamepin, OUTPUT);
    pinMode(relayOutput_pirmotionpin, OUTPUT);
    digitalWrite(relayOutput_pirmotionpin, HIGH);
    digitalWrite(relayOutput_mq_2, HIGH);
    digitalWrite(relayOutput_mq_5, HIGH);

    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Blynk.begin(auth, ssid, pass);

    timer.setInterval(1000L, readSensorsAndControlRelays);
}

void readSensorsAndControlRelays() {
    int mq_02_data = analogRead(mq_02);
    int mq_05_data = analogRead(mq_05);
    int ldrdata = digitalRead(ldrpin);
    int flamedata = digitalRead(flamepin);
    int pirmotiondata = digitalRead(pirmotionpin);

    Serial.println("MQ-2 Analog Value: " + String(mq_02_data));
    Serial.println("MQ-5 Analog Value: " + String(mq_05_data));
    Serial.println("Flame Sensor Value: " + String(flamedata));
    Serial.println("LDR Sensor Value: " + String(ldrdata));
    Serial.println("PIR Motion Sensor Value: " + String(pirmotiondata));

    if (pirmotiondata == 1) {
        Blynk.logEvent("motion detected");
    } 
    else if(pirmotiondata ==0){
        
    }

    if (flamedata == 0) {
      Blynk.logEvent("Flame detected");
        digitalWrite(relayOutput_flamepin, LOW);
    } else{
        digitalWrite(relayOutput_flamepin, HIGH);
    }

    if (ldrdata == 1) {
        digitalWrite(relayOutput_ldrpin, LOW);
    } else {
      Blynk.logEvent("Darkness Detected");
        digitalWrite(relayOutput_ldrpin, HIGH);
    }

    if (mq_02_data > 500) {
      Blynk.logEvent("Gas Detected");
        digitalWrite(relayOutput_mq_2, LOW);
    } else {
        digitalWrite(relayOutput_mq_2, HIGH);
    }

    if (mq_05_data > 500) {
        digitalWrite(relayOutput_mq_5, LOW);
    } else {
        digitalWrite(relayOutput_mq_5, HIGH);
    }

    // Send sensor data to Blynk app
    Blynk.virtualWrite(V1, mq_02_data);
    Blynk.virtualWrite(V2, mq_05_data);
    Blynk.virtualWrite(V3, flamedata);
    Blynk.virtualWrite(V4, ldrdata);
    Blynk.virtualWrite(V5, pirmotiondata);
}

void loop() {
    Blynk.run();
    timer.run();
}
