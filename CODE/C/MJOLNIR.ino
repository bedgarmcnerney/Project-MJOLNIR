#include "accelerometer.h"
#include "timers.h"
#include "sensor.h"
#include "suit_temp_humidity.h"
#include "pulse_oximeter.h"
#include "body_temp.h"


uint8_t gest, gest_state;
uint8_t threat_detect, rear_guard_state;
uint8_t direction = 0;

uint8_t heartrate, HR_sensor_state, finger_contact_state;
uint8_t O2 = 99;

float suit_temperature, suit_humidity;
float body_temp;

// void print_new_status(){

// // Glove signal Pins
// // Left Glove Status
//   //Thumb
//   Serial.print(",GLOVE_STATE:");
//   Serial.print();
//   //Pointer , Gesture enable state
//   Serial.print(",GEST_STATE:");
//   Serial.print();
//   //Middle , Rear Guard enable state
//   Serial.print(",REAR_STATE:");
//   Serial.print();
//   //Ring  1, Weapon + Laser
//   Serial.print(",WEAPON_STATE:");
//   Serial.print();
//   //Ring 2, Flood lights
//   Serial.print(",LIGHTS_STATE:");
//   Serial.print();
//   //Pinky 1, Voice Changer
//   Serial.print(",VOICE_STATE:");
//   Serial.print();
//   //Pinky 2, 
//   Serial.print(",EMPTY1:");
//   Serial.print();

// // Sensor Functioning States 
//   // Heartrate sensor connection status
//   Serial.print(",HR_SENSOR:");
//   Serial.print(HR_sensor_state);
//   // Heartrate finger contact status
//   Serial.print(",HR_FINGER:");
//   Serial.print(finger_contact_state);


// // Sensor Readings
//   // Glove Action
//   Serial.print(",GEST:");
//   Serial.print(gest);
//   // RearGuard Reading
//   Serial.print(",THREAT_DET:");
//   Serial.print(threat_detect);
//   // Suit Temp
//   Serial.print(",SUIT_TEMP:");
//   Serial.print(suit_temperature);
//   // Suit Humidity
//   Serial.print(",SUIT_HUM:");
//   Serial.print(suit_humidity);
//   // HeartRate
//   Serial.print(",HR:");
//   Serial.print(heartrate);
//   // Body Temperature
//   Serial.print(",BODY_TEMP:");
//   Serial.print(body_temperature);
//   // Battery
//   Serial.print(",BATTERY:");
//   Serial.print(100);

// }

void print_status(){
  // Compass
  Serial.print("DIR:");
  Serial.print(direction);

  // Glove Status
  Serial.print(",GLOVE_STATE:");
  Serial.print(gest_state);

  // Glove Action
  Serial.print(",GEST:");
  Serial.print(gest);

  // RearGuard Status
  Serial.print(",REAR_STATE:");
  Serial.print(rear_guard_state);

  // RearGuard Reading
  Serial.print(",THREAT_DET:");
  Serial.print(threat_detect);

  // Suit Temp
  Serial.print(",SUIT_TEMP:");
  Serial.print(suit_temperature);

  // Suit Humidity
  Serial.print(",SUIT_HUM:");
  Serial.print(suit_humidity);

  // HeartRate
  Serial.print(",HR:");
  Serial.print(heartrate);

  // Heartrate finger contact status
  Serial.print(",BODY_TEMP:");
  Serial.print(body_temp);

  // Battery
  Serial.print(",BATTERY:");
  Serial.print(100);

  // O2
  Serial.print(",OXYGEN:");
  Serial.print(O2);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  accelerometer_setup();
  timer_setup();
  ultrasonic_sensor_setup();
  suit_temp_setup();
  pulse_setup();
  body_temp_setup();
  Serial.println();

}

void loop() {

  accelerometer_loop();
  gest = get_action();
  gest_state = get_gesture_enable_state();

  threat_detect = ultrasonic_sensor_loop();
  rear_guard_state = rear_guard_enable_state();

  suit_temp_loop();
  suit_temperature = suit_temp();
  suit_humidity = humidity();

  pulse_loop();
  heartrate = pulse();

  body_temp_loop();
  body_temp = get_body_temp();
  print_status();
}