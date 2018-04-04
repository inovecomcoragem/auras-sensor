#include <Arduino.h>
#include "SensorPin.h"

SensorPin::SensorPin(short _pin) {
  pin = _pin;
  for (currentIndex = 0; currentIndex < NUM_SAMPLES; currentIndex++) {
    sample[currentIndex] = analogRead(pin);
    sum += sample[currentIndex];
  }
  currentIndex = 0;
  calibrate(3);
}

void SensorPin::calibrate() {
  calibrate(1);
}

void SensorPin::getSample() {
  sum -= sample[currentIndex];
  sample[currentIndex] = analogRead(pin);
  sum += sample[currentIndex];
  currentIndex = (currentIndex + 1) % NUM_SAMPLES;
}

void SensorPin::calibrate(short _numRounds) {
  noInterrupts();
  for (int round = 0; round < _numRounds * NUM_SAMPLES; round++) {
    getSample();
    delay(10);
  }
  averageThreshold = sum / NUM_SAMPLES;
  interrupts();
}

int SensorPin::getReadingRaw() {
  return sum / NUM_SAMPLES;
}

int SensorPin::getReading() {
  return ((getReadingRaw() - averageThreshold) > THRESHOLD_DIFFERENCE);
}

