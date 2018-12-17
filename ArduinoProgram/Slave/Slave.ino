#include <SoftwareSerial.h>
#include <PMsensor.h>
#include <Servo.h>
#define sensitivity 0.1

SoftwareSerial mySerial(8, 9); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정
PMsensor PM;

Servo win1, win2;

short sensorPin = A0, sensorLED = 3;
int rain_INPUT = 5, rain_DIGIT_INPUT = 13, cds = 6;
int rainVal, cdsVal;
boolean blsRaining = false;
String isRain, printStr = "";
float out = 0.0f, in = 0.0f;

void setup() {
  win1.attach(10); win2.attach(11);
  win1.write(0); win2.write(180);
  PM.init(sensorPin, sensorLED);
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(rain_DIGIT_INPUT, INPUT);
}
void sensing();
void servo(int s, int e, boolean FLAG);
void loop() {
  if (mySerial.available()) {
    rainVal = analogRead(rain_INPUT);
    cdsVal = analogRead(cds);
    blsRaining = !(digitalRead(rain_DIGIT_INPUT));
    sensing(); out = (float) mySerial.parseInt() / 100;
    printStr = String(out) + ',' + String(in) + ',' + String(isRain) + ',' + String(rainVal) + ',' + String(cdsVal) + '\n';
    Serial.print(printStr);
    if(blsRaining){
      isRain = "Yes";
      Serial.print("C,");
      servo(180, 0, false);
    } else {
      isRain = "NO";
      if (in - out > 0 && in - out >= 15) { // 안의 값이 밖보다 크면 창문 열기
        servo(0, 180, true);
        Serial.print("O,");
        digitalWrite(13, LOW); // 추후에 모터로 수정
      }
      else if (out - in > 0 && out - in >= 15) { // 밖의 값이 안보다 크면 창문 닫기
        servo(180, 0, false);
        Serial.print("C,");
        digitalWrite(13, HIGH); // 추후에 모터로 수정
      }
      else {
        Serial.print("W,");  
      }
    }
  }
}
void sensing() {
  int err = PMsensorErrSuccess;
  if ((err = PM.read(&in, true, sensitivity)) != PMsensorErrSuccess) {
    Serial.print("data Error = ");
    Serial.println(err);
    delay(1500);
    return;
  }
}
void servo(int s, int e, boolean FLAG){
  if(FLAG)
    for(int i = s; i < e; i++){
        win1.write(i); // 왼쪽
        win2.write(180 - i); // 오른쪽
    }
  else
    for(int i = s; i > e; i--){
      win1.write(i); // 왼쪽
      win2.write(180 - i); // 오른쪽  
    }
}
