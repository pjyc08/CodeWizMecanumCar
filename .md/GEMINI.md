# Project: CodeWiz Mecanum Car Control System

## Project Overview
This repository contains firmware for controlling a KeyeStudio 4WD Mecanum wheel car, primarily utilizing an ESP32 microcontroller and integrated with the Codewiz platform. The system allows for remote control via the WizController app using Bluetooth communication, enabling 6-directional movement (forward, backward, left/right turn, left/right slide).

The project also includes an alternative or related implementation for the Maqueen Plus robot platform, detailing its specific I2C communication protocols.

**Key Technologies:**
*   **Microcontroller:** ESP32
*   **Framework:** Arduino
*   **Language:** C++
*   **Communication:** Bluetooth Serial, I2C
*   **Sensors:** VL53L0X (distance), SPARKFUN LIS2DH12 (gyro/accelerometer), Microphone, Light Sensor
*   **Actuators:** 8 DC Motors (Mecanum Car), Passive Buzzer, RGB LEDs, Servo Motors (Maqueen Plus)

## Building and Running
This is a standard Arduino project for the ESP32 platform.

**To build and upload using Arduino IDE:**
1.  Open either `CodewizMecanumCar.ino` or `esp32_mecanum_car.ino` in the Arduino IDE.
2.  Select "ESP32 Dev Module" as the board.
3.  Upload the sketch to the ESP32 device.

**Key Libraries (required for compilation and functionality):**
*   `Arduino.h` (Standard Arduino core)
*   `wizSystem.h` (Codewiz platform framework)
*   `BluetoothSerial.h` (ESP32 Bluetooth)
*   `WizNeopixel.h` (NeoPixel control)
*   `wizOLED.h` (OLED display)
*   `VL53L0X.h` (Distance sensor)
*   `Wire.h` (I2C communication)
*   `SparkFun_LIS2DH12.h` (Accelerometer/Gyroscope)

*(Note: If `platformio.ini` is added, PlatformIO can also be used with `pio run --target upload`)*

## Development Conventions

### 1. Code Structure and Modularity
*   The project is organized with `.ino`, `.h`, and `.cpp` files, following common Arduino and C++ practices for modularity.
*   Main control logic for the Mecanum car is likely found in `CodewizMecanumCar.h` and `CodewizMecanumCar.cpp`.
*   Entry points for the Arduino sketches are `CodewizMecanumCar.ino` and `esp32_mecanum_car.ino`.

### 2. Motor Control
*   **Mecanum Car:** Uses bit-pattern motor control for 8 DC motors via I2C address `0x30`.
    *   Speed range: `0x00` to `0x3F` (0-63).
    *   Defined bit patterns for various movements (e.g., `0b01010101` for forward).
*   **Maqueen Plus:** Uses I2C slave address `0x10` with specific register addresses for motor control, servo control, and RGB LEDs.
    *   Motor speed range: `0` to `255`.

### 3. Sensor Integration and Data Handling
*   Multiple sensors are integrated via I2C and ADC.
*   Sensor data (mic, light, distance, gyroX, gyroY, gyroZ) is typically formatted as a comma-separated string.
*   Light sensor values are inverted (higher ADC value means lower light).
*   All sensor initialization and reads include timeout and error handling.

### 4. Communication and Safety
*   Bluetooth Serial communication is used, with the device named "BrCar".
*   A 500ms communication timeout is implemented; if no command is received within this period, the car's motors stop for safety.
*   I2C communication errors are handled by stopping the motors immediately.

### 5. Embedded System Best Practices
*   **Performance and Memory Efficiency:** As a firmware project, performance and memory efficiency are paramount. Code should be optimized for the ESP32's resources.
*   **I2C Communication:** Robust I2C error checking and timeout mechanisms are implemented for reliable sensor and motor control.
*   **Standard Coding Conventions:** C/C++ code should adhere to general embedded systems coding standards, as implied by the Arduino framework usage.
