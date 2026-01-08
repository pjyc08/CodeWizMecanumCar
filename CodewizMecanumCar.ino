//-- KeyeStudio 4WD mecanum car를 codewiz와 연동 시킴.
//-- WizController APP으로 조정할 수 있음.
//-- 전진, 후진, 좌회전, 우회전, 좌로 수평이동, 우로 수평이동 가능.

//-- 마이크로비트에 한정되어 사용되던 메카넘 자동차를 이용하는 것은 좋으나 
//-- 자동차 조립이 난이도가 높은 편이고 가격도 비싸다

#include "CodewizMecanumCar.h"

CodewizMecanumCar car;

void setup() {
    car.begin();
}

void loop() {
    car.loop();
}
