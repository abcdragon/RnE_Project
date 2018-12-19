
#include <SoftwareSerial.h>
#include <Servo.h>
#define RAIN_DIGIT_PIN 4
#define SERVO_PIN 5
#define RAIN_ANALOG_PIN A5
#define RAIN_LED 13 // BLUE LED
#define DUST_LED 12 // RED LED

SoftwareSerial mySerial(10, 11); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정
Servo win;

void setup() {
  win.attach(SERVO_PIN);
  win.write(0);

  pinMode(RAIN_LED, OUTPUT);
  pinMode(DUST_LED, OUTPUT);
  
  Serial.begin(9600);
  mySerial.begin(9600);
}

bool isRain = false;

void loop() {
  if (mySerial.available()) {
    isRain = !(digitalRead(RAIN_DIGIT_PIN));
    char ss = mySerial.read();
    Serial.println(ss);
    
    if(isRain){ // 비가 오는 경우 // 나중에 rainChk 함수로 대체
      Serial.println('C');
      digitalWrite(RAIN_LED, HIGH); // 따라서 비 LED를 켠다.
      for(int i = 90; i >= 0; i--) // 그리고 창문도 닫는다.
        win.write(i); // 왼쪽
    } 
    
    else { // 비가 오지 않는 경우
      digitalWrite(RAIN_LED, LOW); // 따라서 비 LED를 끈다.
      if (ss == 'o' || ss == 'O') { // 안의 값이 밖보다 크면 창문 열기
        digitalWrite(DUST_LED, LOW); // 실내가 외부보다 공기가 좋지 않으므로 끈다. 
        Serial.println('O');
        for(int i = 0; i < 90; i++) // 외부가 실내보다 공기가 좋으므로 연다.
          win.write(i); // 왼쪽
      }
      else if (ss == 'c' || ss == 'C') { // 밖의 값이 안보다 크면 창문 닫기
        digitalWrite(DUST_LED, HIGH); // 외부가 실내보다 공기가 좋지 않으므로 켠다.
        Serial.println('C');
        for(int i = 90; i >= 0; i--) // 실내가 외부보다 공기가 좋으므로 닫는다.
          win.write(i); // 왼쪽
      }
      else
        Serial.println('W');
    }
  }
}
