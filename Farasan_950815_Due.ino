#include "Main/Loop.h"
#include "Main/SerialEvent.h"


void setup() {
    // put your setup code here, to run once:
    TimeStartup();
}

void loop() {
    // put your main code here, to run repeatedly:
    TimeLoop();

}

//void serialEvent1() {
//    ExternSerialEvent1();

//}

/*
  // constants won't change. Used here to set a pin number :
  const int ledPin =  43;      // the number of the LED pin
  const int ledPinP =  45;      // the number of the LED pin
  const int ledPinN =  47;      // the number of the LED pin

  // Variables will change :
  int ledState = LOW;             // ledState used to set the LED
  bool InOvelrFlowCheck = false;
  unsigned long InOverFlowTime = 0;
  // Generally, you should use "unsigned long" for variables that hold time
  // The value will quickly become too large for an int to store
  unsigned long previousMillis = 0;        // will store last time LED was updated
  unsigned long previousMillis1 = 0;        // will store last time LED was updated

  // constants won't change :
  const long interval = 5000;           // interval at which to blink (milliseconds)
  const long interval1 = 500;           // interval at which to blink (milliseconds)

  void setup() {
  // set the digital pin as output:
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinP, OUTPUT);
  pinMode(ledPinN, OUTPUT);
  ledState = LOW;
  digitalWrite(ledPin, ledState);

  }

  void loop() {
  unsigned long currentMillis = millis();

  int sensorValue = analogRead(A8);


  if (sensorValue < 100 || sensorValue > 890) {
  previousMillis = currentMillis;
  if ( InOvelrFlowCheck == false)
  {
  Serial.print("Changinge----->   To high");
  InOverFlowTime = currentMillis;
  InOvelrFlowCheck = true;
  } else
  { //InOvelrFlowCheck == true
  if (currentMillis - InOverFlowTime >=  200)
  {
   Serial.print("Change   To high");
   digitalWrite(ledPin, HIGH);
   Serial.println(sensorValue);
  }
  }
  }
  if (currentMillis - previousMillis1 >= interval1)
  {
  InOvelrFlowCheck = false;
  Serial.println(sensorValue);
  previousMillis1 = currentMillis;
  }
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.

  if (currentMillis - previousMillis >= interval) {
  // save the last time you blinked the LED
  previousMillis = currentMillis;

  // if the LED is off turn it on and vice-versa:
  if (ledState == LOW) {
  ledState = HIGH;
  } else {
  ledState = LOW;
  }

  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPinP, ledState);
  digitalWrite(ledPinN, ledState);
  Serial.println( ledState);


  }
  }*/



