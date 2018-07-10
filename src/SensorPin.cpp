#include <Arduino.h>
#include "SensorPin.h"

SensorPin::SensorPin(short _pin) {
  pin = _pin;
  for (currentIndexSlow = 0; currentIndexSlow < NUM_SAMPLES_SLOW; currentIndexSlow++) {
    sampleSlow[currentIndexSlow] = analogRead(pin);
    sumSlow += sampleSlow[currentIndexSlow];

    if (currentIndexSlow < NUM_SAMPLES_FAST) {
      sampleFast[currentIndexFast] = sampleFast[currentIndexSlow];
      sumFast += sampleFast[currentIndexFast];
      currentIndexFast = (currentIndexFast + 1) % NUM_SAMPLES_FAST;
    }
  }
  currentIndexFast = 0;
  currentIndexSlow = 0;
}

void SensorPin::getSample() {
  sumFast -= sampleFast[currentIndexFast];
  sumSlow -= sampleSlow[currentIndexSlow];

  sampleFast[currentIndexFast] = analogRead(pin);
  sampleSlow[currentIndexSlow] = sampleFast[currentIndexFast];

  sumFast += sampleFast[currentIndexFast];
  sumSlow += sampleSlow[currentIndexSlow];

  currentIndexFast = (currentIndexFast + 1) % NUM_SAMPLES_FAST;
  currentIndexSlow = (currentIndexSlow + 1) % NUM_SAMPLES_SLOW;
}

int SensorPin::getReadingRaw() {
  return sumFast / NUM_SAMPLES_FAST;
}

int SensorPin::getSlowAverage() {
  return sumSlow / NUM_SAMPLES_SLOW;
}

int SensorPin::getReading() {
  return ((getReadingRaw() - getSlowAverage()) > THRESHOLD_DIFFERENCE);
}

