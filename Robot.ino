#include <Servo.h>
#include "SR04.h"

int enable = LOW;
int pos = 95;
int pinButton = 13;
int valButtonOld = LOW;
int statoDx = 255;
int statoSx = 255;
long distance;
long distanceDx;
long distanceSx;

#define ENABLEDX 5
#define DIRADX 3
#define DIRBDX 4
#define ENABLESX 9
#define DIRASX 6
#define DIRBSX 7
#define TRIG_PIN 12
#define ECHO_PIN 11

Servo myservo;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

void setup() {
  pinMode(pinButton, INPUT);
  pinMode(ENABLEDX, OUTPUT);
  pinMode(DIRADX, OUTPUT);
  pinMode(DIRBDX, OUTPUT);
  pinMode(ENABLESX, OUTPUT);
  pinMode(DIRASX, OUTPUT);
  pinMode(DIRBSX, OUTPUT);
  myservo.attach(10);
  Serial.begin(9600);
}

void loop() {
  int valButton = digitalRead(pinButton);
  if(valButton == HIGH && valButton != valButtonOld)
    enable = !enable;
  if (enable == HIGH) {
    analogWrite(ENABLEDX, statoDx); //enable on
    digitalWrite(DIRADX, HIGH);
    digitalWrite(DIRBDX, LOW);
    analogWrite(ENABLESX, statoSx); //enable on
    digitalWrite(DIRASX, HIGH);
    digitalWrite(DIRBSX, LOW);
  
    distance=sr04.Distance();
    if(distance <= 30) {
      //fermo il robottino
      digitalWrite(ENABLEDX, LOW); //enable off
      digitalWrite(ENABLESX, LOW); //enable off
      //controlla se ci sono ostacoli sulla sinistra
      for (pos = 95; pos <= 180; pos += 1) {
        myservo.write(pos);
        delay(15);  //waits 15ms for the servo to reach the position
      }
      distanceSx=sr04.Distance();

      //controlla se ci sono ostacoli sulla destra
      for (pos = 180; pos >= 5; pos -= 1) {
        myservo.write(pos);
        delay(15);  //waits 15ms for the servo to reach the position
      }

      distanceDx=sr04.Distance();

      //riporta il senso in posizione standard
      for (pos = 5; pos <= 95; pos += 1) {
        myservo.write(pos);
        delay(15);  //waits 15ms for the servo to reach the position
      }

      if(distanceDx >= distanceSx) {
        //gira robottino verso destra
        analogWrite(ENABLEDX, statoDx); //enable on
        analogWrite(ENABLESX, statoSx); //enable on
        digitalWrite(DIRADX, LOW);
        digitalWrite(DIRBDX, HIGH);
        digitalWrite(DIRASX, HIGH);
        digitalWrite(DIRBSX, LOW);
      }
      else {
        //gira robottino verso sinistra
        analogWrite(ENABLEDX, statoDx); //enable on
        analogWrite(ENABLESX, statoSx); //enable on
        digitalWrite(DIRADX, HIGH);
        digitalWrite(DIRBDX, LOW);
        digitalWrite(DIRASX, LOW);
        digitalWrite(DIRBSX, HIGH);
      }
      delay(9000); //tempo necessario al robottino per girare
    }
  }
  else {
    analogWrite(ENABLEDX, LOW); //enable off
    analogWrite(ENABLESX, LOW); //enable off
  }
  valButtonOld = valButton;
}