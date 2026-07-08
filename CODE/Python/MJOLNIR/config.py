import os
import socket
import platform

HOSTNAME = socket.gethostname()

RUNNING_ON_PI = (
    HOSTNAME.lower() == "halo" or
    "arm" in platform.machine().lower() or
    "aarch" in platform.machine().lower()
)

# Optional override:
# HUD_LOCAL_TEST=1 forces local test mode
# HUD_LOCAL_TEST=0 forces real Pi mode
override = os.environ.get("HUD_LOCAL_TEST")

if override == "1":
    LOCAL_TEST = True
elif override == "0":
    LOCAL_TEST = False
else:
    LOCAL_TEST = not RUNNING_ON_PI

HUD_COLOR = "#2CA9C9"

HUD_WIDTH = 1920
HUD_HEIGHT = 1080

VITAL_MONITOR_WIDTH = 320
VITAL_MONITOR_HEIGHT = 240