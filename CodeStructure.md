# Codewiz Mecanum Car 시스템 아키텍처

## 1. 시스템 개요
- KeyeStudio 4WD 메카넘 휠 자동차를 Codewiz 플랫폼과 연동한 제어 시스템
- WizController 앱을 통한 원격 제어 가능
- 전진, 후진, 좌/우 회전, 좌/우 수평이동 등 6가지 동작 모드 지원

## 2. 하드웨어 구성

### 2.1 주요 센서
1. **VL53L0X 거리 센서**
   - I2C 주소: 0x27
   - 최대 타임아웃: 500ms
   - 측정 단위: mm

2. **SPARKFUN LIS2DH12 자이로 센서**
   - I2C 주소: 0x18
   - 3축 자이로스코프
   - 각도 범위: ±90도

3. **마이크 센서**
   - A/D 컨버터: 1320 기준값
   - 출력 범위: 0~1023

4. **조도 센서**
   - A/D 컨버터: 0~4095
   - 출력 범위: 0~1023 (역방향)

### 2.2 제어 시스템
1. **모터 제어**
   - 8개의 DC 모터 사용
   - I2C 통신을 통한 모터 드라이버 제어
   - 속도 제어 범위: 0x00 ~ 0x3F

2. **음향 피드백**
   - 패시브 부저 사용
   - LEDC 타이머: 13비트
   - 기본 주파수: 5000Hz
   - 노트 범위: C~B (12개)

## 3. 소프트웨어 아키텍처

### 3.1 주요 라이브러리
```cpp
#include <Arduino.h>
#include "wizSystem.h"
#include "BluetoothSerial.h"
#include "WizNeopixel.h"
#include <wizOLED.h>
#include <VL53L0X.h>
#include <Wire.h>
#include <SparkFun_LIS2DH12.h>
```

### 3.2 제어 모드
| 모드명 | 비트 패턴 | 설명 |
|--------|-----------|------|
| 정지 | 0x00 | 모든 모터 정지 |
| 전진 | 0b01010101 | 모든 모터 정방향 회전 |
| 후진 | 0b10101010 | 모든 모터 반대방향 회전 |
| 좌회전 | 0b01100110 | 전방 모터만 회전 |
| 우회전 | 0b10011001 | 후방 모터만 회전 |
| 좌로 수평이동 | 0b01101001 | 전방 왼쪽, 후방 오른쪽 모터만 회전 |
| 우로 수평이동 | 0b10010110 | 전방 오른쪽, 후방 왼쪽 모터만 회전 |

### 3.3 센서 데이터
```cpp
String getSensorValue() {
  return String.format("%d,%d,%d,%d,%d,%d",
    getMicValue(),    // 마이크 값
    getLightValue(),  // 조도 값
    getDistance(),    // 거리 센서 값
    getGyroValue(0),  // X축 자이로
    getGyroValue(1),  // Y축 자이로
    getGyroValue(2)   // Z축 자이로
  );
}
```

## 4. 통신 프로토콜

### 4.1 블루투스 설정
```cpp
BluetoothSerial bt;
void setup() {
  bt.begin("BrCar");  // 블루투스 장치명
}
```

### 4.2 타임아웃 처리
```cpp
#define TIMEOUT_MS 500
unsigned long lastReceiveTime = 0;

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastReceiveTime > TIMEOUT_MS) {
    runWheel(Stop);  // 타임아웃 시 정지
    WheelSpeed = 0;
  }
}
```

## 5. 오류 처리

### 5.1 I2C 통신 오류
```cpp
void runWheel(uint8_t ActNumber) {
  for (int i = 0; i < 8; i++) {
    int result = Wire.endTransmission();
    if (result != 0) {
      WheelSpeed = 0;
      return;
    }
  }
}
```

### 5.2 센서 초기화 오류
```cpp
void setup() {
  if (!Wire.begin()) {
    delay(1000);  // I2C 초기화 실패 시 1초 대기
  }
}
```

## 6. 주요 함수 설명

### 6.1 모터 제어
```cpp
void runWheel(uint8_t ActNumber) {
  // 8개 모터 각각 제어
  // I2C 주소: 0x30
  // 속도 제어: 0x00 ~ 0x3F
}
```

### 6.2 센서 데이터 처리
```cpp
uint16_t getDistance() {
  // 거리 센서 읽기
  // 타임아웃 체크
}

uint16_t getMicValue() {
  // 마이크 값 읽기
  // 범위: 0~1023
}

uint16_t getLightValue() {
  // 조도 값 읽기
  // 역방향 맵핑
}
```

### 6.3 음향 피드백
```cpp
void makeTone() {
  // C, E, G 음계 재생
  // 각 음符: 1/8박자
}
```

## 7. 주의사항
1. 모든 센서 초기화 시 타임아웃 처리 필요
2. I2C 통신 오류 발생 시 모터 정지 처리
3. 블루투스 연결 타임아웃 설정 (500ms)
4. 모터 속도 제어 범위: 0x00 ~ 0x3F
5. 조도 센서는 역방향 맵핑 처리 필요
