import pygame
from PIL import Image

from config import HUD_COLOR, LOCAL_TEST
from serial_data import hud_data

if not LOCAL_TEST:
    from lcd import LCD_2inch


def load_assets():
    fonts = load_fonts()

    assets = {
        "bkg": pygame.image.load(
            "Images/wrist_icons/wrist_bkg.png"
        ).convert_alpha(),

        "cal": fonts["calibrating_font"].render(
            "CAL",
            True,
            (66, 245, 129)
        ),

        "err": fonts["error_font"].render(
            "ERR",
            True,
            (255, 0, 0)
        )
    }

    return assets


def load_fonts():
    return {
        "vitals_font": pygame.font.Font(
            "Fonts/Technology.ttf",
            20
        ),
        "error_font": pygame.font.Font(
            "Fonts/Technology.ttf",
            20
        ),
        "calibrating_font": pygame.font.Font(
            "Fonts/Technology.ttf",
            20
        )
    }


def choose_status(data, fonts, assets):
    try:
        value = int(float(data))
    except (TypeError, ValueError):
        return assets["err"]

    if value == 0:
        return assets["err"]

    if value in (1, 2):
        return assets["cal"]

    return fonts["vitals_font"].render(
        str(data),
        True,
        HUD_COLOR
    )


def draw_vitals(screen, assets, fonts):
    screen.fill((0, 0, 0))
    screen.blit(assets["bkg"], (0, 0))

    hr = choose_status(hud_data["HR"], fonts, assets)
    suit_temp = choose_status(hud_data["SUIT_TEMP"], fonts, assets)
    suit_hum = choose_status(hud_data["SUIT_HUM"], fonts, assets)
    oxygen = choose_status(hud_data["OXYGEN"], fonts, assets)
    body_temp = choose_status(hud_data["BODY_TEMP"], fonts, assets)
    battery = choose_status(hud_data["BATTERY"], fonts, assets)

    screen.blit(hr, (90, 40))
    screen.blit(body_temp, (90, 110))
    screen.blit(suit_hum, (90, 190))

    screen.blit(suit_temp, (260, 40))
    screen.blit(oxygen, (260, 110))
    screen.blit(battery, (260, 190))

    return screen


def convert_to_image(surface):
    raw_data = pygame.image.tostring(surface, "RGB")

    return Image.frombytes(
        "RGB",
        surface.get_size(),
        raw_data
    )


def draw_local_wrist_monitor(main_screen, vitals_surface):
    scaled_vitals = pygame.transform.scale(
        vitals_surface,
        (640, 480)
    )

    pygame.draw.rect(
        main_screen,
        HUD_COLOR,
        (1230, 500, 660, 500),
        3
    )

    main_screen.blit(scaled_vitals, (1240, 510))


def initialize_vital_monitor():
    if LOCAL_TEST:
        return None

    disp = LCD_2inch.LCD_2inch()
    disp.Init()
    disp.clear()
    disp.bl_DutyCycle(50)

    return disp


def draw_pi_wrist_monitor(disp, vitals_surface):
    if LOCAL_TEST or disp is None:
        return

    image = convert_to_image(vitals_surface)

    # Pygame surface: 320 × 240
    # Physical LCD: 240 × 320
    image = image.rotate(90, expand=True)

    disp.ShowImage(image)