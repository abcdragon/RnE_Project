#include <Servo.h>
#include <Wire.h>
#define SLAVE 4

Servo win1, win2;
int rain_INPUT = A0, cds = A1;
int rain_DIGIT_INPUT = 13;
int rainVal, cdsVal;
boolean bIsRaining = false;
String isRain;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  win1.attach(9); win2.attach(10);
  win1.write(0); win2.write(180);
  pinMode(rain_DIGIT_INPUT,INPUT);
}
char i2c_communication(){
  Wire.requestFrom(SLAVE, 1); // 1바이트의 데이터 요청
  return Wire.read(); // 읽어들인 값을 char 형으로 반환
}
void loop() {
  rainVal = analogRead(rain_INPUT); // 출력용, 값이 높을 수록 물 X
  cdsVal = analogRead(cds);
  bIsRaining = !(digitalRead(rain_DIGIT_INPUT));
//Qoskld;fj
  
  char r = i2c_communication();
  Serial.print(r);
  if(bIsRaining){ // 비가 온다면 무조건 창문을 닫는다.
    isRain = "YES";
      for(int i = 180; i > 0; i--) {
        win1.write(i); // 왼쪽
        win2.write(180 - i); // 오른쪽
      }
  } else{
    isRain = "NO";
    if(r == 'O') // ==> Open
      for(int i = 0; i < 180; i++) {
        win1.write(i); // 왼쪽
        win2.write(180 - i); // 오른쪽
      }
    else if(r == 'C') // ==> Close
      for(int i = 180; i > 0; i--) {
        win1.write(i); // 왼쪽
        win2.write(180 - i); // 오른쪽
      }
    // r == 'W' // ==> Wait
  }
  Serial.print(',');
  Serial.print(isRain);  
  Serial.print(',');
  Serial.print(rainVal);
  Serial.print(',');
  Serial.print(cdsVal);
  Serial.write('\n');
  delay(125); // 0.125 초 딜레이
}
