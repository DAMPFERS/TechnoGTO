void setup() {
  for(int i = 4; i <= 7; i++)
    pinMode(i, INPUT);
  Serial.begin(9600);
  pinMode(11, INPUT_PULLUP);
  while (1) {
  Serial.println(digitalRead(11));
  delay(500);
  }
  
}

void loop() {
  uint8_t mas[4] = {A0, A1, A2, A3};
  uint8_t res = 0;
  for(int i = 0; i < 4; i++){
    res += digitalRead(mas[i]);
    res = res << 1;
  }
  Serial.println(res >> 1, BIN);
  delay(500);
}
