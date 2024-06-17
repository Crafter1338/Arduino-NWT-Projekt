#include "Wire.h"
////Einstellungen//////////////////
const unsigned char farben[3][3]  = { {255,0,0}, {0,255,0}, {0,0,255} };
const unsigned int frequenzen[3]  = {80, 400, 1200};
////Pins///////////////////////////
const unsigned char rgbLed[3]     = {3, 9, 6}; 
const unsigned char speaker       = 2;
const unsigned char taster[3]     = {5, 8, 7};
const unsigned char normaleLeds[3]        = {10, 11, 12};
int x = 0;
////Setup//////////////////////////
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A1));
  
  pinMode(taster[0], INPUT_PULLUP);
  pinMode(taster[1], INPUT_PULLUP);
  pinMode(taster[2], INPUT_PULLUP);
  pinMode(normaleLeds[0], OUTPUT);
  pinMode(normaleLeds[1], OUTPUT);
  pinMode(normaleLeds[2], OUTPUT);
  noTone(speaker);

  Wire.begin(9); 
  Wire.onReceive(receiveEvent);
  test_main();
}

void receiveEvent(int bytes) {
  x = Wire.read();
}

void loop() {
  if (x == 1){
    digitalWrite(normaleLeds[0], HIGH);
    digitalWrite(normaleLeds[1], HIGH);
    digitalWrite(normaleLeds[2], HIGH);
  }else{
    digitalWrite(normaleLeds[0], LOW);
    digitalWrite(normaleLeds[1], LOW);
    digitalWrite(normaleLeds[2], LOW);
  }
}
            
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

void introduction_led(){
  set_rgb(rgbLed, farben[0]);
  digitalWrite(normaleLeds[0], HIGH);
  delay(1200);
  
  digitalWrite(normaleLeds[0], LOW);
  digitalWrite(normaleLeds[1], HIGH);
  set_rgb(rgbLed, farben[1]);
  delay(1200);

  digitalWrite(normaleLeds[1], LOW);
  digitalWrite(normaleLeds[2], HIGH);
  set_rgb(rgbLed, farben[2]);
  delay(1200);
  
  digitalWrite(normaleLeds[2], LOW);
  clear_rgb(rgbLed);
  delay(1000);
}

////Ton////////////////////////////
void introduction_tone(){
  tone(speaker, frequenzen[0]);
  digitalWrite(normaleLeds[0], HIGH);
  delay(1200);
  
  digitalWrite(normaleLeds[0], LOW);
  digitalWrite(normaleLeds[1], HIGH);
  tone(speaker, frequenzen[1]);
  delay(1200);

  digitalWrite(normaleLeds[1], LOW);
  digitalWrite(normaleLeds[2], HIGH);
  tone(speaker, frequenzen[2]);
  delay(1200);

  digitalWrite(normaleLeds[2], LOW);
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

//Test/////////////////////////////
void test_main(){
  float GesamtD   = 0;
  float LichtD    = 0;
  float TonD      = 0;

  introduction_led();
  for (int i = 1; i <= 6; i++){
    LichtD += (float) test_licht();
  }
  LichtD /= 2;
  GesamtD += LichtD;
  delay(1000);

  introduction_tone();
  for (int i = 1; i <= 6; i++){
    TonD += (float) test_ton();
  }
  TonD /= 2;
  GesamtD += TonD*0.9; //Ton wird etwas weniger stark gewertet, um es in relation mit dem Licht zu setzen
  delay(1000);
  
  Wire.beginTransmission(9);
  Wire.write((int) round(GesamtD));
  Wire.endTransmission();
  
  Serial.println((String) LichtD + " - " + (String) TonD);
}
