import pygame
import datetime
import cv2
from picamera2 import Picamera2

from config import HUD_COLOR, HUD_WIDTH, HUD_HEIGHT
from serial_data import hud_data


## Utitlity ##
# Returns ON OR OFF image
def choose_status(img_enabled,img_disabled,status):
    try:
        return img_enabled if int(status) == 1 else img_disabled
    except (ValueError, TypeError):
        return img_disabled

    return img_disabled

def load_hud_assets():
    assets = {"bg": pygame.image.load("Images/hud_icons/HUD1.jpg").convert_alpha(),
              "battery": pygame.image.load("Images/hud_icons/battery.png").convert_alpha(),

              "gauntlet_enabled": pygame.image.load("Images/hud_icons/gauntlet.png").convert_alpha(),  # Physical switch is on
              "gauntlet_disabled": pygame.image.load("Images/hud_icons/gauntlet_disabled.png").convert_alpha(),  # Switch is off
              "gesture_reading" : pygame.image.load("Images/hud_icons/gauntlet_disabled.png").convert_alpha(),  # Taking accelerometer reading

              "aircannon_enabled": pygame.image.load("Images/hud_icons/aircannon.png").convert_alpha(),
              "aircannon_disabled" : pygame.image.load("Images/hud_icons/aircannon_disabled.png").convert_alpha(),

              # TODO: replace with RearGuard icons
              "rearguard_enabled": pygame.image.load("Images/hud_icons/aircannon.png").convert_alpha(),  # Connected
              "rearguard_disabled": pygame.image.load("Images/hud_icons/aircannon_disabled.png").convert_alpha(),  # Not connected
              "rearguard_armed": pygame.image.load("Images/hud_icons/aircannon_disabled.png").convert_alpha(),  # Reading Threats



              #"compass" : pygame.image.load("Images/hud_icons/compass.png").convert_alpha()
              }
    return assets

def load_hud_fonts():
    fonts = {
        "time_font" : pygame.font.Font("Fonts/Technology.ttf", 60),
        "battery_font" : pygame.font.Font("Fonts/Technology.ttf", 60)
    }
    return fonts

def draw_default_hud(screen, assets, fonts):
    screen.blit(assets["bg"], (0,0))

    current_time = datetime.datetime.now().strftime("%H:%M:%S")
    hud_data["time"] = current_time
    time_text = fonts["time_font"].render(f"{current_time}", True, HUD_COLOR)
    screen.blit(time_text, (100, 100))

    battery_text = fonts["battery_font"].render(f"{hud_data['BATTERY']}", True, HUD_COLOR)
    screen.blit(battery_text,(100, 170))
    screen.blit(assets["battery"], (180, 162))

    screen.blit(
        choose_status(
            assets["gauntlet_enabled"],
            assets["gauntlet_disabled"],
            hud_data["GLOVE_STATE"]),
        dest=(1700, 100))

    screen.blit(
        choose_status(assets["aircannon_enabled"],
        assets["aircannon_disabled"],
        hud_data["GLOVE_STATE"]),
        dest=(1700, 180))
    screen.blit(
        choose_status(assets["rearguard_enabled"],
        assets["rearguard_disabled"],
        hud_data["REAR_STATE"]),
        dest=(1700, 260))

    # rotated_image = pygame.transform.rotate(compass, current_angle)
    # rotated_rect = rotated_image.get_rect(center=(290,900))
    # screen.blit(rotated_image, rotated_rect)



def draw_thermal_hud(cap,screen):
    frame = get_camera_frame(cap)

    if frame is not None:
        camera_surface = frame_to_pygame_surface(frame)
        camera_surface = pygame.transform.scale(camera_surface, (1920, 1080))

        screen.blit(camera_surface, (0, 0))
def draw_blank(screen):
    screen.fill((0,0,0))

def setup_camera():
    picam2 = Picamera2()

    config = picam2.create_preview_configuration(
        main={
            "size": (1920, 1080),
            "format": "RGB888"
        }
    )

    picam2.configure(config)
    picam2.start()

    return picam2

def get_camera_frame(camera):
    if camera is None:
        return None

    frame = camera.capture_array()

    return frame

def frame_to_pygame_surface(frame):
    return pygame.surfarray.make_surface(frame.swapaxes(0, 1))

