#include <SoftwareSerial.h>

SoftwareSerial XBee(5,4); // RX,TX

byte ON, OFF, ATMY = 0;

byte ON_MASK  = B10000000;
byte OFF_MASK = B01000000;

byte ALL_ON   = B11111111;
byte ALL_OFF  = B00000000;

int LED = 10;
int BUTTON = 9;
unsigned long SLEEP_UNTIL = 0;

void setup() {
  XBee.begin(9600);
  Serial.begin(9600);

  pinMode(BUTTON, INPUT);
  pinMode(LED, OUTPUT);
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
    if (digitalRead(BUTTON) == HIGH) {
      if (millis() > SLEEP_UNTIL) {
        XBee.write(ATMY);
        Serial.print("Sending ");
        Serial.println(ATMY);
      }

      SLEEP_UNTIL = millis() + 1000;
    }
    
    if (XBee.available()) { 
      byte message = XBee.read();
      Serial.print("Received message: ");
      Serial.println(message);
      
      if (message == ON || message == ALL_ON) {
        Serial.println("Turning LED on");
        digitalWrite(LED,HIGH);
      } else if (message == OFF || message == ALL_OFF) {
        Serial.println("Turning LED off");
        digitalWrite(LED,LOW);
      }
    }
  }
}
