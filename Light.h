#include <Adafruit_NeoPixel.h>

#define LED_PIN 5
#define NUMPIXELS 24

class Light {
  private:
    int mBrightness;
    Adafruit_NeoPixel *mPixels;
    void setPixelsColor(int color);
  public:
    Light();
    void setColor(float colorPercent);
};


