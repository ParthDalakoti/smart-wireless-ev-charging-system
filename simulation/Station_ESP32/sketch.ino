
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 5
#define RST_PIN 4
#define IR1_PIN 14
#define IR2_PIN 27
#define GAS_PIN 34
#define FLAME_PIN 35
#define RELAY_PIN 26
#define BUZZER_PIN 25
#define SERVO_PIN 13

MFRC522 rfid(SS_PIN, RST_PIN);
Servo gateServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int GAS_THRESHOLD = 1800;
const int FLAME_THRESHOLD = 1800;

void gateOpen(){ gateServo.write(90); }
void gateClose(){ gateServo.write(0); }

void alarm(const char* msg){
  digitalWrite(RELAY_PIN, LOW);
  lcd.clear(); lcd.setCursor(0,0); lcd.print(msg);
  digitalWrite(BUZZER_PIN,HIGH); delay(500);
  digitalWrite(BUZZER_PIN,LOW);
}

void setup(){
  Serial.begin(115200);
  SPI.begin(); rfid.PCD_Init();
  pinMode(IR1_PIN, INPUT_PULLUP);
  pinMode(IR2_PIN, INPUT_PULLUP);
  pinMode(GAS_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  gateServo.attach(SERVO_PIN);
  gateClose(); digitalWrite(RELAY_PIN,LOW);
  lcd.init(); lcd.backlight();
  lcd.print("EV Station Ready");
}

void loop(){
  int gas = analogRead(GAS_PIN);
  int flame = analogRead(FLAME_PIN);

  if(gas > GAS_THRESHOLD){
    alarm("GAS ALERT!");
    lcd.setCursor(0,1); lcd.print("Charging OFF");
    delay(1200); return;
  }

  if(flame > FLAME_THRESHOLD){
    alarm("FIRE ALERT!");
    lcd.setCursor(0,1); lcd.print("Charging OFF");
    delay(1200); return;
  }

  lcd.setCursor(0,0); lcd.print("Waiting for Car ");
  lcd.setCursor(0,1); lcd.print("Press ENTRY IR  ");

  if(digitalRead(IR1_PIN)==LOW){
    gateOpen();
    lcd.clear(); lcd.print("Scan RFID Card");
    delay(1000);

    // Prototype shortcut: any RFID card read is treated as authorized.
    if(rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()){
      lcd.clear(); lcd.print("Access Granted");
      rfid.PICC_HaltA();
      delay(900);
      lcd.clear(); lcd.print("Drive Forward");
      while(digitalRead(IR2_PIN)==HIGH){
        if(analogRead(GAS_PIN)>GAS_THRESHOLD || analogRead(FLAME_PIN)>FLAME_THRESHOLD){
          gateClose(); alarm("SAFETY ALERT!");
          delay(1000); return;
        }
        delay(20);
      }
      delay(500); gateClose();
      lcd.clear(); lcd.print("Charging ON");
      digitalWrite(RELAY_PIN,HIGH);
      lcd.setCursor(0,1); lcd.print("IR2 = Exit");
      while(digitalRead(IR2_PIN)==LOW){
        if(analogRead(GAS_PIN)>GAS_THRESHOLD || analogRead(FLAME_PIN)>FLAME_THRESHOLD){
          alarm("SAFETY ALERT!");
          delay(1000); return;
        }
        delay(20);
      }
      digitalWrite(RELAY_PIN,LOW);
      lcd.clear(); lcd.print("Charging OFF");
      gateOpen(); delay(700); gateClose();
    } else {
      alarm("Access Denied");
      gateClose();
    }
  }
  delay(50);
}
