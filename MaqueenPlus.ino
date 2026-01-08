#include <Wire.h>

#define adr 0x10

class MaqueenPlus {
public:
  //-- 모터
  const int MT_L = 0;
  const int MT_R = 1;

  //-- 서보 모터
  const int S1 = 1;
  const int S2 = 2;
  const int S3 = 3;

  //-- RGB LED
  const int RGB_L = 1;
  const int RGB_R = 2;
  const int RGB_ALL = 3;
  const int RED = 1;
  const int GREEN = 2;
  const int BLUE = 4;
  const int YELLOW = 3;
  const int PINK = 5;
  const int CYAN = 6;
  const int WHITE = 7;
  const int OFF = 8;

  //-- 라인트랙 센서
  struct Patrol {
    int L1 = 0x04;
    int L2 = 0x02;
    int L3 = 0x01;
    int R1 = 0x08;
    int R2 = 0x10;
    int R3 = 0x20;
  };
  Patrol patrol;

  MaqueenPlus()
  {
    Wire.begin();
  }


  void motorControl(int mot, int dir, int spd)
  {
    byte buf[3];
    if (mot == MT_L)
      buf[0] = 0x00;
    else
      buf[0] = 0x02;
    buf[1] = dir;
    buf[2] = spd;
    Wire.beginTransmission(adr);
    Wire.write(buf, 3);
    Wire.endTransmission();
  }


  void go(int dL, int sL, int dR, int sR)
  {
    motorControl(MT_L, dL, sL);
    motorControl(MT_R, dR, sR);
  }


  void servo(int num, int angle)
  {
    byte buf[3];
    if (num == S1)
      buf[0] = 0x14;
    else if (num == S2)
      buf[0] = 0x15;
    else
      buf[0] = 0x16;
    buf[1] = angle;
    Wire.beginTransmission(adr);
    Wire.write(buf, 2);
    Wire.endTransmission();
  }


  void RGBLight(int rgbshow, int color)
  {
    byte buf[3];
    buf[0] = 0x0B;
    buf[1] = color;
    if (rgbshow == RGB_R)
      buf[0] = 0x0C;
    else if (rgbshow == RGB_ALL)
      buf[2] = color;
    Wire.beginTransmission(adr);
    Wire.write(buf, 3);
    Wire.endTransmission();
  }


  void stop()
  {
    go(1, 0, 1, 0);
  }


  void move(String dir, int spd)
  {
    if (dir == "F")
      go(1, spd, 1, spd);
    else if (dir == "L")
      go(1, 0, 1, spd);
    else if (dir == "R")
      go(1, spd, 1, 0);
    else if (dir == "B")
      go(2, spd, 2, spd);
  }


  //-- The goto function is not directly portable due to the encoder implementation.
  //-- It requires further adaptation based on the specific hardware and encoder setup.
  /*
  void goto(String dir, int spd, int dst) {
    int en = getEncoders(); // This needs to be adapted for Arduino
    int goal = dst;
    if (dir == "F") {
      goal += en[0];
      while (en[0] < goal) {
        go(1, spd, 1, spd);
        en = getEncoders(); // This needs to be adapted for Arduino
      }
    }
    if (dir == "L") {
      goal += en[1];
      while (en[1] < goal) {
        go(1, 0, 1, spd);
        en = getEncoders(); // This needs to be adapted for Arduino
      }
    } else if (dir == "R") {
      goal += en[0];
      while (en[0] < goal) {
        go(1, spd, 1, 0);
        en = getEncoders(); // This needs to be adapted for Arduino
      }
    }
    stop();
  }
  */

  //-- The ultrasonic function requires specific pin configurations and timing.
  //-- It needs to be adapted based on the ESP32's capabilities and pin assignments.
  /*
  float ultrasonic(float maxDist = 0.4) {
    // pins: trig=2, echo=8
    digitalWrite(2, HIGH);
    delayMicroseconds(10);
    digitalWrite(2, LOW);
    //pin8.read_digital(); // This is not a standard Arduino function
    long duration = pulseIn(8, HIGH); // This needs to be tested and verified
    float dst = maxDist;
    if (duration > 0) {
      dst = 340.29 * (duration / (2 * 1000000.0));
    } else {
      dst = maxDist;
    }
    return dst;
  }
  */

  //-- The getLine function requires I2C communication and bitwise operations.
  //-- It needs to be adapted based on the specific sensor setup and data format.
  /*
  PatrolSensors getLine() {
    Wire.beginTransmission(adr);
    Wire.write(0x1D);
    Wire.endTransmission();
    Wire.requestFrom(adr, 1);
    byte patrol_y = Wire.read();
    PatrolSensors sens;
    sens.L1 = ((patrol_y & patrol.L1) == patrol.L1) ? 1 : 0;
    sens.L2 = ((patrol_y & patrol.L2) == patrol.L2) ? 1 : 0;
    sens.L3 = ((patrol_y & patrol.L3) == patrol.L3) ? 1 : 0;
    sens.R1 = ((patrol_y & patrol.R1) == patrol.R1) ? 1 : 0;
    sens.R2 = ((patrol_y & patrol.R2) == patrol.R2) ? 1 : 0;
    sens.R3 = ((patrol_y & patrol.R3) == patrol.R3) ? 1 : 0;
    return sens;
  }
  */

  //-- The getEncoders function requires I2C communication and data unpacking.
  //-- It needs to be adapted based on the specific encoder setup and data format.
  /*
  int getEncoders() {
    byte buf[1];
    buf[0] = 0x04;
    Wire.beginTransmission(adr);
    Wire.write(buf, 1);
    Wire.endTransmission();
    Wire.requestFrom(adr, 4);
    byte encoderData[4];
    for (int i = 0; i < 4; i++) {
      encoderData[i] = Wire.read();
    }
    // Implement the data unpacking logic here based on the encoder data format
    // For example:
    // int encoderValue = (encoderData[0] << 24) | (encoderData[1] << 16) | (encoderData[2] << 8) | encoderData[3];
    return 0; // Replace with the actual encoder value
  }
  */

  void clearEncoders()
  {
    byte buf[5];
    buf[0] = 0x04;
    buf[1] = buf[2] = buf[3] = buf[4] = 0x00;
    Wire.beginTransmission(adr);
    Wire.write(buf, 5);
    Wire.endTransmission();
  }
};


MaqueenPlus maqueen;


void setup()
{
  Wire.begin();
}


void loop()
{
  //-- put your main code here, to run repeatedly:
}
