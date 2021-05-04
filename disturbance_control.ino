
#define LED1 22
#define LED2 24
#define LED3 26
#define LED3 28
#define LED3 30
#define LED3 32
#define LED3 34
#define LED3 36
#define LED3 38

int LED_array[] = {22, 24, 26, 28, 30, 32, 34, 36, 38}; 

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 10; i++) {
    pinMode(LED_array[i], OUTPUT); 
  }
}

void loop() {
  turnOn(); 
  delay(1000); 
  turnOff(); 
  delay(1000); 
}

void turnOn() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_array[i], HIGH); 
  }
}

void turnOff() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_array[i], LOW); 
  }
}
