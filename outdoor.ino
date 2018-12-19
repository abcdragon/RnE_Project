#include <SoftwareSerial.h>
#define CDS A0

SoftwareSerial mySerial(10, 11); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정
void setup() {
  pinMode(A0, INPUT);
  Serial.begin(9600);
  //PM.init(sensorPin, sensorLED);
  mySerial.begin(9600);
}
bool FLAG = false;
unsigned long prev_time = 0, current_time = 0;
void loop() {
  current_time = millis();
  if(current_time - prev_time > 1000) {  // 현재시각과 미리 저장해둔 시각사이의 시간이 1000을 넘기면
    stop(); // 멈추고
    FLAG = false; // FLAG를 거짓으로 돌리기
  }
  int aaa = analogRead(A0);

  if(!FLAG && aaa > 750) {   // FLAG = false 이면서 aaa는 750 이상의 값일 때
    FLAG = true; ford(); // FLAG = true로 바꾸고, ford()(돌리는 함수)를 실행, prev_time 에 메모
    prev_time = millis();
  }
  
  if(Serial.available()){
    mySerial.write(Serial.read());  
  }
}
