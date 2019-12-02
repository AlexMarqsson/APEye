#include "ESP8266WiFi.h"
#include <Adafruit_Neopixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PIN D5

Adafruit_SSD1306 display(-1);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN);

void setup() {
  Serial.begin(115200);
  strip.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.clearDisplay();
  strip.setBrightness(128);
  strip.show();
}
 
void loop() {
  int numberOfNetworks = WiFi.scanNetworks();
  for(int i =0; i<numberOfNetworks; i++){
      display.clearDisplay();
      display.print("Network name: ");
      display.println(WiFi.SSID(i));
      display.display();
      //FIX RSSI MAPPING TO LED RING VALUES!
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));
      Serial.println("-----------------------");
      delay(8000);
  }  
  delay(2000);
}
