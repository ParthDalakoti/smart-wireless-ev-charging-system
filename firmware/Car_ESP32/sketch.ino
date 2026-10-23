
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

#define VOLT1_PIN 34
#define VOLT2_PIN 35
#define TEMP_PIN 33
#define BUZZER_PIN 25
#define FRONT_LIGHT_PIN 4
#define BACK_LIGHT_PIN 2

BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27,16,2);

float divider1=2.0, divider2=2.0;
float adcRef=3.3;
float battMin=6.0, battMax=8.4;
float alignMin=4.0;
float tempLimit=45.0;

float readVoltage(int pin,float ratio){
  return (analogRead(pin)/4095.0)*adcRef*ratio;
}

void setup(){
  Serial.begin(115200);
  SerialBT.begin("EV_PROTOTYPE_CAR");
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(FRONT_LIGHT_PIN,OUTPUT);
  pinMode(BACK_LIGHT_PIN,OUTPUT);
  lcd.init(); lcd.backlight();
  lcd.print("EV Car Ready");
  lcd.setCursor(0,1); lcd.print("BT: EV_PROTOTYPE");
  delay(1200); lcd.clear();
}

void loop(){
  float rx=readVoltage(VOLT1_PIN,divider1);
  float batt=readVoltage(VOLT2_PIN,divider2);
  float temp=(analogRead(TEMP_PIN)/4095.0)*adcRef*100.0;

  bool aligned = rx >= alignMin;
  int pct=(int)(((batt-battMin)/(battMax-battMin))*100.0);
  pct=constrain(pct,0,100);

  if(temp > tempLimit){
    digitalWrite(BUZZER_PIN,HIGH);
    lcd.clear(); lcd.print("OVERHEAT!");
    lcd.setCursor(0,1); lcd.print("Charging Halted");
    delay(250); digitalWrite(BUZZER_PIN,LOW);
  } else if(aligned){
    lcd.setCursor(0,0); lcd.print("Charging: ON    ");
    lcd.setCursor(0,1); lcd.print("Batt: "); lcd.print(pct); lcd.print("%     ");
    if(pct>=100){
      for(int i=0;i<3;i++){digitalWrite(BUZZER_PIN,HIGH);delay(120);
        digitalWrite(BUZZER_PIN,LOW);delay(80);}
      lcd.clear(); lcd.print("Fully Charged!");
      lcd.setCursor(0,1); lcd.print("Remove Car Now");
    }
  } else {
    lcd.setCursor(0,0); lcd.print("Charging: OFF   ");
    lcd.setCursor(0,1); lcd.print("Adjust Position ");
  }

  if(SerialBT.available()){
    char c=SerialBT.read();
    if(c=='W') digitalWrite(FRONT_LIGHT_PIN,HIGH);
    if(c=='w') digitalWrite(FRONT_LIGHT_PIN,LOW);
    if(c=='U') digitalWrite(BACK_LIGHT_PIN,HIGH);
    if(c=='u') digitalWrite(BACK_LIGHT_PIN,LOW);
  }
  delay(250);
}
