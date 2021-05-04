// Mechatronics Final Project
// Team 11
// John Han, Peter Weiss, Carl Urbanik

#include <Servo.h>
#include <Pixy2.h>
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

Servo gun_motor; 
AF_Stepper thwack_motor(stepsPerRevolution, 2); 
Pixy2 pixy; 
int servo_position; 
int front_ping_sensor = 4; 
float distance_enemy, desired_distance, pulseduration; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  gun_motor.attach(10); 
  servo_position = 30; 
  pixy.init(); 
  distance_enemy = 0.0;
  
  pinMode(front_ping_sensor, OUTPUT); 
  for (int i = 0; i < 9; i++) {
    pinMode(LED_array[i], OUTPUT); 
  }

  thwack_motor.setSpeed(10); 
}

void loop() {
  // put your main code here, to run repeatedly:
  thwack_motor.step(1024, FORWARD, DOUBLE); 
  turnOnLED(); 
  delay(1000); 
  pixy.ccc.getBlocks(); 
  // if we don't detect anything, then just continue looking around motion
  if (!pixy.ccc.numBlocks) {
    // robot.spinAround(); 
    delay(1000); 
    return; 
  }

  // read the distance from the front distance sensor
  distance_enemy = readDistance(front_ping_sensor); 

  // we need to get to the distance to shoot the gun from so that it can hit their balloon
  while (abs(distance_enemy - desired_distance) > 5) {
    if (distance_enemy > desired_distance) {
      // we're too far away 
      // robot.goForward(); 
    } else {
      // too close up
      // robot.goBackwards(); 
    }
  }

  // we detected it! calculate some sort of distance + then shoot it
  shoot(); 
  
}

void shoot() {
  gun_motor.write(servo_position); 
  delay(500); 
  gun_motor.write(servo_position + 45); 
  delay(100); 
  gun_motor.write (servo_position); 
  delay(5000); 
}

float readDistance(int pin) {
  // send a flurry of signal to activate trigger
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(5);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
  // then start reading from the pin
  pinMode(pin, INPUT);
  // get the duration of the pulse as it's high
  pulseduration = pulseIn(pin, HIGH);
  // do some calculations and return the value in cm
  return pulseduration * 343 / 20000;
}

void turnOnLED() {
  for (int i = 0; i < 9; i++) {
    digitalWrite(LED_array[i], HIGH); 
  }
}

void turnOffLED() {
  for (int i = 0; i < 9; i++) {
    digitalWrite(LED_array[i], LOW); 
  }
}
