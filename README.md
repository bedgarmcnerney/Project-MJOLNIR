# Project-MJOLNIR

A real-world recreation of Master Chief's MJOLNIR armor, built as an embedded systems project on a college student's budget. An Arduino reads the suit's biometric and environmental sensors (C++), and a Raspberry Pi turns that data into a heads-up display and a wrist LCD (Python).

It started as an elaborate Halloween costume and grew into a full hardware and software build: CAD and 3D printing, sensor integration, serial communication, and real-time display code.

<!-- TODO: add helmet photo and HUD GIF here -->

## Current Status

| Component                                                      | Status |
|----------------------------------------------------------------|---|
| Arduino sensor firmware (modular C++, handles offline sensors) | ✅ Working |
| Serial link to the Pi with automatic reconnect                 | ✅ Working |
| Main HUD (pygame)                                              | ✅ Working |
| Wrist LCD vitals display (SPI)                                 | ✅ Working |
| Gesture-based HUD switching (MPU6050)                          | ✅ Working |
| Night vision HUD mode                                          | 🚧 In progress |
| 3D-printed armor shell                                         | 🚧 In progress |
| Power System (LiFePO4 batteries)                               | 🚧 In progress |
| Thermal vision mode                                            | ⏳ Planned |
| Battery monitoring (HUD currently shows a placeholder)         | ⏳ Planned |
| Weapon System                                                  | ⏳ Planned |

## Architecture

```
 Sensors ───► Arduino (C++) ──── USB serial, 115200 baud ────► Raspberry Pi (Python)
 MAX30205                      DIR:0,GEST:1,HR:72,...           ├─ serial_data.py   parse + reconnect
 MAX30102                                                       ├─ hud.py           pygame HUD
 MPU6050                                                        └─ vital_tracker.py wrist LCD (SPI)
 DHT11
 HC-SR04
```

- **Arduino firmware** (`CODE/Arduino/suit_controller/`): one module per sensor (`accelerometer`, `body_temp`, `pulse_oximeter`, `suit_temp_humidity`, plus the ultrasonic RearGuard), polled on timers. Each loop it prints a single line of `KEY:value` pairs, e.g. `DIR:0,GEST:1,HR:72,BODY_TEMP:36.6,...`. A sensor that goes offline reports a status code instead of freezing the suit.
- **Raspberry Pi software** (`CODE/Python/MJOLNIR/`): parses each line into a data dictionary, reopens the serial port automatically if the Arduino is unplugged or resets, and zeroes stale readings on disconnect. The HUD runs at 60 FPS and renders to the helmet display, while vitals are drawn to the wrist LCD.
- **Gestures**: the MPU6050 reports a gesture code. The Pi switches HUD screens only when the code changes, since the Arduino repeats the last gesture on every line.

## Features

- **MJOLNIR Armor**
  - PLA shell (credit: MoeSizzlac on Thingiverse) reinforced with fiberglass cloth and epoxy
- **ATHENA Helm**
  - Heads-up display with multiple modes:
    - Main HUD: suit status, battery indicator, time, Halo-style HUD border
    - Night vision ("Predator mode")
    - Planned Thermal Vision
    - Admin Screen (For debugging)
  - Floodlights, thermal imaging, rearview and night vision cameras
  - One-way orange tinted visor
- **AEGIS-INTELLISENSE:** environmental and biometric sensor suite
  - **HEPHAESTUS Gauntlets**
    - MAX30205 and MAX30102 on the index and middle fingers for pulse, body temperature, and blood oxygen
    - MPU6050 accelerometer for gesture-based HUD switching
    - HC-SR04 ultrasonic sensor for the **RearGuard** threat detector
    - Metal contact pads for additional suit controls and macros
  - DHT11 suit temperature and humidity sensor
  - 2" IPS LCD (240×320, SPI) in the left forearm armor showing all sensor data and biometrics, with a custom-modeled frame for armor embedding
- **M88-AC Njörðr Cannon** (planned)
  - Forearm-mounted compressed air cannon with laser sight
  - Solenoid triggered from the left gauntlet


Four planned weapon subsystems, still in the conceptual planning phase:

- **ARES Weapon System** 
  - **Njörðr's Wind** (compressed air cannon) | Pump option (hand pump or CO2 cartridge), pressure chamber, 12V–24V solenoid valve, barrel
  - **Thor's Hammer** | AC converter, step-up transformer, servo for retracting the mechanism, safety features (evaluating)
  - **Surtr's Flame** | Gas line feed (evaluating a back-mounted fire extinguisher), pilot flame or spark, solenoid valve (evaluating)
  - **Odin's Eye** (laser) | 12V–24V laser, cooling system, lens magnifying array (evaluating)

## Repository Layout

```
CODE/Arduino/suit_controller/   Arduino firmware (C++)
CODE/Python/MJOLNIR/            HUD and wrist display software (Python)
BOM.md                          Bill of materials
```

## Hardware

- Arduino-compatible board (USB serial to the Pi)
- Raspberry Pi 5
- Sensors: MAX30205 (body temperature), MAX30102 (pulse and SpO2), MPU6050 (gestures), DHT11 (suit temperature and humidity), HC-SR04 (RearGuard)
- 2" SPI IPS LCD, 240×320 (Waveshare driver)

**Arduino pins** (`hardware.h`)

| Function | Pin |
|---|---|
| DHT11 data | D2 |
| Ultrasonic enable | D4 |
| Gesture enable | D7 |
| HC-SR04 trigger / echo | D9 / D10 |

The I2C sensors share the Arduino's I2C bus.


**Raspberry Pi to wrist LCD** (`lcd/lcdconfig.py`, BCM numbering)

| Signal | GPIO |
|---|---|
| Reset | 27 |
| Data/Command | 25 |
| Backlight (PWM) | 18 |
| SPI | SPI0 (CE0), 40 MHz |

The full parts list is in [BOM.md](BOM.md).

## Dependencies

- **Arduino Dependencies**                                                                                                                                                                               
  Install these through the Arduino IDE Library Manager (Tools → Manage Libraries):                                                                                                                      
                                                                                                                                                                                                         
  | Library | Version | Used for |                                                                                                                                                                       
  |---|---|---|                                                                                                                                                                                          
  | DHT sensor library (Adafruit) | 1.4.7 | DHT11 suit temperature/humidity |                                                                                                                            
  | Adafruit Unified Sensor | 1.1.15 | Required by the DHT library |                                                                                                                                     
  | SparkFun MAX3010x Pulse and Proximity Sensor Library | 1.1.2 | MAX30102 heart rate |                                                                                                                 
  | ProtoCentral MAX30205 Body Temperature Sensor Library | 2.1.1 | MAX30205 body temperature |
- **Python** (`CODE/Python/MJOLNIR/requirements.txt`): pygame, opencv-python, numpy, pillow, pyserial, plus spidev and gpiozero on Linux

## How to Run

**1. Flash the Arduino**
1. Open `CODE/Arduino/suit_controller/suit_controller.ino` in the Arduino IDE.
2. Point the IDE's sketchbook (libraries) location at `CODE/Arduino/`, or install the libraries above.
3. Select your board and port, then upload.

**2. Start the HUD**
```bash
cd CODE/Python/MJOLNIR
pip install -r requirements.txt
python main.py
```

- On a Raspberry Pi the HUD runs fullscreen and drives the SPI wrist LCD. The serial port is tried as `COM7`, then `/dev/ttyACM0`.
- On any other machine it runs in a 1920×1080 window in local test mode, with the wrist display drawn on-screen. Set `HUD_LOCAL_TEST=1` or `0` to force a mode.
- Keys: `1` main HUD, `2` thermal view, `3` blank, `Esc` quit. Gestures from the gauntlet switch screens the same way.

*This is a working document, updated as the build progresses.*