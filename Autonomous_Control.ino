// Mechatronics Final Project
// Team 11
// John Han, Peter Weiss, Carl Urbanik

#include <Pixy2UART.h>
#include <AFMotor.h>
#include <Servo.h>
#include <SPI.h>
#include <SharpIR.h>

#define stepsPerRevolution 2048
#define echo_pin 44 
#define trig_pin 46
#define ultrasonic_generator 30
#define IR_model 1080
#define IR_sensor_pinL A12
#define IR_sensor_pinR A13

AF_DCMotor L_motor(3);
AF_DCMotor R_motor(4);
Pixy2UART pixy; 
Servo gun_motor; 
SharpIR irSensorL = SharpIR(IR_sensor_pinL, IR_model);
SharpIR irSensorR = SharpIR(IR_sensor_pinR, IR_model);

int servo_position = 30; 
int motor_speed = 250; 
float distance_enemy; 
float distance_needed = 90; // how far away do we shoot?
long pulseduration;
float distR, distL; 
int count = 0; 

// LED disturbance
int LED_array[] = {22, 24, 26, 28, 30, 32, 34, 36, 38}; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 

  // pixy init
  pixy.init(9600); 
  
  // distance sensor init
  pinMode(trig_pin, OUTPUT); 
  pinMode(echo_pin, INPUT); 

  // ultrasonic signal generator init
  pinMode(ultrasonic_generator, OUTPUT); 
  digitalWrite(ultrasonic_generator, HIGH); 

  // gun servo init
  gun_motor.attach(10);
  gun_motor.write (servo_position); 
  
  for (int i = 0; i < 9; i++) {
    pinMode(LED_array[i], OUTPUT); 
  }
}

void loop() {

  // first check for a wall on our left and right to veer away
  //distL = irSensorL.distance(); 
  //distR = irSensorR.distance(); 
  
//  if (distL < 5) {
//    // too close to a wall on our left
//    scootchRight(); 
//    return;
//  } 
//  if (distR < 5) {
//    // too close to a wall on our right
//    scootchLeft();  
//    return; 
//  }
  
  //digitalWrite(ultrasonic_generator, LOW); 
  // put your main code here, to run repeatedly:
  pixy.ccc.getBlocks(); 
  
  // a centering algorithm
  if (getPixyColor() == 1) {
    if (pixy.ccc.blocks[0].m_x > 170) { // Post-it too far right
      scootchRight();                     // Turn right (slightly)
      Serial.println("Turn right");
    }
    else if (pixy.ccc.blocks[0].m_x < 146) { // Post-it too far left
      scootchLeft(); 
      Serial.println("Turn left");
    }
    else {
       // get distance sensor measurements
       // get to that distance and maintain
       // shoot();
       distance_enemy = readDistance(); 
       if (abs(distance_enemy - distance_needed) > 5) {
        if (distance_enemy > distance_needed) {
          // it's too far away 
          Serial.println("Aligned, but too far");
          Serial.println(distance_enemy);
          moveForward(); 
        } else {
          moveBackward(); 
          Serial.println("Aligned, but too close");
          Serial.println(distance_enemy);
        }
        //distance_enemy = readDistance(); 
        Serial.print("dist sensed: "); 
        Serial.println(distance_enemy); 
       } else {
        shoot(); 
        Serial.println("Fire!");
       }
    }
  } else {
    spinSlowly(); 
    Serial.println("Balloon not spotted");
  }
  //digitalWrite(ultrasonic_generator, HIGH); 
}

int getPixyColor() {
  return pixy.ccc.blocks[0].m_signature;
}

void moveForward() {
  L_motor.run(RELEASE);
  R_motor.run(RELEASE); 
  delay(100); 
  setDefaultSpeed();
  L_motor.run(FORWARD); 
  R_motor.run(FORWARD); 
  delay(300); 
  L_motor.run(RELEASE);
  R_motor.run(RELEASE); 
  delay(100); 
}

void moveBackward() {
  L_motor.run(RELEASE);
  R_motor.run(RELEASE); 
  delay(100); 
  setDefaultSpeed();
  L_motor.run(BACKWARD); 
  R_motor.run(BACKWARD); 
  delay(300); 
  L_motor.run(RELEASE);
  R_motor.run(RELEASE); 
  delay(100); 
}

void scootchLeft() {
  L_motor.run(RELEASE);
  R_motor.run(RELEASE); 
  delay(100); 
  L_motor.setSpeed(150); 
  R_motor.setSpeed(150); 
  L_motor.run(BACKWARD);
  R_motor.run(FORWARD);
  delay(100); 
  L_motor.run(RELEASE);
  R_motor.run(RELEASE);  
  delay(200); 
  //setDefaultSpeed(); 
}

void scootchRight() {
  L_motor.run(RELEASE);
  R_motor.run(RELEASE); 
  delay(100); 
  L_motor.setSpeed(150); 
  R_motor.setSpeed(150); 
  L_motor.run(FORWARD);
  R_motor.run(BACKWARD);
  delay(100); 
  L_motor.run(RELEASE);
  R_motor.run(RELEASE); 
  delay(200); 
  //setDefaultSpeed(); 
}

void setDefaultSpeed() {
  L_motor.setSpeed(motor_speed); 
  R_motor.setSpeed(motor_speed); 
}

float readDistance() {
  // send a flurry of signal to activate trigger
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  pulseduration = pulseIn(echo_pin, HIGH);
  // Calculating the distance
  return pulseduration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
}

void shoot() {
  gun_motor.write(servo_position + 90); 
  delay(100); 
  gun_motor.write (servo_position); 
  delay(500);
}

void spinSlowly() {
  if (count % 2 == 0) {
    L_motor.run(RELEASE);
    R_motor.run(RELEASE); 
    delay(100); 
    L_motor.setSpeed(100);
    R_motor.setSpeed(100); 
    L_motor.run(BACKWARD); 
    R_motor.run(FORWARD); 
    delay(800); 
    L_motor.run(RELEASE);
    R_motor.run(RELEASE); 
    delay(100); 
    L_motor.run(FORWARD); 
    R_motor.run(BACKWARD); 
    delay(800); 
    L_motor.run(RELEASE);
    R_motor.run(RELEASE); 
    delay(100); 
  } else {
    L_motor.run(RELEASE);
    R_motor.run(RELEASE); 
    delay(100); 
    L_motor.setSpeed(100);
    R_motor.setSpeed(100); 
    L_motor.run(FORWARD); 
    R_motor.run(BACKWARD); 
    delay(800); 
    L_motor.run(RELEASE);
    R_motor.run(RELEASE); 
    delay(100); 
    L_motor.run(BACKWARD); 
    R_motor.run(FORWARD); 
    delay(800); 
    L_motor.run(RELEASE);
    R_motor.run(RELEASE); 
    delay(100); 
  }
  count++; 
}
