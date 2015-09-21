#include <Arduino.h>
#include <RBD_Capacitance.h>
#include <RBD_Threshold.h>
#include <RBD_HumanSensor.h>

namespace RBD {

  HumanSensor::HumanSensor(int send_pin, int receive_pin)
  : _cap_sensor(send_pin, receive_pin), _threshold(3) {
    _cap_sensor.setSampleSize(100);
    _cap_sensor.start();
  }

  void HumanSensor::update() {
    _cap_sensor.update();

    if(_cap_sensor.isFinished()) {
      _raw_value = _cap_sensor.getValue();
      _cap_sensor.start();
    }
  }

  bool HumanSensor::isGone() {
    return getLevel() == 0;
  }

  bool HumanSensor::isNear() {
    return getLevel() == 1;
  }

  bool HumanSensor::isTouch() {
    return getLevel() == 2;
  }

  bool HumanSensor::isPickup() {
    return getLevel() > 2;
  }

  int HumanSensor::getLevel() {
    // this function memoizes the level so it's only computed when the raw value or modifier changes
    // get the current (modifed raw) value
    _temp_value = getValue();

    // check if it has changed
    if (_temp_value != _temp_prev_value) {
      // save a new level if it has changed
      _temp_level = _threshold.computeLevel(_temp_value);
      // store the old value
      _temp_prev_value = _temp_value;
    }
    return _temp_level;
  }

  void HumanSensor::setNearValue(int value) {
    _threshold.setLevel(1, value);
  }

  void HumanSensor::setTouchValue(int value) {
    _threshold.setLevel(2, value);
  }

  void HumanSensor::setPickupValue(int value) {
    _threshold.setLevel(3, value);
    _threshold.setMaxLevel(value + 10); // just consider levels 3 and 4 as pickup
  }

  void HumanSensor::setModifier(int value) {
    _raw_value_modifier = value;
  }

  void HumanSensor::resetModifier() {
    setModifier(0);
  }

  int HumanSensor::getRawValue() {
    return _raw_value;
  }

  int HumanSensor::getValue() {
    return _raw_value + _raw_value_modifier;
  }
}