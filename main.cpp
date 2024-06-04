#include "LedControl.h"
////Einstellungen//////////////////
const unsigned char farben[3][3]  = { {255,0,0}, {0,255,0}, {0,0,255} };
const unsigned int frequenzen[3]  = {80, 400, 1200};
////Pins///////////////////////////
const unsigned char rgbLed[3]     = {3, 10, 11}; 
const unsigned char speaker       = 4;
const unsigned char taster[3]     = {5, 6, 7};
LedControl display                = LedControl(12, 11, 10, 1);
////Setup//////////////////////////
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A1));

  display.shutdown(0, false);
  display.setIntensity(0, 8);
  display.clearDisplay(0);

  pinMode(taster[0], INPUT_PULLUP);
  pinMode(taster[1], INPUT_PULLUP);
  pinMode(taster[2], INPUT_PULLUP);
  pinMode(speaker, OUTPUT);

  test_main();
}

void loop() {}

////Licht//////////////////////////
int test_licht(){
  int ran       = random(0,3);
  int farbe     = farben[ran];
  bool pressed  = false;
  int delta     = 0;
  int _start, result;
  delay(600);

  _start = millis();
  set_rgb(rgbLed, farbe);

  while(pressed == false){
    for (int index = 0; index < 3; index++){
      if (digitalRead(taster[index]) == 0){
        if (ran == index){
          result = (millis() - _start);
          clear_rgb(rgbLed);
          pressed = true;
        }else{tone(speaker, frequenzen[1]);
          delta = 200;
        }
      }
    } 
  }
  noTone(speaker);
  clear_rgb(rgbLed);

  return result + delta;
}

void set_rgb(char led[3], char farbe[3]){
  clear_rgb(led[3]);
  analogWrite(led[0], farbe[0]);
  analogWrite(led[1], farbe[1]);
  analogWrite(led[2], farbe[2]);
}

void clear_rgb(char led[3]){
  digitalWrite(led[0], LOW);
  digitalWrite(led[1], LOW);
  digitalWrite(led[2], LOW);
}

////Ton////////////////////////////
void introduction_tone(){
  tone(speaker, frequenzen[0]);
  delay(1000);
  tone(speaker, frequenzen[1]);
  delay(1000);
  tone(speaker, frequenzen[2]);
  delay(1000);
  noTone(speaker);
  delay(1000);
}

int test_ton(){ 
  int ran       = random(0,3);
  int frequenz  = frequenzen[ran];
  bool pressed  = false;
  int delta     = 0;
  int _start, result;
  delay(800);

  _start = millis();
  tone(speaker, frequenz);

  while(pressed == false){
    for (int index = 0; index < 3; index++){
      if (digitalRead(taster[index]) == 0){
        if (ran == index){
          result = (millis() - _start);
          pressed = true;
        }
        if (index != ran){
          set_rgb(rgbLed, farben[0]);
          delta = 200;
        }
      }
    } 
  }
  clear_rgb(rgbLed);
  noTone(speaker);

  return result + delta;
}

////Display////////////////////////
void circle(){
  display.setColumn(0,1,B00011000);
  display.setColumn(0,2,B00111100);
  display.setColumn(0,3,B01111110);
  display.setColumn(0,4,B01111110);
  display.setColumn(0,5,B00111100);
  display.setColumn(0,6,B00011000);
}

void arrow(){
  display.setColumn(0,1,B00011000);
  display.setColumn(0,2,B00011000);
  display.setColumn(0,3,B00011000);
  display.setColumn(0,4,B01111110);
  display.setColumn(0,5,B00111100);
  display.setColumn(0,6,B00011000);
}

void square(){
  for (int column = 1; column < 7; column++){
    display.setColumn(0,column,B01111110);
  }
}

void (*Formen[])() = {circle, arrow, square};

void introduction_display(int num){
  for (int i = 0; i < 3; i++) {
    Formen[i]();
    if (i == num){
      tone(speaker, frequenzen[1]);
    }
    delay(2000);
  }
}

int test_display(){
  int ran       = random(0,3);
  bool pressed  = false;
  int _start    = 0;
  int result;

  while (pressed == false) {
    int current = random(0, 3);
    Formen[current]();
    if (current = ran){
      _start = millis();
      while (digitalRead(taster[0]) == 1) {}
      while (digitalRead(taster[0]) == 0) {}

      result = millis() - _start;
      return result;
    }

    delay(1200);
  }
}

//Test/////////////////////////////
void test_main(){
  float GesamtD   = 0;
  float LichtD    = 0;
  float TonD      = 0;
  float DisplayD  = 0;

  for (int i = 1; i <= 10; i++){
    LichtD += (float) test_licht();
  }
  LichtD /= 10;
  GesamtD += LichtD;
  delay(1000);

  introduction_tone();
  for (int i = 1; i <= 10; i++){
    TonD += (float) test_ton();
  }
  TonD /= 10;
  GesamtD += TonD*0.9;  //Ton wird etwas weniger stark gewertet, um es in relation mit dem Licht zu setzen
  delay(1000);

  for (int i = 1; i <= 10; i++){
    DisplayD += (float) test_display();
  }
  DisplayD  /= 10;
  GesamtD   += DisplayD;
  GesamtD   /= 3;

  Serial.println((String) LichtD + " - " + (String) TonD);
}
