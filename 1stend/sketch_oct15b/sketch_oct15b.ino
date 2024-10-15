#include <NecDecoder.h>

#define TIME_LEVEL_1 600
#define TIME_LEVEL_2 600
#define TIME_LEVEL_3 600
#define PIN_KEY_SET 12

#define PIN_ZERO_SECTION 3
#define PIN_FIRST_SECTION 4
#define PIN_SECOND_SECTION 5
#define PIN_THIRD_SECTION 6
#define PIN_ANODE_0 7
#define PIN_ANODE_1 8
#define PIN_ANODE_2 9
#define PIN_ANODE_3 10


#define PIN_SOCKET A0

#define PIN_ETHERNET_1 A1
#define PIN_ETHERNET_2 A2
#define PIN_ETHERNET_3 A3
#define PIN_ETHERNET_4 A4

#define ON_IK 0x1E
#define OFF_IK 0x1F
#define IK_INTERRUPT 2 // const
#define PIN_POWER_LED 11


const uint8_t mas_sections[4] = {PIN_ZERO_SECTION, PIN_FIRST_SECTION, PIN_SECOND_SECTION, PIN_THIRD_SECTION};
const uint8_t mas_anods[4] = {PIN_ANODE_0, PIN_ANODE_1, PIN_ANODE_2, PIN_ANODE_3};

bool level_1_start = false;
bool level_1_end = false;
bool level_2_start = false;
bool level_2_end = false;
bool level_3_start = false;
bool level_3_end = false;

bool level_1_result;
bool level_2_result;
bool level_3_result;

bool read_key_set = true;
bool old_state_key_set;
bool finish_game = false;

uint16_t count_second_level_1 = TIME_LEVEL_1;
uint16_t count_second_level_2 = TIME_LEVEL_2;
uint16_t count_second_level_3 = TIME_LEVEL_3;

long int timer = 0;

NecDecoder ir;

void setStartLevelFlag();

bool levelOne();
bool _readSocket();

bool levelTwo();
bool _readEthernet();

bool levelThree();

void irIsr();

void _powerLed(bool state);

void _setNumber(uint8_t number);
void _setAnode(uint8_t anode);
void timeOutput(uint16_t t);

void setup() {
  pinMode(PIN_KEY_SET, INPUT_PULLUP);
  
  pinMode(PIN_ZERO_SECTION, OUTPUT);
  pinMode(PIN_FIRST_SECTION, OUTPUT);
  pinMode(PIN_SECOND_SECTION, OUTPUT);
  pinMode(PIN_THIRD_SECTION, OUTPUT);
  pinMode(PIN_ANODE_0, OUTPUT);
  pinMode(PIN_ANODE_1, OUTPUT);
  pinMode(PIN_ANODE_2, OUTPUT);
  pinMode(PIN_ANODE_3, OUTPUT);

  pinMode(PIN_SOCKET, INPUT);

  pinMode(PIN_ETHERNET_1, INPUT);
  pinMode(PIN_ETHERNET_2, INPUT);
  pinMode(PIN_ETHERNET_3, INPUT);
  pinMode(PIN_ETHERNET_4, INPUT);

  attachInterrupt(0, irIsr, FALLING);
  pinMode(PIN_POWER_LED, OUTPUT);

  old_state_key_set = !digitalRead(PIN_KEY_SET);
  Serial.begin(9600);
}

void loop() {
  if (read_key_set){
    bool new_state_key_set = !digitalRead(PIN_KEY_SET);
    if ((new_state_key_set == HIGH) && (old_state_key_set == LOW)){
        setStartLevelFlag();
        read_key_set = false;
        timer = millis();
    }
    old_state_key_set = new_state_key_set;
  }
  
  if ((level_1_start == true) && (level_1_end == false)){
    if (levelOne() == true)
      read_key_set = true;
  }

  if ((level_2_start == true) && (level_2_end == false)){
    if (levelTwo() == true)
      read_key_set = true;
  }

  if ((level_3_start == true) && (level_3_end == false)){
    if (levelThree() == true)
      finish_game = true;
  }

  if (finish_game){
    if (ir.available()) {
    // выводим весь пакет (32 бита)
    //Serial.print("0x");
    //Serial.println(ir.readPacket(), HEX);
    // можно вывести только команду (8 бит)
    uint8_t res;
    Serial.println(res = ir.readCommand(), HEX);
    if (res == ON_IK)
      _powerLed(true);
    else if (res == OFF_IK)
      _powerLed(false);
    }
  }

}


void timeOutput(uint16_t t){
  
  return;
}


void _setNumber(uint8_t number){
  for(int i = 0; i < 4; i++){
    digitalWrite(mas_sections[i], number & 0x01);
    number = number >> 1;
  }
  return;
}


void _setAnode(uint8_t anode){
  for(int i = 0; i < 4; i++){
    if(i != anode)
      digitalWrite(mas_anods[i], LOW);
    else
      digitalWrite(mas_anods[i], HIGH);
  }
  return;
}



void setStartLevelFlag(){
  if (!level_1_start)
    level_1_start = true;
  else if (!level_2_start)
      level_2_start = true;
      else if (!level_3_start)
        level_3_start = true;
  return;
}



bool levelOne(){
  while (count_second_level_1 != 0) {
    if (_readSocket()){
      level_1_end = true;
      level_1_result = true;
      return true;
   }

   if (millis() - timer >= 1000){
    --count_second_level_1;
    timer = millis();
   }
  }

  level_1_end = true;
  level_1_result = false;
  return true;
}


bool _readSocket(){
  if (digitalRead(PIN_SOCKET) == HIGH)
    return true;
  else
    return false;
}

bool levelTwo(){
  while (count_second_level_2 != 0) {
  if (_readEthernet()){
      level_2_end = true;
      level_2_result = true;
      return true;
  }
  if (millis() - timer >= 1000){
    --count_second_level_2;
    timer = millis();
  }

  }
  level_2_end = true;
  level_2_result = false;
  return true;
}


bool _readEthernet(){
  uint8_t res = 0;
  uint8_t pins_ethernet[4] = {PIN_ETHERNET_1, PIN_ETHERNET_2, PIN_ETHERNET_3, PIN_ETHERNET_4};
  for(int i = 0; i < 4; i++){
    res = res << 1;
    res += digitalRead(pins_ethernet[i]);
  }
  if (res == 0)
    return true;
  else
    return false;
}


bool levelThree(){
  
  while (count_second_level_3 != 0) {
    if (ir.available()) {
    // выводим весь пакет (32 бита)
    //Serial.print("0x");
    //Serial.println(ir.readPacket(), HEX);

    // можно вывести только команду (8 бит)
    uint8_t res;
    Serial.println(res = ir.readCommand(), HEX);
    if (res == ON_IK){
      _powerLed(true);
      level_3_end = true;
      level_3_result = true;
      return true;
    }
      
    else if (res == OFF_IK){
      _powerLed(false);
    }
      
    }
    
    
    
    
    if (millis() - timer >= 1000){
      --count_second_level_3;
      timer = millis();
    }

  }
  level_3_end = true;
  level_3_result = false;
  return true;
  
}

void irIsr(){
  ir.tick();
}

void _powerLed(bool state){
  if (state)
    digitalWrite(PIN_POWER_LED, HIGH);
  else
    digitalWrite(PIN_POWER_LED, LOW);
  return;
}

