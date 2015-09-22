#include <RBD_Timer.h>
#include <RBD_Capacitance.h>
#include <RBD_Threshold.h>
#include <RBD_HumanSensor.h>

#define BAUD 115200

RBD::Timer timer;
RBD::HumanSensor human(1,0);

void setup() {
  Serial.begin(BAUD);
  timer.setHertz(50);
  human.setNearValue(100);
  human.setTouchValue(120);
  human.setPickupValue(140);
}

void loop() {
  human.update();

  if(timer.isExpired()) {
    timer.restart();

    if(human.isGone()) {
      Serial.print("0. ---    ");
    }
    else if(human.isNear()) {
      Serial.print("1. near   ");
    }
    else if(human.isTouch()) {
      Serial.print("2. touch  ");
    }
    else if (human.isPickup()) {
      Serial.print("3. pickup ");
    }
    else {
      Serial.print("-1. error ");
    }

    Serial.println(human.getValue());
  }
}