#include "CodewizMecanumCar.h"

CodewizMecanumCar::CodewizMecanumCar() : WheelSpeed(0), recvByte(0), lastReceiveTime(0) {
}

void CodewizMecanumCar::begin() {
    bt.begin("CodewizMecanumCar");
    
    Wire.begin();
    Wire.setClock(400000);

    if (accel.begin() == false) {
        Serial.println("Accelerometer not detected.");
        return;
    }

    DistanceSen.setAddress(0x27);
    DistanceSen.setTimeout(500);
    DistanceSen.init();
    DistanceSen.setSignalRateLimit(0.1);
    DistanceSen.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    DistanceSen.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

    lastReceiveTime = millis();
}

void CodewizMecanumCar::loop() {
    unsigned long currentTime = millis();
    if (currentTime - lastReceiveTime > TIMEOUT_MS) {
        runWheel(Stop);
        WheelSpeed = 0;
    }

    if (bt.available()) {
        recvByte = bt.read();
        lastReceiveTime = millis();

        switch (recvByte) {
            case '0': case '1': case '2': case '3': case '4': 
            case '5': case '6': case '7': case '8': case '9':
                WheelSpeed = (recvByte - '0' + 1) * 10;
                break;
            case 'q': 
                WheelSpeed = 127;
                break;
            case 'S': 
                runWheel(Stop);
                break;
            case 'F': 
                runWheel(Forward);
                break;
            case 'B': 
                runWheel(Backward);
                break;
            case 'L': 
                runWheel(TurnLeft);
                break;
            case 'R': 
                runWheel(TurnRight);
                break;
            case 'G': case 'H': 
                runWheel(SlideLeft);
                break;
            case 'I': case 'J': 
                runWheel(SlideRight);
                break;
            case 'W': 
                wizPixels.setColorAll("#ffffff");
                break;
            case 'w': 
                wizPixels.setColorAll(0, 0, 0);
                break;
            case 'X': 
                OLED_drawCircle(64, 64, 60, 1, 1);
                break;
            case 'x': 
                OLED_clear();
                break;
            case 'V': 
                makeTone();
                break;
            
        }
    }
}

void CodewizMecanumCar::runWheel(uint8_t ActNumber) {
    // Implementation from original runWheel function
}

uint16_t CodewizMecanumCar::getDistance() {
    return DistanceSen.readRangeSingleMillimeters();
}

uint16_t CodewizMecanumCar::getMicValue() {
    return analogRead(36);
}

uint16_t CodewizMecanumCar::getLightValue() {
    return analogRead(39);
}


void CodewizMecanumCar::makeTone() {
    // Implementation from original makeTone function
}
