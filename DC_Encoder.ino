#include <AFMotor.h>

// motor 1 = left motor
// motor 2 = right motor

// Motor controller pins:
int L_EN1 = 18;  // ch1 motor1
int L_EN2 = 19;  // ch2 motor1
int R_EN1 = 20;  // ch1 motor2
int R_EN2 = 21;  // ch2 motor2

// encoder tick counter variables
unsigned long num_ticksL = 0;
unsigned long num_ticksR = 0;

// pulse durations 
unsigned long lastPulseL, lastPulseR; 

// width of pulse in ms we want the motor to run
int motorTime = 120;

// encoder tick timing variables
float ltime = 0.0;
float rtime = 0.0;
float time_elapsedR = 0.0;
float time_elapsedL = 0.0;

// Motor speed

int desired_speed = 100; 
byte motor_speedL = desired_speed;
byte motor_speedR = desired_speed;

#define runEvery(t) for (static typeof(t) _lasttime;(typeof(t))((typeof(t))millis() - _lasttime) > (t);_lasttime += (t))

// motor will be on motor shield 3 and 4
AF_DCMotor L_motor(3);
AF_DCMotor R_motor(4);

void setup() {
  Serial.begin(9600); //to use the serial monitor
  pinMode(L_EN1, INPUT);
  pinMode(L_EN1, INPUT);
  pinMode(R_EN2, INPUT);
  pinMode(R_EN2, INPUT);

  L_motor.setSpeed(motor_speedL);
  R_motor.setSpeed(motor_speedR);

  attachInterrupt(L_EN1, count_tickL, CHANGE);
  attachInterrupt(L_EN2, check_speedL, CHANGE);
  attachInterrupt(R_EN1, count_tickR, CHANGE);
  attachInterrupt(R_EN2, check_speedR, CHANGE);
}

void loop() {

  //runEvery(500) {
//    Serial.print("lpulse = ");
//    Serial.println(lpulse);
//    Serial.print("speedLeft = ");
//    Serial.println(motor_speedL);
//    Serial.print("speedRight = ");
//    Serial.println(motor_speedR);
  //}


  runEvery(5) {                            // Run every 5ms, too fast would stop the motors
    lastPulseL = millis() - ltime;     // Check how much time has passed since last pulse
    lastPulseR = millis() - rtime;

    if (lastPulseL > motorTime && motor_speedL < desired_speed)    // If the pulse is longer than the time we want speed up motor
    {
      motor_speedL = motor_speedL + 5;
    }
    if (lastPulseL < motorTime && motor_speedL > 0)      // If the pulse is shorter than the time we want slow down motor
    {
      motor_speedL = motor_speedL - 5;
    }
    if (lastPulseR > motorTime && motor_speedR < desired_speed) {
      motor_speedR = motor_speedR + 5;
    }
    if (lastPulseR < motorTime && motor_speedR > 0) {
      motor_speedR = motor_speedR - 5;
    }
    if (num_ticksL < num_ticksR) {
      // not enough ticks on the left motor, so increase its speed
      motor_speedL = motor_speedL + 5; 
    } else if (num_ticksL > num_ticksR) {
      // not enough ticks on the right motor, increase right motor speed
      motor_speedR = motor_speedR + 5; 
    }

    L_motor.setSpeed(motor_speedL);
    R_motor.setSpeed(motor_speedR);

    Serial.print("left motor speed: "); 
    Serial.println(motor_speedL); 
    Serial.print("right motor speed: "); 
    Serial.println(motor_speedR); 
  }

  L_motor.run(FORWARD);
  R_motor.run(FORWARD);
}

void count_tickL() {
  num_ticksL++;
}

void count_tickR() {
  num_ticksR++;
}

void check_speedL() {
  time_elapsedL = millis() - ltime;
  ltime = millis();
}

void check_speedR() {
  time_elapsedR = millis() - rtime;
  rtime = millis();
}
