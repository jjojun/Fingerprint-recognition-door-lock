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
  Serial.begin(9600); //PC-�Ƶ��̳밣 ���
  finger.begin(57600); //�Ƶ��̳�-FPM10A ��Ŷ���
  pinMode(relay,OUTPUT);
  pinMode(door,INPUT_PULLUP); //���� ��������(LOW), ���� ��������(HIGH)
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
  //���� ����ä�� 10���̻� ����ϸ� �ڵ����� ���� ��װڴ�!
  //�����ġ�� Ǯ���ִ� ����
  if(digitalRead(relay) == HIGH && digitalRead(door) == LOW){
    //�����ġ�� Ǯ�������鼭 ���� �����ִ»���
    //10�ʰ� ī��Ʈ!
    if(millis() - t > 10000){
      //10�� ����� ���
      Serial.println("���� �����ִ� ���°� 10�ʰ� �����Ǿ���!");
      //����� �ٽ� �����Ѵ�
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
    Serial.print("�νĵ� ������ ID = ");
    Serial.println(id);
    if(id == 1){
      //�ݰ��� ������ �ִ� ����
      Serial.println("�ݰ��� ����� �����մϴ�!");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("GG UNLOCK!");
      lcd.setCursor(0,1);
      lcd.print("NOCKANDA!!");

      //�����ġ�� �����Ѵ�
      digitalWrite(relay,HIGH);

      //�ð��� ����
      t = millis();
      Serial.print("����� ������ �ð�");
      Serial.println(t);

      //������ ���� ����Ѵ�!
      //���ֵ̼�
      tone(buzzer,2093); //��
      delay(300); 
      tone(buzzer,1319); //��
      delay(300);
      tone(buzzer,1568); //��
      delay(300);
      tone(buzzer,2093); //��
      delay(300);
      noTone(buzzer);
    }else{
      //������ ���� ����
      Serial.println("������ ���� �����Դϴ�!");

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("YOUR FINGER PRINT");
      lcd.setCursor(0,1);
      lcd.print("IS NOT AVAIALBLE!");
      
      //������ ���� ����Ѵ�!
      //��-��-
      tone(buzzer,1568); //��
      delay(300);
      tone(buzzer,1568); //��
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
