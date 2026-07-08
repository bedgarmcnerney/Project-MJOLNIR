//////////////////////////////////////////////////////////////////////////////////////////
//
//    Arduino example for the MAX30205 body temperature sensor breakout board
//
//    Author: Ashwin Whitchurch
//    Copyright (c) 2020 ProtoCentral
//
//    This software is licensed under the MIT License(http://opensource.org/licenses/MIT).
//
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
//   NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//   For information on how to use, visit https://github.com/protocentral/ProtoCentral_MAX30205
/////////////////////////////////////////////////////////////////////////////////////////

/*

This program Print temperature on terminal

Hardware Connections (Breakoutboard to Arduino):
Vin  - 5V (3.3V is allowed)
GND - GND
SDA - A4 (or SDA)
SCL - A5 (or SCL)

*/

#include <Wire.h>
#include <Arduino.h>
#include "hardware.h"

#include "Protocentral_MAX30205.h"
MAX30205 tempSensor;
float body_temperature = 0;

void body_temp_setup() {

  Wire.begin();

  // //scan for temperature in every 30 sec untill a sensor is found. Scan for both addresses 0x48 and 0x49
  // while(!tempSensor.scanAvailableSensors()){
  //   Serial.println("Couldn't find the temperature sensor, please connect the sensor." );
  // }

  tempSensor.begin();   // set continuos mode, active mode
}

void body_temp_loop() {
	body_temperature = tempSensor.getTemperature(); // read temperature for every 100ms
  body_temperature = (body_temperature * 9.0 / 5.0) + 32.0;
}
float get_body_temp(){
  if (!tempSensor.scanAvailableSensors()){
    return BODY_TEMP_OFFLINE;
  }
  else if (body_temperature < 90){
    return BODY_TEMP_CALIBRATING;
  }
  return body_temperature;
}
