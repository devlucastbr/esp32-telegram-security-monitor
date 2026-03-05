#include <Arduino.h>
#include "pir_sensor.h"

PirSensor::PirSensor(int pin) {
    this->pin = pin;
}

void PirSensor::begin() {
    pinMode(pin, INPUT);
}

bool PirSensor::isMotionDetected() {
    return digitalRead(pin) == HIGH;
}