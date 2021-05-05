// Mechatronics Final Project
// Team 11
// John Han, Peter Weiss, Carl Urbanik

#include <Pixy2UART.h>
#include <AFMotor.h>
#include <Servo.h>

#define stepsPerRevolution 2048
#define echo_pin 44
#define trig_pin 46
#define ultrasonic_generator 30

AF_DCMotor L_motor(3);
AF_DCMotor R_motor(4);
Pixy2UART pixy; 
Servo gun_motor; 

int servo_position; 
int motor_speed = 200; 
float distance_enemy; 
float distance_needed = 150; // how far away do we shoot?
long pulseduration;

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
  
  for (int i = 0; i < 9; i++) {
    pinMode(LED_array[i], OUTPUT); 
  }

}

void loop() {

  digitalWrite(ultrasonic_generator, LOW); 
  // put your main code here, to run repeatedly:
  pixy.ccc.getBlocks(); 
  
  // a centering algorithm
  if (getPixyColor() == 1) {
    if (pixy.ccc.blocks[0].m_x > 165) { // Post-it too far right
      scootchRight();                     // Turn right (slightly)
      Serial.println("Turn right");
    }
    else if (pixy.ccc.blocks[0].m_x < 151) { // Post-it too far left
      scootchLeft();                          // Turn left (slightly)
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
        //Serial.print("dist sensed: "); 
        //Serial.println(distance_enemy); 
       } else {
        shoot(); 
        Serial.println("Fire!");
       }
    }
  } else {
    stopMotors(); 
    Serial.println("Balloon not spotted");
  }
  digitalWrite(ultrasonic_generator, HIGH); 
}

int getPixyColor() {
  return pixy.ccc.blocks[0].m_signature;
}

void moveForward() {
  L_motor.run(FORWARD); 
  R_motor.run(FORWARD); 
  delay(150); 
}

void moveBackward() {
  L_motor.run(BACKWARD); 
  R_motor.run(BACKWARD); 
}

void scootchLeft() {
  //L_motor.run(RELEASE);
  //R_motor.run(RELEASE); 
  L_motor.setSpeed(150); 
  R_motor.setSpeed(150); 
  L_motor.run(BACKWARD);
  R_motor.run(FORWARD);
  delay(100); 
  //L_motor.run(RELEASE);
  //R_motor.run(RELEASE);  
  setDefaultSpeed(); 
}

void scootchRight() {
  //L_motor.run(RELEASE);
  //R_motor.run(RELEASE); 
  L_motor.setSpeed(150); 
  R_motor.setSpeed(150); 
  L_motor.run(FORWARD);
  R_motor.run(BACKWARD);
  delay(100); 
  //L_motor.run(RELEASE);
  //R_motor.run(RELEASE); 
  setDefaultSpeed(); 
}

void setDefaultSpeed() {
  L_motor.setSpeed(motor_speed); 
  R_motor.setSpeed(motor_speed); 
}

void stopMotors() {
  L_motor.run(RELEASE);
  R_motor.run(RELEASE); 
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
  gun_motor.write(servo_position + 45); 
  delay(100); 
  gun_motor.write (servo_position); 
}
