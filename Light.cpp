#include "Light.h"

Light::Light() {
  mBrightness = 0;
}

void Light::setPixelsColor(int color) {
  uint32_t cColor = Adafruit_NeoPixel::Color(
                      (uint8_t) (color & 0xff),
                      (uint8_t) (color & 0xff),
                      (uint8_t) (color & 0xff));

  for (int i = 0; i < NUMPIXELS; i++) {
    mPixels->setPixelColor(i, cColor);
  }
  mPixels->show();
}

void Light::setColor(float colorPercent) {
  int targetBrightness = colorPercent * 255;

  mPixels = new Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
  mPixels->begin();

  for (int i = 0; i < 256; i++) {
    setPixelsColor(map(i, 0, 255, mBrightness, targetBrightness));
    delay(10);
  }
  delete mPixels;
  mBrightness = targetBrightness;
}

