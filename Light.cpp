#include "Light.h"

Light::Light(short _pin, short _numPixels) {
  mBrightness = 0;
  pin = _pin;
  numPixels = _numPixels;
}

void Light::setPixelsColor(int color) {
  uint32_t cColor = Adafruit_NeoPixel::Color(
                      (uint8_t) (color & 0xff),
                      (uint8_t) (color & 0xff),
                      (uint8_t) (color & 0xff));

  for (int i = 0; i < numPixels; i++) {
    mPixels->setPixelColor(i, cColor);
  }
  mPixels->show();
}

void Light::setColor(float colorPercent) {
  int targetBrightness = colorPercent * 128;

  mPixels = new Adafruit_NeoPixel(numPixels, pin, NEO_GRB + NEO_KHZ800);
  mPixels->begin();

  for (int i = 0; i < 256; i += 2) {
    setPixelsColor(map(i, 0, 255, mBrightness, targetBrightness));
    delay(10);
  }
  setPixelsColor(targetBrightness);
  delay(10);

  delete mPixels;
  mBrightness = targetBrightness;
}

