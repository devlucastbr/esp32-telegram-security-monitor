#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

class PirSensor {
private:
    int pin;

public:
    PirSensor(int pin);
    void begin();
    bool isMotionDetected();
};

#endif