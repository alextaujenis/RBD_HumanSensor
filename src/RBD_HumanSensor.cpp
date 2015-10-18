// Arduino RBD Human Sensor Library v1.0.1 - Detect human presence, touch, and pickup events.
// https://github.com/alextaujenis/RBD_HumanSensor
// Copyright 2015 Alex Taujenis
// MIT License

#include <Arduino.h>
#include <RBD_Capacitance.h> // https://github.com/alextaujenis/RBD_Capacitance
#include <RBD_Threshold.h>   // https://github.com/alextaujenis/RBD_Threshold
#include <RBD_HumanSensor.h> // https://github.com/alextaujenis/RBD_HumanSensor

namespace RBD {

  HumanSensor::HumanSensor(int send_pin, int receive_pin)
  : _cap_sensor(send_pin, receive_pin), _threshold(2) {
    _cap_sensor.setSampleSize(250);
    // default threshold values are set at unobtainable numbers so they can remain unused
    _threshold.setLevel(1,10010);
    _threshold.setLevel(2,10020);
    _threshold.setMaxLevel(10030);
  }

  void HumanSensor::update() {
    _cap_sensor.update();

    if(_cap_sensor.isFinished()) {
      _raw_value = _cap_sensor.getValue();
      _cap_sensor.start();
    }
  }

  void HumanSensor::setSampleSize(int value) {
    _cap_sensor.setSampleSize(value);
  }

  bool HumanSensor::onGone() {
    if(isGone()) {
      if(!_has_been_gone) {
        _resetEvents();
        _has_been_gone = true;
        return true;
      }
      return false;
    }
    else {
      _has_been_gone = false;
      return false;
    }
  }

  bool HumanSensor::onNear() {
    if(isNear()) {
      if(!_has_been_near) {
        _resetEvents();
        _has_been_near = true;
        return true;
      }
      return false;
    }
    else {
      _has_been_near = false;
      return false;
    }
  }

  bool HumanSensor::onTouch() {
    if(isTouch()) {
      if(!_has_been_touch) {
        _resetEvents();
        _has_been_touch = true;
        return true;
      }
      return false;
    }
    else {
      _has_been_touch = false;
      return false;
    }
  }

  bool HumanSensor::onPickup() {
    if(isPickup()) {
      if(!_has_been_pickup) {
        _resetEvents();
        _has_been_pickup = true;
        return true;
      }
      return false;
    }
    else {
      _has_been_pickup = false;
      return false;
    }
  }

  bool HumanSensor::isGone() {
    return getActiveLevel() == 0;
  }

  bool HumanSensor::isNear() {
    return getActiveLevel() == 1;
  }

  bool HumanSensor::isTouch() {
    return getActiveLevel() == 2;
  }

  bool HumanSensor::isPickup() {
    return getActiveLevel() == 3;
  }

  bool HumanSensor::onValueChange() {
    _new_value = getValue();

    if(_past_value != _new_value) {
      _past_value = _new_value;
      return true;
    }
    else {
      return false;
    }
  }

  bool HumanSensor::onRawValueChange() {
    _new_raw_value = getRawValue();

    if(_past_raw_value != _new_raw_value) {
      _past_raw_value = _new_raw_value;
      return true;
    }
    else {
      return false;
    }
  }


  bool HumanSensor::onActiveLevelChange() {
    _new_level = getActiveLevel();

    if(_past_level != _new_level) {
      _past_level = _new_level;
      return true;
    }
    else {
      return false;
    }
  }

  void HumanSensor::setNearValue(int value) {
    _threshold.setLevel(1, value);
  }

  void HumanSensor::setTouchValue(int value) {
    _threshold.setLevel(2, value);
  }

  void HumanSensor::setPickupValue(int value) {
    _threshold.setMaxLevel(value);
  }

  void HumanSensor::setModifier(int value) {
    _raw_value_modifier = value;
  }

  void HumanSensor::resetModifier() {
    setModifier(0);
  }

  int HumanSensor::getValue() {
    return _raw_value + _raw_value_modifier;
  }

  int HumanSensor::getRawValue() {
    return _raw_value;
  }

  int HumanSensor::getActiveLevel() {
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
    // return the changed or memoized level
    return _temp_level;
  }


  // private

  void HumanSensor::_resetEvents() {
    _has_been_gone   = false;
    _has_been_near   = false;
    _has_been_touch  = false;
    _has_been_pickup = false;
  }
}


