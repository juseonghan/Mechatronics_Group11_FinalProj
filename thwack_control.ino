// Mechatronics Final Project
// Team 11
// John Han, Peter Weiss, Carl Urbanik

#include <AFMotor.h>

#define LED1 22
#define LED2 24
#define LED3 26
#define LED3 28
#define LED3 30
#define LED3 32
#define LED3 34
#define LED3 36
#define LED3 38
#define stepsPerRevolution 2048

int LED_array[] = {22, 24, 26, 28, 30, 32, 34, 36, 38}; 

AF_Stepper thwack_motor(stepsPerRevolution, 2); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  for (int i = 0; i < 9; i++) {
    pinMode(LED_array[i], OUTPUT); 
  }

  thwack_motor.setSpeed(10); 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  thwack_motor.step(1024, FORWARD, DOUBLE); 
  turnOn(); 
  delay(1000); 
  turnOff(); 
  delay(1000); 
}

void turnOn() {
  for (int i = 0; i < 9; i++) {
    digitalWrite(LED_array[i], HIGH); 
  }
}

void turnOff() {
  for (int i = 0; i < 9; i++) {
    digitalWrite(LED_array[i], LOW); 
  }
}

//#include <Wire.h>
//#include <Adafruit_MotorShield.h>
//#include <Adafruit_PWMServoDriver.h>
//
//Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//Adafruit_StepperMotor *thwack_motor = AFMS.getStepper(2048, 2);
//
//void setup() {
//  thwack_motor->setSpeed(10); 
//}
//
//void loop() {
//  thwack_motor->step(512, FORWARD, DOUBLE);
//  delay(200); 
//}
