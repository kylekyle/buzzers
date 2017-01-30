#include <SoftwareSerial.h>

SoftwareSerial XBee(2, 3); // RX, TX

int BUTTON = 12;
char ATMY = NULL;
int BUTTON_STATE = LOW;

void setup() {
  XBee.begin(9600);
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (ATMY == NULL) {

    // switch to AT mode
    XBee.print("+++");
    delay(1000);
    
    // receive "OK"
    XBee.readString();
    
    // read ATMY
    XBee.print("ATMY\r");
    ATMY = XBee.read();
    
    Serial.println("ATMY: " + ATMY);
  } else {
    int state = digitalRead(BUTTON);
    
    if (state != BUTTON_STATE) {
      BUTTON_STATE = state;

      if (state == HIGH) {
        XBee.print(ATMY);
        XBee.flush();
        delay(100);
      }
    }
    
    if (XBee.available()) { 
      char message = XBee.read();
      Serial.print("Received message: ");
      Serial.println(message);

      if(message == ATMY) {
        Serial.println("Turning LED on");
        digitalWrite(LED_BUILTIN,HIGH);
      } 
      
      if(message == ATMY) {
        Serial.println("Turning LED off");
        digitalWrite(LED_BUILTIN,LOW);
      }
    }
  }
}
