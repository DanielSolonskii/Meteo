#include <LiquidCrystalRus.h>
#include <Adafruit_NeoPixel.h>
#include "DHT.h"
#define NUMPIXELS 11 // Кол-во светодиодов
#define LED_PIN 6
#define analogPin A0
#define DHTPIN 7
DHT dht(DHTPIN, DHT22); //Определяем датчик температуры(В нашем случае DHT22)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
float HIPPM = 200;
bool isHIPPM = false;
float LOTEMP = 20; 
float HITEMP = 30;
int black[3]  = { 0, 0, 0 };
int white[3]  = { 100, 100, 100 };
int red[3]    = { 100, 0, 0 };
int green[3]  = { 0, 100, 0 };
int blue[3]   = { 0, 0, 100 };
int yellow[3] = { 40, 95, 0 };
int dimWhite[3] = { 30, 30, 30 };
int redVal = black[0];
int grnVal = black[1]; 
int bluVal = black[2];
int wait = 10;
int hold = 0;      
int DEBUG = 0;      
int loopCount = 60; 
int repeat = 3;     
int j = 0;         
int prevR = redVal;
int prevG = grnVal;
int prevB = bluVal;
LiquidCrystalRus lcd(12, 10, 11, 5, 4, 3, 2);

void setup()
{
  Serial.println("LCD INIT!");
  lcd.begin(8, 2);
  Serial.println("LCD BEGIN!");
  Serial.println("DHT INIT!");
  dht.begin();
  Serial.println("DHT BEGIN!");
  strip.begin();
    Serial.begin(9600); 
  Serial.println("--------------------------METEO V1.0------------------------------------------------");
  Serial.println("-------------------Author: Daniil Solonskiy-----------------------------------------");
  Serial.println("------------scientific director: Alexandr Sergeevich--------------------------------");
  Serial.println("-----------------------Thanks For Using!)-------------------------------------------");
  crossFade(red, 1);
  crossFade(green, 1);
  crossFade(blue, 1);

  
}

void loop()
{

  float t = dht.readTemperature();
  lcd.print("t=");
  lcd.setCursor(2,0);
  lcd.print(t);
  float ppm =  analogRead(analogPin);
  lcd.setCursor(0,1);                
  lcd.print("ppm=");
  lcd.setCursor(4,1);
  lcd.print(ppm); 
  Serial.println(t);
  Serial.println(ppm);          
  //crossFade(red);
         if(t >= HITEMP)crossFade(red, 10);       
         if(t <= LOTEMP)crossFade(blue, 10);
         if(t > LOTEMP && t < 27)crossFade(green, 10);
         if(t > LOTEMP && t>27 && t<HITEMP)crossFade(yellow, 10);
  while(ppm >= HIPPM){
    isHIPPM = true;
    ppm =  analogRead(analogPin);
    Serial.println(ppm);
    for(int i=0; i < NUMPIXELS; i++ ){
       strip.setPixelColor(i, strip.Color(255, 0, 0));// - Закрашиваем светодиоды Красный
       strip.show();
       delay(100);
       strip.setPixelColor(i++, strip.Color(255, 255, 255)); // - Закрашиваем светодиоды Желтый
       strip.show();
       delay(100);
    }
  }
//------------------------------------------------------



  
  //crossFade(green);
  //crossFade(blue);
  //crossFade(yellow);
  lcd.clear();
}

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = 1020/step;              //   divide by 1020
  } 
  return step;
}



int calculateVal(int step, int val, int i) {

  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;           
    } 
    else if (step < 0) {        
      val -= 1;
    } 
  }
  if (val > 255) {
    val = 255;
  } 
  else if (val < 0) {
    val = 0;
  }
  return val;
}



void crossFade(int color[3], int temp) {
   float t = dht.readTemperature();
  // Convert to 0-255
  Serial.println(t);
  int R = (color[0] * 255) / 100;
  int G = (color[1] * 255) / 100;
  int B = (color[2] * 255) / 100;

  int stepR = calculateStep(prevR, R);
  int stepG = calculateStep(prevG, G); 
  int stepB = calculateStep(prevB, B);

  for (int i = 0; i <= 1020; i++) {
    redVal = calculateVal(stepR, redVal, i);
    grnVal = calculateVal(stepG, grnVal, i);
    bluVal = calculateVal(stepB, bluVal, i);
    for(int f = 0; f < 11; f++){
      strip.setPixelColor(f, strip.Color(redVal, grnVal, bluVal));// - Закрашиваем светодиоды Красный
    }
    strip.show();
      
    delay(temp); // Pause for 'wait' milliseconds before resuming the loop

    if (DEBUG) { // If we want serial output, print it at the 
      if (i == 0 or i % loopCount == 0) { // beginning, and every loopCount times
        Serial.print("Loop/RGB: #");
        Serial.print(i);
        Serial.print(" | ");
        Serial.print(redVal);
        Serial.print(" / ");
        Serial.print(grnVal);
        Serial.print(" / ");  
        Serial.println(bluVal); 
        
      } 
      DEBUG += 1;
    }
  }
  // Update current values for next loop
  prevR = redVal; 
  prevG = grnVal; 
  prevB = bluVal;
  delay(hold); // Pause for optional 'wait' milliseconds before resuming the loop
}
