void setup() {
  for(int i = 4; i <= 7; i++)
    pinMode(i, INPUT);
  Serial.begin(9600);
}

void loop() {
  uint8_t res = 0;
  for(int i = 4; i <= 7; i++){
    res += digitalRead(i);
    res = res << 1;
  }
  Serial.println(res >> 1, BIN);
  delay(500);
}
