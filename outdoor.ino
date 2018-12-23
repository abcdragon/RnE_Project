#include <SoftwareSerial.h>

#define sensitivity 0.1

#define CDS A0
#define MA_F 6
#define MA_B 9
#define MB_F 3
#define MB_B 5
#define DUST_LED_PIN 2 // 먼지 센서 IR LED 핀
#define DUST_ANALOG_PIN A1 // 먼지 센서 받는 핀

SoftwareSerial mySerial(10, 11); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정

int led = 13;
byte speed = 255;

void setup() {
  pinMode(CDS, INPUT);
  
  pinMode(MA_F, OUTPUT);
  pinMode(MA_B, OUTPUT);
  pinMode(MB_F, OUTPUT);
  pinMode(MB_B, OUTPUT);

  pinMode(DUST_ANALOG_PIN, INPUT);
  pinMode(DUST_LED_PIN, OUTPUT);
  
  Serial.begin(9600);
  mySerial.begin(9600);
}
bool FLAG = false;
unsigned long prev_time = 0, current_time = 0;

void spin(){
  analogWrite(MA_F, 0);
  analogWrite(MA_B, 255);
  analogWrite(MB_F, 255);
  analogWrite(MB_B, 0);  
}

void stop(){
  analogWrite(MA_F, 0);
  analogWrite(MA_B, 0);
  analogWrite(MB_F, 0);
  analogWrite(MB_B, 0);  
}

void sensing(){
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(280);
  float dust = analogRead(DUST_ANALOG_PIN);
  delayMicroseconds(40);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(9680);
  float calcVolt = dust * (5.0 / 1024);
  float result = (0.17 * calcVolt - 0.1) * 1000;
  Serial.println(result);
  mySerial.println(result);
}

void loop() {
  int cValue = analogRead(CDS); //Serial.println(cValue);
  if(millis() - prev_time > 3000) { // millis 를 이용해서 3초간 모터 돌리기
    FLAG = false;
    stop();
  }
  if(!FLAG && cValue > 750){
    Serial.print("Opened door");
    prev_time = millis();
    FLAG = true;
    spin();
  }
  sensing();
  delay(350);
}
