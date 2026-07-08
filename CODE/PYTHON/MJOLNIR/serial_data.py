import serial
import serial.tools.list_ports
import time
from config import LOCAL_TEST



# Identify port
# ports = serial.tools.list_ports.comports()
# for port in ports:
#     print(port.device)

# Sensor Data
hud_data = {
    "DIR": 0,
    "GLOVE_STATE": 0, #
    "GEST": 0,
    "REAR_STATE": 0, # RearGuard Activated
    "THREAT_DET": 0, #Priming sequence
    "SUIT_TEMP": 0, # Tied to air weapon
    "SUIT_HUM": 0,
    "HR":0,
    "BATTERY": 100,
    "OXYGEN": 0,
    "BODY_TEMP": 0,
    "time": 0,
}

is_connected = False

def read_serial_data():
    try:
        return serial.Serial('COM7', 115200, timeout=0.01)
    except serial.SerialException:
        try:
            return serial.Serial('/dev/ttyACM0', 115200, timeout=0.01)
        except serial.SerialException:
            return None

# Give Arduino time to reset after opening serial
ser = read_serial_data()
if ser is not None:
    is_connected = True
    time.sleep(2)
    ser.reset_input_buffer()
else:
    is_connected = False

def get_screen_index(screen_index):
    if int(hud_data["GEST"]) != 0:
        screen_index = int(hud_data["GEST"])
    else:
        screen_index = screen_index
    return screen_index

def split_assign_data():
    status = []
    while ser.in_waiting > 0:
        full_data = ser.readline().decode('utf-8', errors='ignore').strip()

        if not full_data:
            continue

        print(full_data)

        status = full_data.split(",")

        for item in status:
            if ":" in item:
                key, value = item.split(":", 1)

                if key in hud_data:
                    hud_data[key] = value


def data_update():
    global ser
    global is_connected

    if is_connected:
        try:
            split_assign_data()
        except (serial.SerialException, AttributeError):
            is_connected = False

    elif not is_connected:
        for items in hud_data:
            hud_data[items] = 0
        try:
            ser = read_serial_data()
            is_connected = True
        except serial.SerialException:
            ser = None
            is_connected = False
    else:
        return
