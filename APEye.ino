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
void nullFound();
void clearAll();
int scan();
void output();
void booted();

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
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  // Setting pin mode
  pinMode(FETCHPIN, INPUT);
  pinMode(CYCLEPIN, INPUT);

  // Initiate NeoPixel
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

  // Clear OLED and let user know that boot sequence is finished
  clearAll();
  booted();
}

void loop() {
  // put your main code here, to run repeatedly:
  stateMachine();
  delay(10);
}

void stateMachine(){
  switch(state){
    case READ:
      /*Default state which checks for actions*/
      if(digitalRead(FETCHPIN) == HIGH){state = FETCH; break;}
      if(digitalRead(CYCLEPIN) == HIGH){state = CYCLE; break;}
      break;

    case FETCH:
      delay(200);
      /*Code for Fetching AP list*/
      clearAll();
      nrOfAps = scan();
      idx=-1;
      state = CYCLE;
      break;

    case CYCLE:
      delay(500);
      /*Code for Cycling through AP list*/
      if(nrOfAps == 0){state = READ;
        nullFound(); 
        break;
      }
      if((idx) >= (nrOfAps-1)){idx = -2;}
      idx++;
      output();
      state = READ;
      break;
  }
}

// Function handeling the case of scan() returning 0
void nullFound(){
  display.println("Nothing");
  display.println("Found..");
  display.display();
  delay(2 000);
  clearAll();
}

// Function clearing NeoPixel and OLED
void clearAll(){
  display.clearDisplay();
  display.setCursor(0,0);
  for(int i = 0; i<LEDNR; i++){
      strip.setPixelColor(i, 0, 0, 0);
  }
  display.display();
  strip.show();
}

// Function returning number of networks found while scanning
int scan(){
  clearAll();
  WiFi.scanDelete();
  display.println("Fetching"); 
  display.println("Data");
  display.display();
  return WiFi.scanNetworks();
}

// Function outputting data using NeoPixel and OLED
void output(){
  clearAll();
  if(idx+1 != 0){
    display.print("AP: ");
    display.print(idx+1);
    display.print("/");
    display.println(nrOfAps);
    display.print(WiFi.SSID(idx));
    sigStr = map(WiFi.RSSI(idx), -100, 0, 1, 24);
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

// Function to indicate that boot is finished
void booted(){
  for(int i = 0; i<3; i++){
    for(int i = 0; i<LEDNR; i++){
        strip.setPixelColor(i, 0, 0, 255);
    }
    strip.show();
    delay(500);
    clearAll();
    delay(200);
  }
}
