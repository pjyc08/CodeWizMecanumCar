#ifndef MaqueenPlus_h
#define MaqueenPlus_h

#include <Wire.h>

class MaqueenPlus {
public:
  // Constants
  const int MT_L = 0;
  const int MT_R = 1;
  //... (other constants)

  // Struct Patrol
  struct Patrol {
    int L1 = 0x04;
    int L2 = 0x02;
    int L3 = 0x01;
    int R1 = 0x08;
    int R2 = 0x10;
    int R3 = 0x20;
  };

  // Function prototypes
  void motorControl(int mot, int dir, int spd);
  void go(int dL, int sL, int dR, int sR);
  void servo(int num, int angle);
  void RGBLight(int rgbshow, int color);
  void stop();
  void move(String dir, int spd);
  //... (other function prototypes)
};

#endif  // MaqueenPlus_h
