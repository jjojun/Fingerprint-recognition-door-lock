#include <Adafruit_Fingerprint.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define relay 4
#define buzzer 5
#define door 6

SoftwareSerial mySerial(2, 3);
LiquidCrystal_I2C lcd(0x27,16,2); //1602
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

unsigned long t = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //PC-아두이노간 통신
  finger.begin(57600); //아두이노-FPM10A 통신라인
  pinMode(relay,OUTPUT);
  pinMode(door,INPUT_PULLUP); //문이 닫혔을때(LOW), 문이 열렸을때(HIGH)
  lcd.init();
  lcd.backlight();

  //1602
  //0~15/0~1
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");
  lcd.setCursor(0,1);
  lcd.print("nockanda!");
}

void loop() {
  // put your main code here, to run repeatedly:
  //문이 열린채로 10초이상 경과하면 자동으로 문을 잠그겠다!
  //잠금장치가 풀려있는 상태
  if(digitalRead(relay) == HIGH && digitalRead(door) == LOW){
    //잠금장치가 풀려있으면서 문이 닫혀있는상태
    //10초간 카운트!
    if(millis() - t > 10000){
      //10초 경과한 경우
      Serial.println("문이 닫혀있는 상태가 10초가 유지되었다!");
      //잠금을 다시 설정한다
      digitalWrite(relay,LOW);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("GG LOCK!");
      lcd.setCursor(0,1);
      lcd.print("NOCKANDA!!");
    }
  }else{
    t = millis();
  }
  
  int id = getFingerprintIDez();
  if(id != -1){
    Serial.print("인식된 지문의 ID = ");
    Serial.println(id);
    if(id == 1){
      //금고의 권한이 있는 지문
      Serial.println("금고의 잠금을 해제합니다!");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("GG UNLOCK!");
      lcd.setCursor(0,1);
      lcd.print("NOCKANDA!!");

      //잠금장치를 해제한다
      digitalWrite(relay,HIGH);

      //시간을 측정
      t = millis();
      Serial.print("잠금이 해제된 시간");
      Serial.println(t);

      //부저의 음을 재생한다!
      //도미솔도
      tone(buzzer,2093); //도
      delay(300); 
      tone(buzzer,1319); //미
      delay(300);
      tone(buzzer,1568); //솔
      delay(300);
      tone(buzzer,2093); //도
      delay(300);
      noTone(buzzer);
    }else{
      //권한이 없는 지문
      Serial.println("권한이 없는 지문입니다!");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("YOUR FINGER PRINT");
      lcd.setCursor(0,1);
      lcd.print("IS NOT AVAIALBLE!");
      
      //부저의 음을 재생한다!
      //솔-솔-
      tone(buzzer,1568); //솔
      delay(300);
      tone(buzzer,1568); //솔
      delay(300);
      noTone(buzzer);
    }
  }
}


int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
