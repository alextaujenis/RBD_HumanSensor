#Arduino Human Sensor Library
Detect and interact with humans through change in capacitance. You can calibrate this library for your sensor very quickly to detect [onNear()](#onnear), [onTouch()](#ontouch), and [onPickup()](#onpickup) events. The example below uses a piece of aluminum foil, but you can attach the wire to anything conductive and calibrate the sensor accordingly.

This library measures relative capacitance so it is prone to interference. You can temporarily adjust the entire scale and then reset it when the capacitance disruption has stopped by using [setModifier()](#setmodifiervalue) and [resetModifier()](#resetmodifiervalue). Look at the [Arduino Capacitance Library](https://github.com/alextaujenis/RBD_Capacitance) if you would like to know more about how capacitance is calculated.

##Real-Time Library
This real-time library requires that you manage the flow of your program without delay() or interrupts. Check out this [Arduino Timer Library](https://github.com/alextaujenis/RBD_Timer) if you are managing millis() by hand.

##Example Setup
0. Bridge a 10Meg Ohm resistor across the send (tx 1) and receive (rx 0) pins
0. Run a length of wire from the receive (rx 0) pin to a strip of aluminum foil
0. Attach the foil to an object by covering it with a non-conductive material (duct tape)
0. Load up the example sketch and open a serial connection at 115200 baud
0. Move your hand by the object, touch it, pick it up, and change the raw threshold values in the sketch
0. Compile and reload the calibrated sketch back into the Arduino
0. Your human sensor is calibrated and ready to use

**Example Sketch**:

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

#Documentation
##Public Methods

* [constructor(send\_pin, receive\_pin)](#constructorsend_pin-receive_pin)
* [update()](#update)
* [setSampleSize(value)](#setsamplesizevalue)
* [setNearValue(value)](#setnearvaluevalue)
* [setTouchValue(value)](#settouchvaluevalue)
* [setPickupValue(value)](#setpickupvaluevalue)
* [setModifier(value)](#setmodifiervalue)
* [resetModifier()](#resetmodifier)
<hr />
* [isGone()](#isgone)
* [isNear()](#isnear)
* [isTouch()](#istouch)
* [isPickup()](#ispickup)
* [getValue()](#getvalue)
* [getRawValue()](#getrawvalue)
* [getActiveLevel()](#getactivelevel)
<hr />
* [onGone()](#ongone)
* [onNear()](#onnear)
* [onTouch()](#ontouch)
* [onPickup()](#onpickup)
* [onValueChange()](#onvaluechange)
* [onRawValueChange()](#onrawvaluechange)
* [onActiveLevelChange()](#onactivelevelchange)

##constructor(send\_pin, receive\_pin)
Create a new human sensor and pass in the Arduino pin numbers for the send and receive pins.

    RBD::HumanSensor human_sensor(1,0);

    void setup() {
      ...
    }

##update()
This must be call continuously from within the loop() to move the library along in real-time.

    void loop() {
      human_sensor.update();
    }

##setSampleSize(value)
Pass in an integer to change the number of readings taken to calculate a moving average [getRawValue()](#getrawvalue). This can be called inside of setup() or also safely at runtime inside of loop().

* **Decrease Variation** in readings by making this number larger: 1000
* **Increase Variation** in readings by making this number smaller: 10
* **Default Value**: 250

Example:

    void setup() {
      human_sensor.setSampleSize(1000);
    }


##setNearValue(value)
Provide an integer from [getRawValue()](#getrawvalue) to calibrate the lowest threshold for when a person is near the sensor.

    void setup() {
      human_sensor.setNearValue(100);
    }

##setTouchValue(value)
Provide an integer from [getRawValue()](#getrawvalue) to calibrate the lowest threshold for when a person is touching the sensor.

    void setup() {
      human_sensor.setTouchValue(150);
    }

##setPickupValue(value)
Provide an integer from [getRawValue()](#getrawvalue) to calibrate the lowest threshold for when a person is picking up the sensor.

    void setup() {
      human_sensor.setPickupValue(200);
    }

##setModifier(value)
Provide a positive or negative integer to temporarily adjust the human sensor threshold scale. The default value is 0.

For example; if you have calibrated the scale and turn on a motor near the sensor: all readings will need to be adjusted for the increased capacitance from the motor.

* If a running motor increases the difference in human sensor readings by a getRawValue() of +200
* Call [setModifier(200)](#setmodifiervalue) on the human sensor when the motor turns on
* All of the calibrated human sensor thresholds will adjust +200
* The human sensor readings will account for motor interference
* Call [resetModifier()](#resetmodifier) on the human sensor when the motor shuts off

Example:

    void loop() {
      human_sensor.update();

      if(...) { // motor is on
        human_sensor.setModifier(200);
      }
      else {
        human_sensor.resetModifier();
      }

      // when the motor is on; require +200 more from
      // the raw sensor reading to trigger all events
      if(human_sensor.isNear()) {
        ...
      }
      else if(...) {
        ...
      }
    }

##resetModifier()
Changes the [setModifier()](#setmodifiervalue) back to 0 and resets calibration of the human sensor threshold scale.

For example; if you have calibrated the scale and turn on a motor near the sensor: all readings will need to be adjusted for the increased capacitance from the motor.

* If a running motor increases the difference in human sensor readings by a getRawValue() of +200
* Call [setModifier(200)](#setmodifiervalue) on the human sensor when the motor turns on
* All of the calibrated human sensor thresholds will adjust +200
* The human sensor readings will account for motor interference
* Call [resetModifier()](#resetmodifier) on the human sensor when the motor shuts off

Example:

    void loop() {
      human_sensor.update();

      if(...) {
        human_sensor.setModifier(200);
      }
      else {  // motor is off
        human_sensor.resetModifier();
      }

      // when the motor is off; reset the
      // threshold values for all events
      if(human_sensor.isNear()) {
        ...
      }
      else if(...) {
        ...
      }
    }

##isGone()
Returns true if no human is present and the sensor is reading a value below the [setNearValue()](#setnearvaluevalue). This will always return true if no thresholds (near, touch, pickup) are set.

    void loop() {
      human_sensor.update();

      if(human_sensor.isGone()) {
        ...
      }
    }

##isNear()
Returns true if a human is present and the sensor is reading a value at or above the [setNearValue()](#setnearvaluevalue).

Returns false if the sensor is reading a value below the [setNearValue()](#setnearvaluevalue), or if a [setTouchValue()](#settouchvaluevalue) has been assigned and the sensor is reading a value at or above the [isTouch()](#istouch) value.

    void loop() {
      human_sensor.update();

      if(human_sensor.isNear()) {
        ...
      }
    }

##isTouch()
Returns true if a human is touching the sensor and it is reading a value at or above the [setTouchValue()](#settouchvaluevalue).

Returns false if the sensor is reading a value below the [setTouchValue()](#settouchvaluevalue), or if a [setPickupValue()](#setpickupvaluevalue) has been assigned and the sensor is reading a value at or above the [isPickup()](#ispickup) value.

    void loop() {
      human_sensor.update();

      if(human_sensor.isTouch()) {
        ...
      }
    }

##isPickup()
Returns true if a human is picking up the sensor and it is reading a value at or above the [setPickupValue()](#setpickupvaluevalue).

Returns false if the sensor is reading a value below the [setPickupValue()](#setpickupvaluevalue).

    void loop() {
      human_sensor.update();

      if(human_sensor.isPickup()) {
        ...
      }
    }

#getValue()
Returns the capacitance reading after being adjusted with the value given to [setModifier()](#setmodifiervalue).

This will return the raw capacitance reading if you have not used [setModifier()](#setmodifiervalue) yet, or if you have called [resetModifier()](#resetmodifier).

    void loop() {
      human_sensor.update();

      if(human_sensor.onValueChange()) {
        Serial.println(human_sensor.getValue());
      }
    }

##getRawValue()
Returns the raw capacitance reading and ignores any value that has been passed to [setModifier()](#setmodifiervalue).

    void loop() {
      human_sensor.update();

      Serial.println(human_sensor.getRawValue());
    }

##getActiveLevel()
Returns the current sensor level after being adjusted by [setModifier()](#setmodifiervalue).

*  0  no human is present
*  1  a human is near
*  2  a human is touching the sensor
*  3  a human is picking up the sensor
* -1  an error occured (non-continuous range)

Example:

    void loop() {
      human_sensor.update();

      Serial.println(human_sensor.getActiveLevel());
    }

##onGone()
This method will return true once the sensor registers a value below the [setNearValue()](#setnearvaluevalue). The active level must change away from and then come back to [isGone()](#isgone) in order for this method to return true again.

    void loop() {
      human_sensor.update();

      if(human_sensor.onGone()) {
        // code only runs once per event
        Serial.println("Gone");
      }
    }

##onNear()
This method will return true once the sensor registers a value at or above the [setNearValue()](#setnearvaluevalue) and below the [setTouchValue()](#settouchvaluevalue). The active level must change away from and then come back to [isNear()](#isnear) in order for this method to return true again.

    void loop() {
      human_sensor.update();

      if(human_sensor.onNear()) {
        // code only runs once per event
        Serial.println("Near");
      }
    }

##onTouch()
This method will return true once the sensor registers a value at or above the [setTouchValue()](#settouchvaluevalue) and below the [setPickupValue()](#setpickupvaluevalue). The active level must change away from and then come back to [isTouch()](#istouch) in order for this method to return true again.

    void loop() {
      human_sensor.update();

      if(human_sensor.onTouch()) {
        // code only runs once per event
        Serial.println("Touch");
      }
    }

##onPickup()
This method will return true once the sensor registers a value at or above the [setPickupValue()](#setpickupvaluevalue). The active level must change away from and then come back to [isPickup()](#ispickup) in order for this method to return true again.

    void loop() {
      human_sensor.update();

      if(human_sensor.onPickup()) {
        // code only runs once per event
        Serial.println("Pickup");
      }
    }

##onValueChange()
This method will return true once the [getValue()](#getvalue) changes.

    void loop() {
      human_sensor.update();

      if(human_sensor.onValueChange()) {
        // code only runs once per event
        Serial.println(human_sensor.getValue());
      }
    }

##onRawValueChange()
This method will return true once the [getRawValue()](#getrawvalue) changes.

    void loop() {
      human_sensor.update();

      if(human_sensor.onRawValueChange()) {
        // code only runs once per event
        Serial.println(human_sensor.getRawValue());
      }
    }

##onActiveLevelChange()
This method will return true once the [getActiveLevel()](#getactivelevel) changes.

    void loop() {
      human_sensor.update();

      if(human_sensor.onActiveLevelChange()) {
        // code only runs once per event
        Serial.println(human_sensor.getActiveLevel());
      }
    }

#License
This code is available under the [MIT License](http://opensource.org/licenses/mit-license.php).