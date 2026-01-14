
//-- 단일 파일로 라이브러리 없이 MecanumWheel car 구동 가능
//-- 나중에 라이브러리 별도로 만들어서 적용 하자

#include <Arduino.h>
#include "wizSystem.h"
#include "BluetoothSerial.h"
#include "WizNeopixel.h"
#include <wizOLED.h>
#include <VL53L0X.h>
#include <Wire.h>
#include <SparkFun_LIS2DH12.h>

#define LEDC_CHANNEL_0 (6)
#define LEDC_TIMER_13_BIT (13)
#define LEDC_BASE_FREQ (5000)
#define BOARD_PASSIVEBUZZER_PIN (25)

#define I2C_SLAVE_ADDR 0x30
#define TIMEOUT_MS 500
#define MAX_GYRO_VALUE 90
#define MAX_MIC_VALUE 1320
#define MAX_LIGHT_VALUE 4095

note_t note[12] = {
  NOTE_C, NOTE_Cs, NOTE_D, NOTE_Eb, NOTE_E, 
  NOTE_F, NOTE_Fs, NOTE_G, NOTE_Gs, NOTE_A, 
  NOTE_Bb, NOTE_B
};

const uint8_t Stop = 0;
const uint8_t Forward = 0b01010101;
const uint8_t Backward = 0b10101010;
const uint8_t TurnLeft = 0b01100110;
const uint8_t TurnRight = 0b10011001;
const uint8_t SlideLeft = 0b01101001;
const uint8_t SlideRight = 0b10010110;
uint8_t WheelSpeed = 0x3F;

VL53L0X DistanceSen;
SPARKFUN_LIS2DH12 accel;
BluetoothSerial bt;
unsigned long lastReceiveTime = 0;

void driveWheel(uint8_t ActNumber) {
  for (int i = 0; i < 8; i++) {
    Wire.beginTransmission(I2C_SLAVE_ADDR);
    Wire.write(i + 1);
    Wire.write((ActNumber & (0x80 >> i)) ? WheelSpeed : 0);
    
    int result = Wire.endTransmission();
    if (result != 0) {
      WheelSpeed = 0;
      return;
    }
  }
}

uint16_t getDistance() {
  if (DistanceSen.timeoutOccurred() == 0) {
    return DistanceSen.readRangeSingleMillimeters();
  } else {
    return 0;
  }
}

uint16_t getMicValue() {
  uint16_t _value = abs(analogRead(35) - MAX_MIC_VALUE);
  _value = constrain(_value, 0, MAX_MIC_VALUE);
  _value = map(_value, 0, MAX_MIC_VALUE, 0, 1023);
  return _value;
}

uint16_t getLightValue() {
  uint16_t _value = constrain(analogRead(34), 0, MAX_LIGHT_VALUE);
  _value = map(_value, 0, MAX_LIGHT_VALUE, 1023, 0);
  return _value;
}

int16_t getDigitalGyroRawValue(int num) {
  if (!accel.available()) {
    return 0;
  }
  switch (num) {
    case 0: return accel.getRawX() / 181;
    case 1: return accel.getRawY() / 181;
    case 2: return accel.getRawZ() / 181;
    default: return 0;
  }
}

int8_t getGyroValue(int num) {
  while (!accel.available());
  int16_t _value = getDigitalGyroRawValue(num);
  return constrain((int8_t)_value, -MAX_GYRO_VALUE, MAX_GYRO_VALUE);
}

String getSensorValue() {
  static char buffer[64];
  snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d,%d,%d",
    getMicValue(),
    getLightValue(),
    getDistance(),
    getGyroValue(0),
    getGyroValue(1),
    getGyroValue(2));
  return String(buffer);
}

void setup() {
  wizSystem_Init();
  bt.begin("MecanumCar");
  wizPixels.init();
  OLED_init();
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(BOARD_PASSIVEBUZZER_PIN, LEDC_CHANNEL_0);
  
  OLED_clear();
  WheelSpeed = 0;

  if (!Wire.begin()) {
    delay(1000);  
  }

  accel.begin(0x18, Wire);
  DistanceSen.setAddress(0x27);
  DistanceSen.setTimeout(500);
  DistanceSen.init();
  DistanceSen.setSignalRateLimit(0.1);
  DistanceSen.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  DistanceSen.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
  
  lastReceiveTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastReceiveTime > TIMEOUT_MS) {
    driveWheel(Stop);
    WheelSpeed = 0;
    lastReceiveTime = currentTime;
  }

  if (bt.available()) {
    char recvByte = bt.read();
    lastReceiveTime = currentTime;
    
    switch (recvByte) {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        WheelSpeed = (recvByte - '0' + 1) * 10;
        break;
      case 'q': WheelSpeed = 127; break;
      case 'S': driveWheel(Stop); break;
      case 'F': driveWheel(Forward); break;
      case 'B': driveWheel(Backward); break;
      case 'L': driveWheel(TurnLeft); break;
      case 'R': driveWheel(TurnRight); break;
      case 'G': case 'H': driveWheel(SlideLeft); break;
      case 'I': case 'J': driveWheel(SlideRight); break;
      case 'W': wizPixels.setColorAll("#ffffff"); break;
      case 'w': wizPixels.setColorAll(0, 0, 0); break;
      case 'X': OLED_drawCircle(64, 64, 60, 1, 1); break;
      case 'x': OLED_clear(); break;
      case 'V':
        for (int i = 0; i < 3; i++) {
          ledcWriteNote(LEDC_CHANNEL_0, note[i * 3], 4);
          delay((1000 / 8) * 1.3);
          ledcWriteTone(LEDC_CHANNEL_0, 0);
        }
        break;
      case 'O': bt.println(getSensorValue()); break;
      default: break;
    }
  }
}
