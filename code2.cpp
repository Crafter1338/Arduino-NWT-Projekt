#include "LedControl.h"
#include "Wire.h"
////Einstellungen//////////////////
LedControl lc = LedControl(12,11,10,1);
int speaker = 6;
int taster  = 4;
int x = 0;
////Setup//////////////////////////
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A1));
  pinMode(4, INPUT_PULLUP);
  
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  Wire.begin(9);
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
  x = Wire.read();
}

void loop() {
  if (x != 0) {
    x = 0;
    float result = (float) x;

    delay(1000);
    lc.shutdown(0, false);
    lc.clearDisplay(0);
  
    for (int i = 0; i < 5; i++){
      result += ((float) test_display())/10;
      delay(1000);
    }

    Serial.println(result);
  } 
}
            
////Display////////////////////////
void circle(){
  lc.setColumn(0,1,B00011000);
  lc.setColumn(0,2,B00111100);
  lc.setColumn(0,3,B01111110);
  lc.setColumn(0,4,B01111110);
  lc.setColumn(0,5,B00111100);
  lc.setColumn(0,6,B00011000);
}

void arrow(){
  lc.setColumn(0,1,B00011000);
  lc.setColumn(0,2,B00011000);
  lc.setColumn(0,3,B00011000);
  lc.setColumn(0,4,B01111110);
  lc.setColumn(0,5,B00111100);
  lc.setColumn(0,6,B00011000);
}

void square(){
  for (int column = 1; column < 7; column++){
    lc.setColumn(0, column,B01111110);
  }
}

void (*Formen[])() = {circle, arrow, square};

void introduction_display(int num){
  for (int i = 0; i < 3; i++) {
    Formen[i]();
    if (i == num){
      Wire.beginTransmission(9);
      Wire.write(1);
      Wire.endTransmission();
      tone(speaker, 400);
    }
    delay(2000);
    Wire.beginTransmission(9);
    Wire.write(0);
    Wire.endTransmission();
    noTone(speaker);
  }
  lc.clearDisplay(0);
  delay(1000);
}

int test_display(){
  int ran      = random(0,3);
  bool pressed = false;
  int _start;
  int off = 0;

  introduction_display(ran);
  int others[2] = {(ran - 1 < 0)? 2:(ran-1), (ran + 1 > 2)? 0:(ran+1)};
  int current = 0;
  int turns = random(6,16+1);
  for (int i = 0; i < turns; i++){
    Formen[others[current]]();
    delay(1200 - (i*80));
    current++;
    current = (current > 1)? 0:current;
  }
  Formen[ran]();
  
  _start = millis();
  while (digitalRead(taster) == 1){}
  lc.clearDisplay(0);
  
  return millis() - _start + off;
}
