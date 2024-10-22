// приём команд с пульта
// приёмник подключать на прерывание по FALLING

#include <NecDecoder.h>
#define ON_IK 0x1E
#define OFF_IK 0x1F

NecDecoder ir;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  // подключил на D2, прерывание 0
  attachInterrupt(0, irIsr, FALLING);
}

// в прерывании вызываем tick()
void irIsr() {
  ir.tick();
}

void loop() {
  // если пакет успешно принят
  if (ir.available()) {
    // выводим весь пакет (32 бита)
    //Serial.print("0x");
    //Serial.println(ir.readPacket(), HEX);

    // можно вывести только команду (8 бит)
    uint8_t res;
    Serial.println(res = ir.readCommand(), HEX);
    if (res == 0x1E)
      digitalWrite(13, HIGH);
    else if (res == 0x1F)
      digitalWrite(13, LOW);
  }
}