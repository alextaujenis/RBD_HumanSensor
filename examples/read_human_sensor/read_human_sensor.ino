// Arduino RBD Human Sensor Library v1.0.2 Example - Calibrate a human sensor.
// https://github.com/alextaujenis/RBD_HumanSensor
// Copyright 2015 Alex Taujenis
// MIT License

#include <RBD_Capacitance.h> // https://github.com/alextaujenis/RBD_Capacitance
#include <RBD_Threshold.h>   // https://github.com/alextaujenis/RBD_Threshold
#include <RBD_HumanSensor.h> // https://github.com/alextaujenis/RBD_HumanSensor

RBD::HumanSensor human_sensor(1, 0); // send, receive

void setup() {
  Serial.begin(115200);
  human_sensor.setNearValue(100);
  human_sensor.setTouchValue(200);
  human_sensor.setPickupValue(300);
}

void loop() {
  human_sensor.update();

  if(human_sensor.onGone()) {
    Serial.print("0. GONE   ");
  }
  else if(human_sensor.onNear()) {
    Serial.print("1. NEAR   ");
  }
  else if(human_sensor.onTouch()) {
    Serial.print("2. TOUCH  ");
  }
  else if(human_sensor.onPickup()) {
    Serial.print("3. PICKUP ");
  }

  if(human_sensor.onRawValueChange()) {
    Serial.println(human_sensor.getRawValue());
  }
}