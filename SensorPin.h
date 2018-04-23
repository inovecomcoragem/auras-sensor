class SensorPin {
  private:
    const static short NUM_SAMPLES_FAST = 100;
    const static short NUM_SAMPLES_SLOW = 800;
    const static short THRESHOLD_DIFFERENCE = 32;
    const static int SAMPLE_PERIOD_CLOCK = 10 * 80e3;

    short pin;
    short sampleFast[NUM_SAMPLES_FAST];
    short sampleSlow[NUM_SAMPLES_SLOW];
    int sumFast;
    int sumSlow;
    short currentIndexFast;
    short currentIndexSlow;
  public:
    SensorPin(short _pin);
    int getReading();
    int getReadingRaw();
    int getSlowAverage();
    void getSample();
};

