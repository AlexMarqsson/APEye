//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "ESP8266WiFi.h"
#include <Adafruit_NeoPixel.h> 

enum stateEnum {READ, FETCH, CYCLE};

#define PIN             D5  //Pin used for NeoPixel Ring
#define LEDNR           24  //Nr of Leds in NeoPixel Ring
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define FETCHPIN        D6
#define CYCLEPIN        D7

void stateMachine();
void clearAll();
int scan();
void output();

uint8_t state = READ;
int sigStr = 0;
int idx = -1;
int nrOfAps = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNR, PIN);
//3.3V for OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(FETCHPIN, INPUT);
  pinMode(CYCLEPIN, INPUT);
  strip.begin();
  strip.setBrightness(128);
  strip.show();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  display.setTextSize(2);
  display.setTextColor(WHITE);
  clearAll();
}

void loop() {
  // put your main code here, to run repeatedly:
  stateMachine();
  delay(10);
}

void stateMachine(){
  switch(state){
    case READ:
      if(digitalRead(FETCHPIN) == HIGH){state = FETCH; break;}
      if(digitalRead(CYCLEPIN) == HIGH){state = CYCLE; break;}
      break;

    case FETCH:
      delay(200);
      /*Code for Fetching AP list*/
      Serial.println("Fetching..");
      clearAll();
      nrOfAps = scan();
      idx=-1;
      state = CYCLE;
      break;

    case CYCLE:
      delay(500);
      /*Code for Cycling through AP list*/
      Serial.println("Cycling..");
      if(nrOfAps == 0){state = READ; break;}
      if((idx) >= (nrOfAps-1)){idx = -2;}
      idx++;
      output();
      state = READ;
      break;
  }
}

void clearAll(){
  display.clearDisplay();
  display.setCursor(0,0);
  for(int i = 0; i<LEDNR; i++){
      strip.setPixelColor(i, 0, 0, 0);
  }
  display.display();
  strip.show();
}

int scan(){
  clearAll();
  display.println("Fetching Data");
  display.display();
  return WiFi.scanNetworks();
}

void output(){
  clearAll();
  if(idx+1 != 0){
    display.print("AP: ");
    display.print(idx+1);
    display.print(" /");
    display.println(nrOfAps);
    display.print(WiFi.SSID(idx));
    sigStr = map(WiFi.RSSI(idx), -90, -20, 1, 24);
    Serial.println(WiFi.SSID(idx));
    Serial.print(sigStr);
    for(int i = 0; i<sigStr; i++){
      if(sigStr <= 5){strip.setPixelColor(i, 255, 0, 0);}
      else if(sigStr>=6 && sigStr<12){strip.setPixelColor(i, 255, 255, 0);}
      else{strip.setPixelColor(i, 0, 255, 0);}
    }
  }  
    display.display();
    strip.show();
}
