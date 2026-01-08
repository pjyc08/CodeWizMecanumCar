#ifndef CODEWIZ_MECANUM_CAR_H
#define CODEWIZ_MECANUM_CAR_H

#include <Arduino.h>
#include "wizSystem.h"
#include "BluetoothSerial.h"
#include "WizNeopixel.h"
#include <wizOLED.h>
#include <VL53L0X.h>
#include <Wire.h>
#include <SparkFun_LIS2DH12.h>

// Constants
#define LEDC_CHANNEL_0 (6)
#define LEDC_TIMER_13_BIT (13)
#define TIMEOUT_MS 500

// Movement types
enum MovementType {
    Stop,
    Forward,
    Backward,
    TurnLeft,
    TurnRight,
    SlideLeft,
    SlideRight
};

class CodewizMecanumCar {
public:
    CodewizMecanumCar();
    void begin();
    void loop();
    void runWheel(uint8_t ActNumber);
    uint16_t getDistance();
    uint16_t getMicValue();
    uint16_t getLightValue();
    int16_t getDigitalGyroRawValue(int num);
    int8_t getGyroValue(int num);
    String getSensorValue();
    void makeTone();

private:
    BluetoothSerial bt;
    VL53L0X DistanceSen;
    SPARKFUN_LIS2DH12 accel;
    uint8_t WheelSpeed;
    char recvByte;
    unsigned long lastReceiveTime;
};

#endif // CODEWIZ_MECANUM_CAR_H
