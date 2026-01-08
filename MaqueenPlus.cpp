#include "MaqueenPlus.h"

MaqueenPlus::MaqueenPlus()
{
  Wire.begin();
}

void MaqueenPlus::motorControl(int mot, int dir, int spd)
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

void MaqueenPlus::go(int dL, int sL, int dR, int sR)
{
  motorControl(MT_L, dL, sL);
  motorControl(MT_R, dR, sR);
}

void MaqueenPlus::servo(int num, int angle)
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

void MaqueenPlus::RGBLight(int rgbshow, int color)
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

void MaqueenPlus::stop()
{
  go(1, 0, 1, 0);
}

void MaqueenPlus::move(String dir, int spd)
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

void MaqueenPlus::clearEncoders()
{
  byte buf[5];

  buf[0] = 0x04;
  buf[1] = buf[2] = buf[3] = buf[4] = 0x00;

  Wire.beginTransmission(adr);
  Wire.write(buf, 5);
  Wire.endTransmission();
}
