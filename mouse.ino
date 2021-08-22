
#include "Mouse.h"

#define m0Pin 10
#define m1Pin 9
#define m2Pin 8
#define slpPin 7
#define dirPin 5
#define stpPin 6
#define inPin 0
#define outPin 1
#define modePin 4
#define busyPin 2
#define homingPin 3

#define homingDir false

bool pos = false;
const int stpsPerRev = 50;

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

  digitalWrite(m0Pin, LOW);
  digitalWrite(m1Pin, LOW);
  digitalWrite(m2Pin, LOW);

  if (digitalRead(modePin) == LOW) {
    Mouse.begin();
    driverWake();
    homingSeq();
    attachInterrupt(digitalPinToInterrupt(inPin), changePos, CHANGE);
  } else {
    driverSlp();
    digitalWrite(busyPin, HIGH);
    digitalWrite(outPin, LOW);
  }
}

void loop() {
  
}

void moveStp(bool dir) {
  digitalWrite(dirPin, dir);
  delayMicroseconds(100);
  digitalWrite(stpPin, HIGH);
  delayMicroseconds(200);
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
  digitalWrite(busyPin, HIGH);
  while (digitalRead(homingPin) == HIGH) {
    moveStp(homingDir);
    delay(100);
  }
  digitalWrite(busyPin, LOW);
}

void changePos() {
  noInterrupts();
  digitalWrite(busyPin, HIGH);
  for (int i = 0; i <= stpsPerRev; i++) {
    moveStp(!pos);
  }
  pos = !pos;
  delay(100);
  digitalWrite(busyPin, LOW);
  delay(200);
  Mouse.click();
  interrupts();
}
