// приём команд с пульта
// приёмник подключать на прерывание по FALLING

#include <NecDecoder.h>
#define ON_IK 0x1E
#define OFF_IK 0x1F

#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;

NecDecoder ir;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  // подключил на D2, прерывание 0
  attachInterrupt(0, irIsr, FALLING);


  oled.init();        // инициализация
  oled.clear();       // очистка
  oled.setScale(3);   // масштаб текста (1..4)
  oled.setCursor(0, 3);
  oled.print("Start!");
  // oled.home();        // курсор в 0,0
  // oled.print("Привет!");

  // delay(1000);

  // oled.setScale(1);
  // курсор на начало 3 строки
  

  // курсор на (20, 50)
  // oled.setCursorXY(20, 50);
  // float pi = 3.14;
  // oled.print("PI = ");
  // oled.print(pi);
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
    oled.clear();       // очистка
    oled.setCursor(0, 3);
    oled.print("S: 0x");
    oled.print(res, HEX);
    if (res == 0x1E)
      digitalWrite(13, HIGH);
    else if (res == 0x1F)
      digitalWrite(13, LOW);
  }
}