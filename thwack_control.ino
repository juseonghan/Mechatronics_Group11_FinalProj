// Mechatronics Final Project
// Team 11
// John Han, Peter Weiss, Carl Urbanik

#include <AFMotor.h>

#define stepsPerRevolution 2048

AF_Stepper thwack_motor(stepsPerRevolution, 2); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  Serial.println("Thwack control"); 

  thwack_motor.setSpeed(10); 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  thwack_motor.step(1024, FORWARD, DOUBLE); 
  delay(200); 
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
