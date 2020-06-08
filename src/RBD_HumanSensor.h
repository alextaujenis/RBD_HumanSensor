// Arduino RBD Human Sensor Library v1.0.5 - Detect human presence, touch, and pickup events.
// https://github.com/alextaujenis/RBD_HumanSensor
// Copyright (c) 2015 Alex Taujenis - MIT License

#ifndef RBD_HUMAN_SENSOR
#define RBD_HUMAN_SENSOR

#include <Arduino.h>

namespace RBD {
  class HumanSensor {
    public:
      HumanSensor(int send_pin, int receive_pin);
      void update();
      void setSampleSize(int value);
      void setNearValue(int value);
      void setTouchValue(int value);
      void setPickupValue(int value);
      void setModifier(int value);
      void resetModifier();
      int getValue();
      int getRawValue();
      int getActiveLevel();
      bool isGone();
      bool isNear();
      bool isTouch();
      bool isPickup();
      bool onGone();
      bool onNear();
      bool onTouch();
      bool onPickup();
      bool onValueChange();
      bool onRawValueChange();
      bool onActiveLevelChange();
    private:
      void _resetEvents();
      int _new_value          = 0;
      int _past_value         = 0;
      int _new_raw_value      = 0;
      int _past_raw_value     = 0;
      int _new_level          = 0;
      int _past_level         = 0;
      bool _has_been_gone     = false;
      bool _has_been_near     = false;
      bool _has_been_touch    = false;
      bool _has_been_pickup   = false;
      int _raw_value          = 0;
      int _raw_value_modifier = 0;
      int _temp_value         = 0;
      int _temp_prev_value    = 0;
      int _temp_level         = 0;
      Capacitance _cap_sensor;
      Threshold _threshold;
  };
}

#endif