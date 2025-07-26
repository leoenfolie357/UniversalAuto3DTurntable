#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define PIN_SW1   12  // D6 : Start/Stop cycle
#define PIN_SW4   13  // D7 : Change cadence
#define PIN_SWBT  0   // D3 : Déclenchement manuel
#define PIN_SWREM 5   // D1 : Sortie contact
#define PIN_BUZZER 14 // D5 : Buzzer
#define PIN_LED    2  // D4 : LED embarquée
#define PIN_IR_LED 4  // D2 : LED IR

IRsend irsend(PIN_IR_LED);
uint16_t nikonPulse[] = {2000, 27850, 390, 1580, 410, 3580, 400, 63200};

const int cadences[] = {0, 10, 20, 30, 40};
int cadenceIndex = 1;
bool isRunning = false;

void setup() {
  Serial.begin(9600);
  irsend.begin();

  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW4, INPUT_PULLUP);
  pinMode(PIN_SWBT, INPUT_PULLUP);
  pinMode(PIN_SWREM, OUTPUT); digitalWrite(PIN_SWREM, HIGH);
  pinMode(PIN_BUZZER, OUTPUT); digitalWrite(PIN_BUZZER, LOW);
  pinMode(PIN_LED, OUTPUT); digitalWrite(PIN_LED, HIGH); // LED ON = repos
}

void loop() {
  static bool lastSW1 = HIGH, lastSW4 = HIGH, lastSWBT = HIGH;
  bool currSW1 = digitalRead(PIN_SW1);
  bool currSW4 = digitalRead(PIN_SW4);
  bool currSWBT = digitalRead(PIN_SWBT);

  if (lastSW4 == HIGH && currSW4 == LOW) {
    cadenceIndex = (cadenceIndex + 1) % 5;
    signalBip();
    Serial.print("CADENCE:");
    Serial.println(cadences[cadenceIndex]);
  }

  if (lastSW1 == HIGH && currSW1 == LOW) {
    isRunning = !isRunning;
    signalBip();
    digitalWrite(PIN_LED, !isRunning); // LED OFF = en cours
    Serial.println(isRunning ? "START" : "STOP");
  }

  if (lastSWBT == HIGH && currSWBT == LOW) {
    declencher();
  }

  lastSW1 = currSW1;
  lastSW4 = currSW4;
  lastSWBT = currSWBT;

  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();
    if (msg == "SHOOT") {
      declencher(); // émission IR + contact
    }
  }
}

void declencher() {
  signalBip();
  irsend.sendRaw(nikonPulse, sizeof(nikonPulse) / sizeof(nikonPulse[0]), 38);
  digitalWrite(PIN_SWREM, LOW);
  delay(200);
  digitalWrite(PIN_SWREM, HIGH);
}

void signalBip() {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(200);
  digitalWrite(PIN_BUZZER, LOW);
}
