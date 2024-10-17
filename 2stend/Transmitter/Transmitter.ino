#include <NecEncoder.h>

#define ON 4
#define OFF 5
#define IK_PIN 3

bool old_state_ON;
bool old_state_OFF;
NecEncoder enc(IK_PIN);


void setup() {
  pinMode(ON, INPUT_PULLUP);
  pinMode(OFF, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  old_state_ON = !digitalRead(ON);
  old_state_OFF = !digitalRead(OFF);
}

void loop() {
  bool new_state_ON = !digitalRead(ON);
  bool new_state_OFF = !digitalRead(OFF);

  if ((new_state_ON == HIGH) && (old_state_ON == LOW)){
    enc.send(0x1E, 0x1E);
    Serial.println("signal: ON");
    digitalWrite(13, HIGH);
    delay(10);
  }
  else if ((new_state_OFF == HIGH) && (old_state_OFF == LOW)){
    enc.send(0x1F, 0x1F);
    Serial.println("signal: OFF");
    digitalWrite(13, LOW);
    delay(10);
  }
  old_state_OFF = new_state_OFF;
  old_state_ON = new_state_ON;
}