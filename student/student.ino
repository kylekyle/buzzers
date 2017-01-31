#include <SoftwareSerial.h>

SoftwareSerial XBee(2, 3);

byte ON, OFF, ATMY = 0;
byte ON_MASK = B10000000;
byte OFF_MASK = B01000000;

int BUTTON_PIN = 12;
int BUTTON_STATE = LOW;

void setup() {
  XBee.begin(9600);
  Serial.begin(9600);
  
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (ATMY == 0) {
    // switch to AT mode
    XBee.print("+++");
    delay(100);
    
    // receive "OK"
    XBee.readString();
    
    // read ATMY
    XBee.print("ATMY\r");
    delay(100);
    
    ATMY = XBee.read() - '0';
    ON = ATMY | ON_MASK;
    OFF = ATMY | OFF_MASK;
    
    Serial.print("ATMY: ");
    Serial.println(ATMY);

    Serial.print("ON: ");
    Serial.println(ON);

    Serial.print("OFF: ");
    Serial.println(OFF);
  } else {
    if (digitalRead(BUTTON_PIN) == HIGH) {
      XBee.write(ATMY);
      Serial.print("Sending ");
      Serial.println(ATMY);
    }
    
    if (XBee.available()) { 
      byte message = XBee.read();
      Serial.print("Received message: ");
      Serial.println(message);
      
      if (message == ON) {
        Serial.println("Turning LED on");
        digitalWrite(LED_BUILTIN,HIGH);
      } else if (message == OFF) {      
        Serial.println("Turning LED off");
        digitalWrite(LED_BUILTIN,LOW);
      }
    }
  }
}
