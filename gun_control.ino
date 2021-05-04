#include <Servo.h>
#include <Pixy2.h>
#include <AFMotor.h>

Servo gun_motor; 
Pixy2 pixy; 
int servo_position; 
int front_ping_sensor = 4; // pin # of ultrasonic distance sensor
float distance_enemy, desired_distance, pulseduration; 

void setup() {
 
  Serial.begin(9600); 
  delay(4000); 
  gun_motor.attach(10); 
  servo_position = 30; 
  pixy.init(); 
  distance_enemy = 0.0;
  pinMode(front_ping_sensor, OUTPUT); 
}

void loop() {

  // first need to look around to detect the balloon
  // next need to make the car go to a certain distance and maintain that distance
  // then shoot!
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
