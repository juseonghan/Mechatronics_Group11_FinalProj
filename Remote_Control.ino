#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>
#include <math.h>
#include <AFMotor.h>

// motor 1 = left motor
// motor 2 = right motor

// Motor controller pins:
int L_EN1 = 18;  // ch1 motor1
int L_EN2 = 19;  // ch2 motor1
int R_EN1 = 20;  // ch1 motor2
int R_EN2 = 21;  // ch2 motor2

// pulse durations 
unsigned long lastPulseL, lastPulseR; 

// encoder tick counter variables
unsigned long num_ticksL = 0;
unsigned long num_ticksR = 0;

// width of pulse in ms we want the motor to run
int motorTime = 1000;

// something
float motorTime_L, motorTime_R; 

// encoder tick timing variables
volatile float ltime = 0.0;
volatile float rtime = 0.0;
volatile float time_elapsedR = 0.0;
volatile float time_elapsedL = 0.0;

// Motor speed

int desired_speed = 200; 
bool motor_dirL = 0;
bool motor_dirR = 0;
byte motor_speedL = desired_speed;
byte motor_speedR = desired_speed;

#define runEvery(t) for (static typeof(t) _lasttime;(typeof(t))((typeof(t))millis() - _lasttime) > (t);_lasttime += (t))

// motor will be on motor shield 3 and 4
AF_DCMotor L_motor(3);
AF_DCMotor R_motor(4);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);      // make sure your Serial Monitor is also set at this baud rate.
  Dabble.begin(9600);      //Enter baudrate of your bluetooth.Connect bluetooth on Bluetooth port present on evive.
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
  
 Dabble.processInput();             //this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.

  // https://www.impulseadventure.com/elec/robot-differential-steering.html

  // INPUTS
  int     joy_x = GamePad.getXaxisData();              // Joystick X input                     (-128..+127)
  int     joy_y = GamePad.getYaxisData();              // Joystick Y input                     (-128..+127)

  /*  // OUTPUTS
  int     nMotMixL;           // Motor (left)  mixed output           (-128..+127)
  int     nMotMixR;           // Motor (right) mixed output           (-128..+127)
  
  // CONFIG
  // - fPivYLimt  : The threshold at which the pivot action starts
  //                This threshold is measured in units on the Y-axis
  //                away from the X-axis (Y=0). A greater value will assign
  //                more of the joystick's range to pivot actions.
  //                Allowable range: (0..+127)
  float fPivYLimit = 32.0;
        
  // TEMP VARIABLES
  float   nMotPremixL;    // Motor (left)  premixed output        (-128..+127)
  float   nMotPremixR;    // Motor (right) premixed output        (-128..+127)
  int     nPivSpeed;      // Pivot Speed                          (-128..+127)
  float   fPivScale;      // Balance scale b/w drive and pivot    (   0..1   )
  
  
  // Calculate Drive Turn output due to Joystick X input
  if (nJoyY >= 0) {
    // Forward
    nMotPremixL = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
    nMotPremixR = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
  } else {
    // Reverse
    nMotPremixL = (nJoyX>=0)? (127.0 - nJoyX) : 127.0;
    nMotPremixR = (nJoyX>=0)? 127.0 : (127.0 + nJoyX);
  }
  
  // Scale Drive output due to Joystick Y input (throttle)
  nMotPremixL = nMotPremixL * nJoyY/128.0;
  nMotPremixR = nMotPremixR * nJoyY/128.0;
  
  // Now calculate pivot amount
  // - Strength of pivot (nPivSpeed) based on Joystick X input
  // - Blending of pivot vs drive (fPivScale) based on Joystick Y input
  nPivSpeed = nJoyX;
  fPivScale = (abs(nJoyY)>fPivYLimit)? 0.0 : (1.0 - abs(nJoyY)/fPivYLimit);
  
  // Calculate final mix of Drive and Pivot
  nMotMixL = (1.0-fPivScale)*nMotPremixL + fPivScale*( nPivSpeed);
  nMotMixR = (1.0-fPivScale)*nMotPremixR + fPivScale*(-nPivSpeed);
  */

  //if (((int)nMotMixL) > -128) {
    int motor_speedLeft = abs(joy_y*(255.0/7.0));
  //}
  //else {
    //motor_speedL = 254;
  //}
  //if (((int)nMotMixR) > -128) {
    int motor_speedRight = abs(joy_x*(255.0/7.0));
  //}
  //else {
    //motor_speedR = 254;
  //}

  motor_dirL = joy_y<0;
  motor_dirR = joy_x>0;

  motorTime_L = motorTime*(256.0/motor_speedL);
  motorTime_R = motorTime*(256.0/motor_speedR);

  runEvery(5) {
    lastPulseL = millis() - ltime;     // Check how much time has passed since last pulse
    lastPulseR = millis() - rtime;

    if (lastPulseL > motorTime && motor_speedL < desired_speed)    // If the pulse is longer than the time we want speed up motor
    {
      motor_speedL = motor_speedL + 5;
    }
    if (time_elapsedL < motorTime && motor_speedL > 0)      // If the pulse is shorter than the time we want slow down motor
    {
      motor_speedL = motor_speedL - 5;
    }
    if (lastPulseR > motorTime && motor_speedR < desired_speed) {
      motor_speedR = motor_speedR + 5;
    }
    if (time_elapsedR < motorTime && motor_speedR > 0) {
      motor_speedR = motor_speedR - 5;
    }
    //if (num_ticksL < num_ticksR) {
      // not enough ticks on the left motor, so increase its speed
      //motor_speedL = motor_speedL + 5; 
    //} else if (num_ticksL > num_ticksR) {
      // not enough ticks on the right motor, increase right motor speed
      //motor_speedR = motor_speedR + 5; 
    //}

    L_motor.setSpeed(motor_speedLeft);
    R_motor.setSpeed(motor_speedRight);
  }
  
  Serial.print("Left Motor Speed: ");
  if (motor_dirL) {
    Serial.print("-");
  }
  Serial.println(motor_speedL);
  Serial.print("Right Motor Speed: ");
  if (motor_dirR) {
    Serial.print("-");
  }
  Serial.println(motor_speedR);
  Serial.println();
  

  /*Serial.println(GamePad.getXaxisData());
  Serial.println(GamePad.getYaxisData());
  Serial.println();
  */
  // forward = 1
  // backwards = 0
  if (motor_dirL == 1) {
    L_motor.run(FORWARD);
  } else {
    L_motor.run(BACKWARD); 
  }
  if (motor_dirR == 1) {
    R_motor.run(FORWARD);
  } else {
    R_motor.run(BACKWARD);
  }
  //delay(5);
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
