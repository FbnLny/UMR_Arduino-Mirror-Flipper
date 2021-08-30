
#include "Mouse.h"
#include "EEPROM.h"

#define m0Pin 5
#define m1Pin 6
#define m2Pin 7
#define slpPin 8
#define dirPin 10
#define stpPin 9
#define inPin 0
#define outPin 1
#define modePin 4
#define busyPin 2
#define homingPin 3

#define homingDir false

#define stpsPerRev 1600
#define stpDelay 5

#define eepromAddress 0

bool pos = false;

volatile bool changePosRequest = false;
volatile bool homeRequest = false;

void setup() {
  pinMode(m0Pin, OUTPUT);
  pinMode(m1Pin, OUTPUT);
  pinMode(m2Pin, OUTPUT);
  pinMode(slpPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(stpPin, OUTPUT);
  pinMode(outPin, OUTPUT);
  pinMode(busyPin, OUTPUT);
  pinMode(inPin, INPUT);
  pinMode(modePin, INPUT_PULLUP);
  pinMode(homingPin, INPUT_PULLUP);
  pinMode(11, OUTPUT);

  digitalWrite(m0Pin, LOW);
  digitalWrite(m1Pin, LOW);
  digitalWrite(m2Pin, LOW);

  digitalWrite(11, HIGH);

  //Serial.begin(9600);

  if (digitalRead(modePin) == LOW) {
    Mouse.begin();
    driverWake();
    //attachInterrupt(digitalPinToInterrupt(homingPin), intHomeDetected, FALLING);
    homingSeq();
    if (EEPROM.read(eepromAddress) == 1) {
      pos = true;
      changePos();
    }
    digitalWrite(outPin, pos);
    delay(200);
    attachInterrupt(digitalPinToInterrupt(inPin), intDetected, RISING);
    
  } else {
    driverSlp();
    digitalWrite(busyPin, HIGH);
    digitalWrite(outPin, LOW);
  }
}

void loop() {
  if (changePosRequest == true) {
    changePos();
    changePosRequest = false;
    //Serial.println("reset");
  }
}

void moveStp(bool dir) {
  digitalWrite(dirPin, dir);
  delayMicroseconds(200);
  digitalWrite(stpPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(stpPin, LOW);
}

void driverSlp() {
  digitalWrite(slpPin, LOW);
}

void driverWake() {
  digitalWrite(slpPin, HIGH);
  delay(1);
}

void homingSeq() {
  //Serial.println("homing");
  digitalWrite(busyPin, HIGH);
  setMicroStp();
  delay(200);
  //attachInterrupt(digitalPinToInterrupt(homingPin), intHomeDetected, FALLING);
  
  //while (homeRequest == false) {
  while (digitalRead(homingPin) == HIGH) {
    moveStp(homingDir);
    delay(stpDelay);
  }
  setFullStp();
  driverSlp();
  driverWake();
  digitalWrite(busyPin, LOW);
  //Serial.println("homed");
}

void changePos() {
  //Serial.println("moving");
  digitalWrite(busyPin, HIGH);
  setMicroStp();
  delay(1);
  for (int i = 0; i < stpsPerRev; i++) {
    moveStp(!pos);
    delay(stpDelay);
  }
  setFullStp();
  pos = !pos;
  digitalWrite(outPin, pos);
  driverSlp();
  driverWake();
  EEPROM.write(eepromAddress, pos);
  delay(100);
  //Serial.println("clicking");
  Mouse.press();
  delay(50);
  Mouse.release();
  digitalWrite(busyPin, LOW);
}

void intDetected() {
  noInterrupts();
  changePosRequest = true;
  interrupts();
}

void intHomeDetected() {
  detachInterrupt(digitalPinToInterrupt(homingPin));
  homeRequest = true;
}

void setFullStp() {
  digitalWrite(m0Pin, LOW);
  digitalWrite(m1Pin, LOW);
  digitalWrite(m2Pin, LOW);
}

void setMicroStp() {
  digitalWrite(m0Pin, HIGH);
  digitalWrite(m1Pin, HIGH);
  digitalWrite(m2Pin, HIGH);
}
