#include <NecEncoder.h>

#define PIN_ON 4
#define PIN_OFF 5
#define IK_PIN 3

#define ON_IK 0x5F
#define OFF_IK 0xF5

bool old_state_ON;
bool old_state_OFF;
NecEncoder enc(IK_PIN);


void setup() {
  pinMode(PIN_ON, INPUT_PULLUP);
  pinMode(PIN_OFF, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  old_state_ON = !digitalRead(PIN_ON);
  old_state_OFF = !digitalRead(PIN_OFF);
}

void loop() {
  bool new_state_ON = !digitalRead(PIN_ON);
  bool new_state_OFF = !digitalRead(PIN_OFF);

  if ((new_state_ON == HIGH) && (old_state_ON == LOW)){
    enc.send(ON_IK, ON_IK);
    Serial.println("signal: ON");
    digitalWrite(13, HIGH);
    delay(10);
  }
  else if ((new_state_OFF == HIGH) && (old_state_OFF == LOW)){
    enc.send(OFF_IK, OFF_IK);
    Serial.println("signal: OFF");
    digitalWrite(13, LOW);
    delay(10);
  }
  old_state_OFF = new_state_OFF;
  old_state_ON = new_state_ON;
}