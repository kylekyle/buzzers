// https://github.com/rweather/arduinolibs
#include <LCD.h>
#include <SoftwareSerial.h>

LCD lcd;
SoftwareSerial XBee(2, 3);

enum States {
  Reading, Waiting, Judging, Test
};

States STATE = Reading;

byte ON_MASK  = B10000000;
byte OFF_MASK = B01000000;

byte ALL_ON   = B11111111;
byte ALL_OFF  = B00000000;

void updateDisplay(String line1, String line2 = "") {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}

void setup() {  
  XBee.begin(9600);
  XBee.write(ALL_OFF);
  
  lcd.begin(16, 2);
  updateDisplay(" IT105 Jeopardy ","<Test     Start>");
}

void loop() {
  if (STATE == Reading) {
    while (XBee.available()) {
      XBee.read();
    }
    
    switch(lcd.getButton()) {
      case LCD_BUTTON_LEFT:
        STATE = Test;
        break;
      case LCD_BUTTON_SELECT:
        STATE = Waiting;
        XBee.write(ALL_ON);
        updateDisplay("   Waiting ...  ","<Cancel");
        break;
    }
  } 
  
  else if (STATE == Waiting) {
    if (XBee.available()) {
      STATE = Judging;
      XBee.write(ALL_OFF);
      
      byte buzzer = XBee.read();
      char buzzChar = buzzer + '0';
      
      XBee.write(buzzer | ON_MASK);
      
      updateDisplay(
        String("    ") + buzzChar + " Buzzed    ", 
        "<Wrong    Right>"
      );
    }

    if (lcd.getButton() == LCD_BUTTON_LEFT) {
      STATE = Reading;
      XBee.write(ALL_OFF);
      updateDisplay(" IT105 Jeopardy ","<Test     Start>");
    }
  } 
  
  else if (STATE == Judging) {
    while (XBee.available()) {
      XBee.read();
    }
    
    switch(lcd.getButton()) {
      case LCD_BUTTON_LEFT:
        STATE = Waiting;
        XBee.write(ALL_ON);
        updateDisplay("   Waiting ...  ");
        break;
      case LCD_BUTTON_SELECT:
        STATE = Reading;
        XBee.write(ALL_OFF);
        updateDisplay(" IT105 Jeopardy ","<Test     Start>");
        break;
    }
  }

  else if (STATE == Test) {
    updateDisplay("   Testing ...  ");

    for (int i=0; i<5; i++) {
      XBee.write(ALL_ON);
      delay(500);
      XBee.write(ALL_OFF);
      delay(500);
    }

    STATE = Reading;
    updateDisplay(" IT105 Jeopardy ","<Test     Start>");
  }
}
