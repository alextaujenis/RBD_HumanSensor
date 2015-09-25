#ifndef RBD_HUMAN_SENSOR
#define RBD_HUMAN_SENSOR

#include <Arduino.h>

namespace RBD {
  class HumanSensor {
    public:
      HumanSensor(int send_pin, int receive_pin);
      void update();
      bool isGone();
      bool isNear();
      bool isTouch();
      bool isPickup();
      void setNearValue(int value);
      void setTouchValue(int value);
      void setPickupValue(int value);
      void setModifier(int value);
      void resetModifier();
      int getValue();
      int getRawValue();
      int getActiveLevel();
    private:
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