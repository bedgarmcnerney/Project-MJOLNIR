from PIL import Image
import sys
import time

sys.path.append("/home/master_chief/LCD_Module_RPI_code/RaspberryPi/python")

from lib import LCD_2inch

disp = LCD_2inch.LCD_2inch()

disp.Init()
disp.clear()
disp.bl_DutyCycle(50)

image = Image.open("Images/wrist_icons/wrist_bkg.png").convert("RGB")

# Your image is 320x240, but the LCD is usually 240x320.
# Rotate it so it fits the screen.
image = image.rotate(90, expand=True)

disp.ShowImage(image)

while True:
    time.sleep(1)