#include <SoftwareSerial.h>
#include <Servo.h>
// https://m.blog.naver.com/twophase/221139319142
#define sensitivity 0.1

#define RAIN_DIGIT_PIN 4
#define SERVO_PIN 5
#define RAIN_ANALOG_PIN A5
#define RAIN_LED 13 // BLUE LED
#define DUST_LED 12 // RED LED
#define DUST_LED_PIN 2 // 먼지 센서 IR LED 핀
#define DUST_ANALOG_PIN A1 // 먼지 센서 받는 핀

SoftwareSerial mySerial(10, 11); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정
Servo win;

bool isRain = false;

void setup() {
  win.attach(SERVO_PIN);
  win.write(0);

  pinMode(RAIN_LED, OUTPUT);
  pinMode(DUST_LED, OUTPUT);

  pinMode(DUST_ANALOG_PIN, INPUT);
  pinMode(DUST_LED_PIN, OUTPUT);
  
  Serial.begin(9600);
  mySerial.begin(9600);
}

float sensing(){
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(280);
  float dust = analogRead(DUST_ANALOG_PIN);
  delayMicroseconds(40);
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(9680);
  return dust;
}

void loop() {
  float calcVolt = sensing() * (5.0 / 1024);
  Serial.println((0.17 * calcVolt - 0.1) * 1000);
  delay(350);
  if (mySerial.available()) {
    float out = mySerial.parseFloat();
    Serial.println(ss);
    isRain = !(digitalRead(RAIN_DIGIT_PIN));
    
    if(isRain){ // 비가 오는 경우 // 나중에 rainChk 함수로 대체
      Serial.println('C');
      digitalWrite(RAIN_LED, HIGH); // 따라서 비 LED를 켠다.
      for(int i = 90; i >= 0; i--) // 그리고 창문도 닫는다.
        win.write(i); // 왼쪽
    }
    
    else { // 비가 오지 않는 경우
      digitalWrite(RAIN_LED, LOW); // 따라서 비 LED를 끈다.
      if (in >= 300) { // 안의 값이 밖보다 크면 창문 열기
        digitalWrite(DUST_LED, LOW); // 실내가 외부보다 공기가 좋지 않으므로 끈다. 
        Serial.println('O');
        for(int i = 0; i < 90; i++) {// 외부가 실내보다 공기가 좋으므로 연다.
          win.write(i); // 왼쪽
          delay(15);
        }
      }
      else if (out >= 300) { // 밖의 값이 안보다 크면 창문 닫기
        digitalWrite(DUST_LED, HIGH); // 외부가 실내보다 공기가 좋지 않으므로 켠다.
        Serial.println('C');
        for(int i = 90; i >= 0; i--){ // 실내가 외부보다 공기가 좋으므로 닫는다.
          win.write(i); // 왼쪽
          delay(15);
        }
      }
      else
        Serial.println('W');
    }
  }
}




/*외부
  룸 O
  먼지 보내기 O
  
  내부
  창문 열기 O
  먼지 받기 / 측정하기 ==> 이에따른 동작 확인 안함, 그리고 조건문 수정바람
  엘베 LED X
  
  라즈베리파이 // 집에서
  핸드폰 통신 X
  시리얼 보내기 X
*/
