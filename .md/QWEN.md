# QWEN.md - CodeWiz Mecanum Car Project

## Project Overview

This is an ESP32-based mecanum wheel car control system for the KeyeStudio 4WD mecanum wheel car, integrated with the Codewiz platform. The system provides 6-directional movement control (forward, backward, turn left/right, slide left/right) through Bluetooth communication using the WizController app. The project supports multiple sensors including distance, gyroscope, microphone, and light sensors.

## Hardware Configuration

### Main Controller
- **Platform**: ESP32-based Codewiz board
- **Communication**: Bluetooth serial connection

### Sensors and Peripherals
1. **VL53L0X Distance Sensor**
   - I2C Address: 0x27
   - Timeout: 500ms
   - Unit: Millimeters
   - Max Range: Configurable via signal rate limit

2. **LIS2DH12 Gyroscope/Accelerometer** 
   - I2C Address: 0x18
   - 3-axis digital accelerometer
   - Range: ±90 degrees

3. **Microphone Sensor**
   - ADC Pin: 36 (original) / 35 (alternative)
   - Reference Value: 1320
   - Output Range: 0-1023

4. **Light Sensor**
   - ADC Pin: 39 (original) / 34 (alternative) 
   - A/D Converter Range: 0-4095
   - Output Range: 0-1023 (inverted mapping)

5. **Motor Driver**
   - I2C Address: 0x30
   - Controls 8 DC motors independently
   - Speed Range: 0x00 (stop) to 0x3F (max speed)

6. **Passive Buzzer**
   - Pin: 25
   - LEDC Timer: 13-bit
   - Base Frequency: 5000Hz

7. **RGB LEDs and OLED Display**
   - Controlled through wizSystem framework
   - Integrated with sensor and motor controls

## Software Architecture

### File Structure
- `CodewizMecanumCar.ino` - Arduino sketch entry point (main loop)
- `CodewizMecanumCar.h/cpp` - Object-oriented class implementation
- `esp32_mecanum_car.ino` - Standalone implementation 
- `MaqueenPlus.h/cpp/ino` - Alternative robot platform implementation
- `CodeStructure.md` - Detailed system architecture documentation
- `CLAUDE.md` - Development guidelines for AI assistants
- `MaqueenPlus_I2C_Protocol.md` - I2C communication protocol documentation

### Core Classes
- `CodewizMecanumCar` - Primary car control class handling sensors, movement, and Bluetooth communication
- `MaqueenPlus` - Alternative implementation for different robot platform

### Movement Control Patterns
The system uses 8-bit binary patterns to control the 8 motors simultaneously:

| Mode | Binary Pattern | Description |
|------|----------------|-------------|
| Stop | 0x00 | All motors stopped |
| Forward | 0b01010101 | All wheels forward |
| Backward | 0b10101010 | All wheels reverse |
| Turn Left | 0b01100110 | Front wheels turn, rear hold |
| Turn Right | 0b10011001 | Rear wheels turn, front hold |
| Slide Left | 0b01101001 | Diagonal wheel pairs move |
| Slide Right | 0b10010110 | Opposite diagonal wheel pairs move |

### Bluetooth Communication Protocol
- **Device Name**: "BrCar" or "CodewizMecanumCar"
- **Timeout**: 500ms (auto-stop if no command received)
- **Commands**:
  - Speed Control: '0'-'9', 'q' (sets wheel speed)
  - Movement: 'S'(stop), 'F'(forward), 'B'(backward), 'L'(turn left), 'R'(turn right), 'G'/'H'(slide left), 'I'/'J'(slide right)
  - Lights: 'W'(white), 'w'(off)
  - OLED: 'X'(circle), 'x'(clear)
  - Sound: 'V'(play tone sequence)
  - Sensor Data: 'O'(send CSV sensor values)

### Sensor Data Format
CSV string format: `microphone,light,distance,gyroX,gyroY,gyroZ`

## Key Dependencies
```cpp
#include "wizSystem.h"         // Codewiz platform framework
#include "BluetoothSerial.h"   // ESP32 Bluetooth communication
#include "WizNeopixel.h"       // RGB LED control
#include <wizOLED.h>           // OLED display functions
#include <VL53L0X.h>           // Time-of-flight distance sensor
#include <Wire.h>              // I2C communication
#include <SparkFun_LIS2DH12.h> // Accelerometer/gyroscope
```

## Building and Running

This project uses the standard Arduino framework for ESP32:

### Prerequisites
- Arduino IDE with ESP32 board support
- Required libraries: BluetoothSerial, VL53L0X, SparkFun LIS2DH12, Wire, wizSystem, WizNeopixel, wizOLED

### To Build and Upload:
1. Open the main .ino file (either `CodewizMecanumCar.ino` or `esp32_mecanum_car.ino`) in Arduino IDE
2. Select "ESP32 Dev Module" as the board
3. Configure appropriate COM port
4. Upload to device

### Alternative: PlatformIO
If a `platformio.ini` file exists (to be added):
```bash
pio run --target upload
```

## Development Guidelines

### Coding Style
- Follow Arduino/ESP32 C++ conventions
- Use consistent naming for sensor and motor control functions
- Include error checking for I2C communications
- Implement timeout handling for Bluetooth communication

### Testing Practices
- Validate sensor readings before using in logic
- Test all movement patterns independently
- Verify Bluetooth command processing
- Check auto-stop functionality on timeout

### Error Handling
- I2C communication failures should trigger immediate motor stop
- All sensor reads should validate responses
- Sensor initialization should include timeout handling
- Bluetooth commands should have default cases in switch statements

## Important Notes
- Motor control uses I2C address 0x30 for communication with the driver
- The light sensor outputs inverted values (higher ADC = lower light level)
- Maximum motor speed is 0x3F (63 in decimal)
- All sensor data is formatted as a comma-separated string for transmission
- The passive buzzer uses LEDC for tone generation at 5000Hz base frequency