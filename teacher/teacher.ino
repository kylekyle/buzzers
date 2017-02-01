#include <Wire.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial XBee(2, 3);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte ON_MASK = B10000000;
byte OFF_MASK = B01000000;

enum States {
  Reading, Waiting, Judging
};

States STATE = Reading;

void reading() {
  lcd.clear();
  lcd.print("-IT105 Jeopardy-");
  lcd.setCursor(0,1);
  lcd.print("<Test     Start>");

  STATE = Reading;
}

void judging(char buzzer) {
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print(buzzer);
  lcd.print(" BUZZED");
  lcd.setCursor(0,1);
  lcd.print("<Wrong    Right>");
  
  STATE = Judging;
}

void setup() {  
  XBee.begin(9600);
  lcd.begin(16, 2);
  reading();
}

void loop() {
  if (XBee.available()) {
    byte buzzer = XBee.read();
    
    if (STATE == Reading) {
      XBee.write(buzzer | ON_MASK);
      judging(buzzer + '0');
    }
  }
}
