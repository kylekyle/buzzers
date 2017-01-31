#include <Wire.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial XBee(2, 3);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

bool READY = true;
byte ON_MASK = B10000000;
byte OFF_MASK = B01000000;

void setup() {  
  XBee.begin(9600);
  lcd.begin(16, 2);
  lcd.print("-IT105 Jeopardy-");
  lcd.setCursor(0,1);
}

void loop() {
  if (XBee.available()) {
    byte buzzer = XBee.read();
    
    if (READY) {
      XBee.write(buzzer | ON_MASK);
      
      lcd.setCursor(4,1);
      lcd.write(buzzer + '0');
      lcd.print(" BUZZED");      
    }
  }
}
