#include <Adafruit_NeoPixel.h>

#define PIN D5
#define LEDNR 24

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNR, PIN);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(128);
  strip.show();
}

void loop() {
  delay(5000);
  for(int i = 0; i<LEDNR; i++){
    strip.setPixelColor(i, 0, 255, 0);
  }
  strip.show();
  delay(5000);
  for(int i = 0; i<LEDNR; i++){
    strip.setPixelColor(i, 255, 0, 0);
  }
  strip.show();
  delay(5000);
  for(int i = 0; i<LEDNR; i++){
    strip.setPixelColor(i, 0, 0, 255);
  }
  strip.show();
  // put your main code here, to run repeatedly
}
