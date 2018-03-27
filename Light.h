#include <Adafruit_NeoPixel.h>

class Light {
  private:
    int mBrightness;
    Adafruit_NeoPixel *mPixels;
    void setPixelsColor(int color);
  public:
    Light();
    void setColor(float colorPercent);
};

