#include "ESP8266WiFi.h"
#include <Adafruit_NeoPixel.h>

#define PIN D5
#define LEDNR 24

int sigStr = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNR, PIN);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(128);
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i<LEDNR; i++){
      strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
  int numberOfNetworks = WiFi.scanNetworks();
  for(int i =0; i<numberOfNetworks; i++){
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));
      sigStr = map(WiFi.RSSI(i), -90, -20, 1, 24);
      Serial.println("-----------------------");
      for(int i = 0; i<sigStr; i++){
        if(sigStr <= 5){strip.setPixelColor(i, 255, 0, 0);}
        else if(sigStr>=6 && sigStr<12){strip.setPixelColor(i, 255, 255, 0);}
        else{strip.setPixelColor(i, 0, 255, 0);}
      }
      strip.show();
      delay(4000);
      for(int i = 0; i<LEDNR; i++){
        strip.setPixelColor(i, 0, 0, 0);
      }
      strip.show();
  }  
  delay(2000);

}
