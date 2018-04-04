class SensorPin {
  private:
    const static short NUM_SAMPLES = 100;
    const static short THRESHOLD_DIFFERENCE = 100;
    const static int SAMPLE_PERIOD_CLOCK = 10 * 80e3;
    short pin;
    int sample[NUM_SAMPLES];
    int sum;
    int currentIndex;
    int averageThreshold;
    void calibrate();
    void calibrate(short _numRounds);
  public:
    SensorPin(short _pin);
    int getReading();
    int getReadingRaw();
    void getSample();
};

