#include <SoftwareSerial.h>
#include <PMsensor.h>
//#include <Servo.h>
#include <Wire.h>
#define sensitivity 0.1
#define SLAVE 4

SoftwareSerial mySerial(8, 9); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정
PMsensor PM;
//Servo vent;

short sensorPin = A0;
short sensorLED = 3;

float out = 0.0f, in = 0.0f;
char sendMessage;

void setup() {
  Wire.begin(SLAVE);
  PM.init(sensorPin, sensorLED);
  //vent.attach(11);
  Serial.begin(9600);
  mySerial.begin(9600);
  //Serial.println("Hello World! I'm Slave");
  //vent.write(0);
  Wire.onRequest(sendToMaster);
  pinMode(13, OUTPUT);
}
void sensing();
void loop() {
  sendMessage = ' ';
  if (mySerial.available()) {
    sensing();
    Serial.print(out = (float) mySerial.parseInt() / (float) 100);
    Serial.print(",");
    Serial.println(in);
    if (out - in > 0 && out - in < 15) {
      sendMessage = 'C'; // 밖의 값이 더 크고 15 이상의 차이라면, 'C'(Close) 문자 보내기
      digitalWrite(13, LOW); // 추후에 모터로 수정
    }
    else if (in - out > 0 && in - out < 15) {
      sendMessage = 'O'; // 안의 값이 더 크고 15 이상의 차이라면, 'O'(Open) 문자 보내기
      digitalWrite(13, HIGH); // 추후에 모터로 수정
    }
  }
}
//Qoskld;fj
void sensing() {
  int err = PMsensorErrSuccess;
  if ((err = PM.read(&in, true, sensitivity)) != PMsensorErrSuccess) {
    Serial.print("data Error = ");
    Serial.println(err);
    delay(1500);
    return;
  }
}

void sendToMaster() {
  if (sendMessage == ' ') Wire.write('W'); // 만약에 바뀐 사항이 없다면, 'W'(Wait) 문자 보내기
  else Wire.write(sendMessage);
}
