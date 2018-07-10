#include <Adafruit_NeoPixel.h>

class Light {
  private:
    short pin;
    short numPixels;
    int mBrightness;
    Adafruit_NeoPixel *mPixels;
    void setPixelsColor(int color);
  public:
    Light(short _pin, short _numPixels);
    void setColor(float colorPercent);
};

