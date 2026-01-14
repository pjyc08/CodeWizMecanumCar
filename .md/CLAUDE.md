# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an ESP32-based mecanum wheel car control system for the KeyeStudio 4WD mecanum wheel car, integrated with the Codewiz platform. The system provides 6-directional movement control through Bluetooth communication using the WizController app.

## Build and Development

This project uses the standard Arduino framework for ESP32. To build and upload:

```bash
# Using Arduino IDE
# 1. Open the .ino file in Arduino IDE
# 2. Select ESP32 Dev Module as board
# 3. Upload to device

# Using PlatformIO (if platformio.ini is added)
pio run --target upload
```

No Makefile or CMakeLists.txt is present - this is a standard Arduino project structure.

## Code Architecture

### Core Components

1. **Main Classes**:
   - `CodewizMecanumCar` - Primary car control class handling all sensors and movement
   - `MaqueenPlus` - Alternative implementation for Maqueen Plus robot platform

2. **Key Files**:
   - `CodewizMecanumCar.ino` - Arduino sketch entry point (minimal)
   - `esp32_mecanum_car.ino` - Complete standalone implementation
   - `CodewizMecanumCar.h/.cpp` - Main car control logic

### Sensor Integration

The system integrates multiple sensors via I2C and ADC:
- **VL53L0X** distance sensor (I2C: 0x27) - Timeout: 500ms
- **LIS2DH12** gyroscope/accelerometer (I2C: 0x18) - ±90° range
- **Microphone** (ADC pin 36) - Range: 0-1023
- **Light sensor** (ADC pin 39) - Range: 0-1023 (inverted)

### Movement Control System

Uses bit-pattern motor control for 8 motors:
- `0x00` - Stop
- `0b01010101` - Forward (all motors forward)
- `0b10101010` - Backward (all motors reverse)
- `0b01100110` - Turn left
- `0b10011001` - Turn right
- `0b01101001` - Slide left
- `0b10010110` - Slide right

Motor speed control range: `0x00` - `0x3F` (0-63)

### Communication Protocol

- **Bluetooth**: Serial communication at standard baud rate
- **Device name**: "BrCar"
- **Safety**: 500ms timeout with automatic stop if no command received

### Key Dependencies

```cpp
#include "wizSystem.h"        // Codewiz platform framework
#include "BluetoothSerial.h"  // ESP32 Bluetooth
#include "WizNeopixel.h"      // NeoPixel control
#include <wizOLED.h>          // OLED display
#include <VL53L0X.h>          // Distance sensor
#include <Wire.h>             // I2C communication
#include <SparkFun_LIS2DH12.h> // Accelerometer
```

### Error Handling

- I2C communication failures trigger immediate motor stop
- Sensor initialization includes timeout handling
- All sensor reads validate response before processing

## Important Notes

- Motor control uses I2C address 0x30 for the motor driver
- Passive buzzer uses LEDC timer 13-bit at 5000Hz base frequency
- All sensor data is formatted as CSV string: `mic,light,distance,gyroX,gyroY,gyroZ`
- Light sensor values are inverted (higher ADC value = lower light)