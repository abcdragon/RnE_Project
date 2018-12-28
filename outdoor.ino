#define CDS A0
#define MA_F 8
#define MA_B 9
#define MB_F 5
#define MB_B 6

float W = 0.01;
float mean = 0.0f;
bool FLAG = false;
unsigned long prev_time = 0, current_time = 0;

void spin(){
  analogWrite(MA_F, 0);
  analogWrite(MA_B, 0);
  analogWrite(MB_F, 255);
  analogWrite(MB_B, 0);  
}

void stop(){
  analogWrite(MA_F, 0);
  analogWrite(MA_B, 255);
  analogWrite(MB_F, 0);
  analogWrite(MB_B, 0);  
}

void setup() {
  pinMode(CDS, INPUT);
  
  pinMode(MA_F, OUTPUT);
  pinMode(MA_B, OUTPUT);
  pinMode(MB_F, OUTPUT);
  pinMode(MB_B, OUTPUT);

  stop();
  
  Serial.begin(9600);
}

void loop() {
  int cValue = analogRead(CDS); //Serial.println(cValue);
  if(FLAG && millis() - prev_time > 3000) { // millis 를 이용해서 3초간 모터 돌리기
    FLAG = false;
    stop();
    Serial.println(millis());
    Serial.println(prev_time);
  }
  if(!FLAG && (cValue > 750)){
    Serial.print("Opened door");
    FLAG = true;
    mean = analogRead(CDS);
    do{
      Serial.println(mean);  
    }
    while((mean = analogRead(CDS) * W + mean * (1 - W)) >= 660);
    mean = 0.0f;
    delay(1000);
    spin();
    prev_time = millis();
  }
}
