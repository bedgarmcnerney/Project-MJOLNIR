import pygame
from config import HUD_COLOR
from serial_data import hud_data
from PIL import Image
from config import LOCAL_TEST



# def load_assets():
#     assets = {"heartrate": pygame.image.load("Images/wrist_icons/pulse.png").convert_alpha(),
#               "thermometer": pygame.image.load("Images/wrist_icons/thermometer.png").convert_alpha(),
#               "humidity": pygame.image.load("Images/wrist_icons/humidity.png").convert_alpha(),
#               "bodytemp": pygame.image.load("Images/wrist_icons/bodytemp.png").convert_alpha(),
#               "oxygen": pygame.image.load("Images/wrist_icons/oxygen.png").convert_alpha()
#               }
#     return assets


def load_assets():
    font = load_fonts()
    assets = {"bkg": pygame.image.load("Images/wrist_icons/wrist_bkg.png").convert_alpha(),
              "cal": font["calibrating_font"].render("CAL", True, (66, 245, 129)),
              "err": font["error_font"].render("ERR", True, (255,0,0))
              }
    return assets
def load_fonts():
    fonts = {
        "vitals_font": pygame.font.Font("Fonts/Technology.ttf", 20),
        "error_font": pygame.font.Font("Fonts/Technology.ttf", 20),
        "calibrating_font": pygame.font.Font("Fonts/Technology.ttf", 20)
    }
    return fonts

def choose_status(data, font, assets):

    if int(float(data)) == 0:
        return assets["err"]
    elif int(float(data)) == 1 or int(float(data)) == 2:
        return assets["cal"]
    else:
        return font["vitals_font"].render(str(data), True, HUD_COLOR)


def draw_vitals(screen, assets, fonts):
    screen.fill((0,0,0))
    bkg = assets["bkg"]
    screen.blit(bkg, (0, 0))

    hr = choose_status(hud_data['HR'], fonts,assets)
    suit_temp = choose_status(hud_data['SUIT_TEMP'], fonts,assets)
    suit_hum = choose_status(hud_data['SUIT_HUM'], fonts,assets)
    oxygen = choose_status(hud_data['OXYGEN'], fonts,assets)
    body_temp = choose_status(hud_data['BODY_TEMP'], fonts,assets)
    battery = choose_status(hud_data['BATTERY'], fonts,assets)



    # hr_icon = assets["heartrate"]
    # temp_icon = assets["thermometer"]
    # hum_icon = assets["humidity"]
    # body_icon = assets["bodytemp"]
    # oxy_icon = assets["oxygen"]

    screen.blit(hr, (90, 40))
    screen.blit(body_temp, (90, 110))
    screen.blit(suit_hum, (90, 190))

    screen.blit(suit_temp, (260, 40))
    screen.blit(oxygen, (260, 110))
    screen.blit(battery, (260, 190))

    return screen

def convert_to_image(surface):
    raw_data = pygame.image.tostring(surface, "RGB")
    image = Image.frombytes("RGB", surface.get_size(), raw_data)
    return image

def draw_local_wrist_monitor(main_screen, vitals_surface):
    scaled_vitals = pygame.transform.scale(vitals_surface, (640, 480))

    pygame.draw.rect(main_screen, HUD_COLOR, (1230, 500, 660, 500), 3)
    main_screen.blit(scaled_vitals, (1240, 510))

def initialize_vital_monitor():
    import sys

    sys.path.append("/home/master_chief/LCD_Module_RPI_code/RaspberryPi/python")
    from lib import LCD_2inch

    disp = LCD_2inch.LCD_2inch()
    disp.Init()
    disp.clear()
    disp.bl_DutyCycle(50)

    return disp


def draw_pi_wrist_monitor(disp, vitals_surface):
    image = convert_to_image(vitals_surface)

    # Your vitals surface is 320x240.
    # The Waveshare 2 inch LCD is physically 240x320.
    # Rotate so it fills the LCD in portrait orientation.
    image = image.rotate(90, expand=True)

    disp.ShowImage(image)


