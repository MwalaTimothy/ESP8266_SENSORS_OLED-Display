#include <Arduino.h>
const int MOTION_SENSOR_PIN = D3;   // Arduino pin connected to the OUTPUT pin of motion sensor
const int BUZZER_PIN        = D5;   // Arduino pin connected to Buzzer's pin
int motionStateCurrent      = LOW; // current  state of motion sensor's pin
int motionStatePrevious     = LOW; // previous state of motion sensor's pin

void setup() {
  Serial.begin(9600);                // initialize serial
  pinMode(MOTION_SENSOR_PIN, INPUT); // set arduino pin to input mode
  pinMode(BUZZER_PIN, OUTPUT);          // set arduino pin to output mode
}

void loop() {
  motionStatePrevious = motionStateCurrent;   // store old state     
  motionStateCurrent  = digitalRead(MOTION_SENSOR_PIN); // read new state   
  
  if (motionStatePrevious == LOW && motionStateCurrent == HIGH) 
  {
    digitalWrite(BUZZER_PIN, HIGH); // turn on
    delay(5000);
  }
  else
  if (motionStatePrevious == HIGH && motionStateCurrent == LOW) 
  {
    digitalWrite(BUZZER_PIN, LOW);  // turn off
  }
}
