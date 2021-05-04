// Mechatronics Final Project
// Team 11
// John Han, Peter Weiss, Carl Urbanik

#include <Pixy2.h>
#include <SPI.h> 
#include <AFMotor.h>
#include <math.h>

#define dist_sensor1 10
#define dist_sensor2 11
#define dist_sensor3 12

AF_Stepper thwack_motor(2048, 2); 
Pixy2 pixy; 
AF_DCMotor motor(1); 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  thwack_motor.setSpeed(60); 
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
