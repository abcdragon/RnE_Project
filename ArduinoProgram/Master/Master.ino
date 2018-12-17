#include <PMsensor.h>
#include <SoftwareSerial.h>

#define sensitivity 0.1

SoftwareSerial mySerial(8, 9); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정
PMsensor PM;

short sensorPin = A0;
short sensorLED = 3;
float out = 0.0f;

void setup() {
  // 시리얼 통신의 속도를 9600으로 설정
  Serial.begin(9600);
  PM.init(sensorPin, sensorLED);
  Serial.println("Hello I'm working now"); // 블루투스의 Role 이 1임을 알리는 문구
  //블루투스와 아두이노의 통신속도를 9600으로 설정
  mySerial.begin(9600);
}
void sensing();
void loop() { //코드를 무한반복합니다.
  sensing();
  int tmp = (int)out * 100 + (int)(100 * (out - (int)out)); // 4자리 정수로 만들어 주는 과정
  String sData = String(tmp);
  Serial.println(sData);
  //mySerial.write(String((int)data * 100 + (int)(100 * (data - (int)data))));
  mySerial.println(sData);
  delay(100); // 0.1초마다
}

//Qoskld;fj
void sensing(){
  int err = PMsensorErrSuccess;
  if((err = PM.read(&out, true, sensitivity)) != PMsensorErrSuccess){
    Serial.print("data Error = ");
    Serial.println(err);
    delay(1500);
  }
}
