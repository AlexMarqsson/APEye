//OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "ESP8266WiFi.h"
#include <Adafruit_NeoPixel.h> 

#define PIN             D5  //Pin used for NeoPixel Ring
#define LEDNR           24  //Nr of Leds in NeoPixel Ring
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1 // Reset pin # (or -1 if sharing Arduino reset pin)

int sigStr = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNR, PIN);
//3.3V for OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  for(int i = 0; i<LEDNR; i++){
      strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
  int numberOfNetworks = WiFi.scanNetworks();
  
  for(int i =0; i<numberOfNetworks; i++){
      display.clearDisplay();
      display.setCursor(0,0);
      Serial.print("Number of AP:s: ");
      Serial.println(numberOfNetworks);
      Serial.print("Network name: ");
     // Serial.println(WiFi.SSID(i));
      display.println(WiFi.SSID(i));
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));
      sigStr = map(WiFi.RSSI(i), -90, -20, 1, 24);
      Serial.println("-----------------------");
      for(int i = 0; i<sigStr; i++){
        if(sigStr <= 5){strip.setPixelColor(i, 255, 0, 0);}
        else if(sigStr>=6 && sigStr<12){strip.setPixelColor(i, 255, 255, 0);}
        else{strip.setPixelColor(i, 0, 255, 0);}
      }
      display.display();
      strip.show();
      delay(4000);
      for(int i = 0; i<LEDNR; i++){
        strip.setPixelColor(i, 0, 0, 0);
      }
      strip.show();
  }  
  delay(2000);

}
