# Project-MJOLNIR
This project aims to recreate Master Chief's iconic suit of high tech armor to the best of my real world capabilities.

The idea started out as a complicated Halloween Costume, and has upgraded to a full-fledged embedded systems project, utilizing a blend of hardware design techniques with 3D printing and prototyping, and computer and microcontroller programming using C and python.

**Main Features Include:**
- **Master Chief Armor**
  - PLA Shell (Credit to : MoeSizzlac on Thingiverse) reinforced with fiberglass cloth and epoxy
- **APEX HELM**
  - **Heads Up Display** (HUD) featuring various display options:
    - Main HUD Display: Suit System Status, Battery Percentage, Time, Standard Halo game HUD border
    - Thermal Vision "Predator Mode"
    - Night Vision
    - Blank Screen
  - Floodlights, thermal imaging, rearview and night vision cameras
  - One way orange tineted visor
- **Aegis-Sensor Network:** System of various enviornental and biometric sensors within the suit
  - **Exo Gloves**:
    - MAX30205 and MAX30102 placed on the pointer and middle finger for pulse, body temperature, and
      blood oxygen level measurements.
    - MPU6050 accelrometer for HUD display swithching macros
    - HC-SR04 ultrasonic sensor for **RearGuard** threat detector
    - Metal contact pads for additional suit controls and macros
  - DHT11 suit temperature and humidity sensor
  - 2inch LCD Display Module, IPS Screen, 240×320 Resolution, SPI Interface located on the left forearm armor piece
    displaying all sensor data and biometrics. Includes a custom modeled frame for armor embedding.
- **M88-AC “Zephyr” Cannon**:
  - Forearm mounted compressed air cannon
  - Laser Sight
  - Solenoid activated using L-Exo Glove
      
This is a working document that is updated regularly with my progress
