//DC모터 핀 :2,3,40,41,42,43번
//36,37번핀 led
//미세먼지 핀 : A0,A1번  12,11번핀
//빗물센서 핀 A2번
//서보 모터 핀 : 8번
//내부 LED 1,2번 : 5,6,7번 33,34,35번
//내,외부 신호등 : 30,31,32번 /24,23,22번
//블루투스 RX,TX핀 : 0 ,1 번

//기본적인 미세먼지 동작원리

//미세먼지 센서는 센서에 빛이 얼만큼 도달하냐로 공기중의 먼지농도를 측정한다.
//적외선 LED에 LOW값을 주어서 적외선 LED를 켜고 0.28ms만큼 기다린다.
//그후에 analogRead를 통해 현재 A0의 값을 읽어들인다.
//analogRead를 통해 읽어들인후 40us만큼 기다린다.즉 0.04ms만큼 기다린다.
//적외선 led를 끈후 0.968ms만큼 기다린다. 

//적용시킨 조건

//기본적으로 내부먼지 농도가 일정치 이상이고 외부값보다 높을때 창문이 open되고 환풍기가 작동.
//내부농도가 일정치 이상이지만 외부값이 내부값보다 높게 측정되면 창문 close
//외부에 빗방울이 감지되어 빗물센서에 감지되면 close
//앱으로 특정값을 설정시 센서값에 영향을 받지않고 수행

//led 상황
//초록:안전(90us/m3 이하) , 주황:경고(90<센서값<=300us/m3),빨강:위험(300us/m3이상)
//36,37

#include<Servo.h>
#include<SoftwareSerial.h>

#define OUT_LED_R 23
#define OUT_LED_Y 24
#define OUT_LED_G 22

#define IN_LED_R 5
#define IN_LED_Y 6
#define IN_LED_G 7

SoftwareSerial BTSerial(0,1);
Servo myservo;
int pos=0;//모터 각도 변수

int Color=0;//내부 led상태 0일경우 기본값 ,1일경우 앱으로 설정된값

// 내부 팬 모터
const int motA_IA=2;//모터 핀
const int motA_IB=3;

/*
const int motB_IA=40;//모터 핀
const int motB_IB=41;
const int motC_IA=42;//모터 핀
const int motC_IB=43;
*/

int dustPin=A0;//적외선 수신부가 연결된 핀(센서1)
float dustVal=0;
float dustDensityug=0;//내부 미세먼지 ug/m3
float voMeasured=0;
float calcVoltage=0;

int dustPin2=A1;//전외선 수신부가 연결된 핀(센서2)
float dustVal2=0;
float dustDensityug2=0;//외부 미세먼지 ug/m3
float voMeasured2=0;
float calcVoltage2=0;

int ledPower=12;//먼지센서 LED핀1
int ledPower2=11;//먼지센서 LED핀2
int delayTime=280;
int delayTime2=40;
float offTime=9600;

int rainPin = A2;//빗물센서 핀delay(100);
int rain=0;//빗물 상태

int flagDoorOpen=0;//창문 상태
int remoteDoor=0;//창문 앱 상태
int Fan=0;//팬 상태

void setup(){
  myservo.attach(8);//모터
  myservo.write(0);
  Serial.begin(9600);//시리얼 데이터 속도 9600
 
  pinMode(ledPower, OUTPUT);//미세먼지 적외선LED1
  pinMode(ledPower2, OUTPUT);//미세먼지 적외선LED2
  
  pinMode(30, OUTPUT); // 내부 led1 
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);

  pinMode(35,OUTPUT);//내부 led2
  pinMode(34,OUTPUT);
  pinMode(33,OUTPUT);

  pinMode(IN_LED_R, OUTPUT);// 내부 신호등
  pinMode(IN_LED_Y, OUTPUT);
  pinMode(IN_LED_G, OUTPUT);
    
  pinMode(OUT_LED_R, OUTPUT);//외부 신호등
  pinMode(OUT_LED_Y, OUTPUT);
  pinMode(OUT_LED_G, OUTPUT);

  
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  
  pinMode(4, OUTPUT);
  
  pinMode(motA_IA,OUTPUT);
  pinMode(motA_IB,OUTPUT);
  /*pinMode(motB_IA,OUTPUT);
  pinMode(motB_IB,OUTPUT);
  pinMode(motC_IA,OUTPUT);
  pinMode(motC_IB,OUTPUT);*/
}

void loop(){   //센서에 빛이 얼마나 들어 오는지로 미세먼지 농도 확인
  digitalWrite(ledPower, LOW);//적외선led에 low값을 주어서 적외선 led를 켠다
  delayMicroseconds(delayTime);//마이크로 단위로 딜레이,280us만큼 기다린다. 즉 0.28ms만큼 기다린다
  
  dustVal=analogRead(dustPin);//그후에 analogRead를 통해 현재 A0의 값을 읽어들인다.
  calcVoltage = dustVal*(5.0/1024);//전압값 조정
  
  delayMicroseconds(delayTime2);//analogRead를 통해 읽어들인후 40us만큼 기다린다.즉 0.04ms만큼 기다린다.
  
  digitalWrite(ledPower, HIGH);//적외선 LED를 끈다.
  delayMicroseconds(delayTime2);
  
  digitalWrite(ledPower, HIGH);//그리고  0.968ms만큼 기다린다. 
  delayMicroseconds(offTime);
  
  delay(500);// dust 1 딜레이

  digitalWrite(ledPower2, LOW);
  delayMicroseconds(delayTime);
  
  dustVal2=analogRead(dustPin2);
  calcVoltage2 = dustVal2*(5.0/1024);
  
  delayMicroseconds(delayTime2);
  
  digitalWrite(ledPower2, HIGH);
  delayMicroseconds(delayTime2);
  
  digitalWrite(ledPower2, HIGH);
  delayMicroseconds(offTime);
  
  delay(500);//dust 2 딜레이
  
  dustDensityug=(0.17*calcVoltage -0.1)*1000; //먼지센서 1값 도출식
    Serial.print("Dust Density [ug/m3] : ");
    Serial.println(dustDensityug);
  
  dustDensityug2=(0.17*calcVoltage2 -0.1)*1000; //먼지센서 2값 도출식
    Serial.print("Dust Density2 [ug/m3] : ");
    Serial.println(dustDensityug2);

  int readingrain=analogRead(rainPin);//빗물센서 시리얼 모니터로 통신,빗물센서는 물이 많이 묻어 있을수록 센서값이낮아짐
    Serial.print("빗물센서");
    Serial.println(readingrain);
  
  //빗물 센서를 80이하일때 rain변수를 1로 설정하고 이상일때는 0으로 설정
  if(readingrain<80) rain=1;
  else rain=0;

  if(dustDensityug>150){ //먼지센서 1이 150이상일경우
    if(dustDensityug>300 ){//먼지센서 1이 300이상인경우
      if(flagDoorOpen==0 && dustDensityug>dustDensityug2){//문상태가 닫혀있고 내부 센서값이 외부센서값보다 높을때
        if(remoteDoor==0 && rain==0){ // 앱으로 창문 조정하지 않은상태이며 비가 오지 않은 상태
          for(pos=0; pos<90; pos++){//모터 회전 (창 열림)
            myservo.write(pos);
            delay(15);
          }
          flagDoorOpen=1;//문이 열린상태로 표시
        }
        digitalWrite(IN_LED_R,HIGH);//빨간불 표시(내부 공기가 너무 탁함)
        digitalWrite(IN_LED_Y,LOW);
        digitalWrite(IN_LED_G,LOW);
              
        if(Fan==0){//앱으로 팬을 돌리지 않은경우
          DC();
          delay(200); 
        }
      }
      else {
        flagDoorOpen=1;//문이 열린상태로 표시,이미 문이 열린상태이므로 팬만 작동
        if(Fan==0) DC();
      } 
    }
    else{
      digitalWrite(IN_LED_R,LOW);//노란불 표시(내부 공기가 탁함)
      digitalWrite(IN_LED_Y,HIGH);
      digitalWrite(IN_LED_G,LOW);
      delay(500);
    }
  } 
  else{
    digitalWrite(IN_LED_R,LOW);//초록불 표시 (내부 공기 좋음)
    digitalWrite(IN_LED_Y,LOW);
    digitalWrite(IN_LED_G,HIGH);
    if(remoteDoor==0){//내부 공기가 좋으므로 별도로 앱을 설정하지 않은경우 닫는다.
        if(flagDoorOpen == 1){
          for(pos=90; pos>=0;pos--){
            myservo.write(pos);
            delay(15);
          }
        }  
        flagDoorOpen=0;
    }
  }

  if(dustDensityug2>150){
    if(dustDensityug2>300 && dustDensityug2>dustDensityug){ // 외부 미세먼지가 300 보다 크고 내부보다 클 때
      digitalWrite(OUT_LED_R,HIGH); // 빨간 불 상태....
      digitalWrite(OUT_LED_Y,LOW);
      digitalWrite(OUT_LED_G,LOW);
      if(flagDoorOpen==1) { // 이거는 아두이노 내에서 문이 열렸는가 체크, 1이니까 열린 상태
        //if(remoteDoor==0){ // 앱에서 별도로 설정 안했을 경우 문을 닫는다.
          if(flagDoorOpen == 1){
            for(pos=90; pos>=0;pos--){
              myservo.write(pos);
              delay(15);
            }
          }
        //} // if(remoteDoor == 0) 종료
        flagDoorOpen=0; // 닫힌 상태로 바꾸기
      } // if(flagDoorOpen == 1) 종료
      else flagDoorOpen=0; // ?
    } // 300보다 크고 내부보다 큰거 체크하는 if문 종료
    
    else{ // 그냥 150 이상일 경우는 노란불
      digitalWrite(OUT_LED_R,LOW);
      digitalWrite(OUT_LED_Y,HIGH);
      digitalWrite(OUT_LED_G,LOW);
    }
  }
  
  else{ // 정상이면 초록불
    digitalWrite(OUT_LED_R,LOW);
    digitalWrite(OUT_LED_Y,LOW);
    digitalWrite(OUT_LED_G,HIGH);
  }

  if(Color==0){//내부 led1 기본 상태
    digitalWrite(31, LOW);
    digitalWrite(32, LOW);
    digitalWrite(30, LOW);

    digitalWrite(33, LOW);//내부 led2 상태, led는 1번만 색상 변경가능하게 설정해두었음
    digitalWrite(34, LOW);
    digitalWrite(35, LOW);
  }
  
  //300이상이면 led 노란불 점등
  if(dustDensityug2>300) digitalWrite(36, LOW);//켜짐
  else digitalWrite(36,HIGH);//꺼짐

  //빗물센서에 빗물이 감지되면 빨간불 점등
  if(rain==1) {
    digitalWrite(37, LOW);//켜짐
    if(flagDoorOpen==1) { // 이거는 아두이노 내에서 문이 열렸는가 체크, 1이니까 열린 상태
        //if(remoteDoor==0){ // 앱에서 별도로 설정 안했을 경우 문을 닫는다.
          if(flagDoorOpen == 1){
            for(pos=90; pos>=0;pos--){
              myservo.write(pos);
              delay(15);
            }
          }
        //} // if(remoteDoor == 0) 종료
        flagDoorOpen=0; // 닫힌 상태로 바꾸기
      } // if(flagDoorOpen == 1) 종료
  }
  else {
    digitalWrite(37,HIGH);//꺼짐
  }

  // 외부먼지센서가 300이상이면 계속 켜져있다가 300이하로 내려가면 꺼짐(모터2,3번)
  //if(dustDensityug2>300) DC2();
  //else DC3();

  if(Serial.available()){ // 블루투스와 데이터를 주고 받으면서 명령 전달
    char BT;// bt 1번 버튼 .LED설정
    BT=Serial.read();
    //블루투스로 t가 전송되면 Red() 함수값 불러와서 실행,아래는 전부 같은 방식
    if(BT=='t') Red(); 
    if(BT=='b') Blue();
    if(BT=='g') Green();
    
    //Clear함수 실행시 led가 기본 상태로 돌아가게 된다
    if(BT=='c') Clear();
    
    //led를 꺼버린다
    if(BT=='o') LEDOFF();
    
    //팬을 앱으로 직접 돌릴시에 내부의 센서값에 상관없이 계속 돌게 된다.FanClear를 실행하지 않으면 센서값에 영향을 받지 않는다.Fan변수가 1로 설정되있기때문
    if(BT=='q') Fanon();
    if(BT=='w') Fanoff();
    
    //Fan변수를 0으로 돌려놓는다.센서값에 영향을 다시 받게된다.
    if(BT=='e') FanClear();
    
    //창문을 앱으로 열게 되면 센서값에 상관없이 열고 닫기를 할수있다.remoteDoor변수가 1로 넘어가서 센서값에 영향을 받지 않게된다/
    if(BT=='y') WindowOpen();
    if(BT=='u') WindowClose();
    if(BT=='i') WindowClear();//remote변수값을 0으로 되돌려 놓아 센서값에 영향을 받게 된다.
    Serial.println(BT);
  }
  delay(500);
}

void DC(){
  analogWrite(motA_IB,255);
  delay(5000);
  analogWrite(motA_IB,0);
}
/*
void DC2(){ 
  analogWrite(motB_IB,255);
  analogWrite(motC_IA,255);
}

void DC3(){
  analogWrite(motC_IA,0);
  analogWrite(motB_IB,0);
}*/

void Red(){
  digitalWrite(30,LOW);
  digitalWrite(31,HIGH);
  digitalWrite(32,HIGH);
  Color=1;
}

void Green(){
  digitalWrite(30,HIGH);
  digitalWrite(31,LOW);
  digitalWrite(32,HIGH);
  Color=1;
}

void Blue(){
  digitalWrite(30,HIGH);
  digitalWrite(31,HIGH);
  digitalWrite(32,LOW);
  Color=1;
}

void LEDOFF(){
  digitalWrite(30,HIGH);
  digitalWrite(31,HIGH);
  digitalWrite(32,HIGH);
  Color=1;
}

void Clear(){
  //앱사용으로 인해 Color변수값이 1 이였던것을 0으로 설정---기본 led색상으로 변경
  Color=0;
}

void Fanon(){
  //팬을 돌리게 하는 함수.앱으로 실행해서 사용시 Fan변수가 1이 되므로 센서값에 영향을 받지 않고 작동함
  Fan=1;
  analogWrite(motA_IB,255);
}

void Fanoff(){
  //팬을 정지 시키는 함수.실행시 Fan변수가 1이되므로 센서값에 영향을 받지 않고 꺼진 상태 유지
  Fan=1;
  analogWrite(motA_IB,0);
}

void FanClear(){
  //Fan변수를 0으로 되돌려서 센서값에 영향을 받으며 작동
  Fan=0;
  analogWrite(motA_IB,0);
}

void WindowOpen(){
  remoteDoor=1;
  if(pos<10){//모터각도가 10 이하일때 모터 회전
    for(pos=0; pos<=90; pos++){
      myservo.write(pos);
      delay(15);
    }
    flagDoorOpen=1;
  }
}

void WindowClose(){
  remoteDoor=1;
  if(pos>0){//모터 각도가0이상일때 모터 회전(즉 창문이 열려있으면 창문 닫는 함수)
    for(pos=90; pos>=0;pos--){
      myservo.write(pos);
      delay(15); 
    }
    flagDoorOpen=0;
  }
}

void WindowClear(){
  remoteDoor=0;
  if(flagDoorOpen==1){
    for(pos=90; pos>=0; pos--){
      myservo.write(pos);
      delay(15);  
    }
    flagDoorOpen=0;
  }
}
