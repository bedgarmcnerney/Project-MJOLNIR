import pygame

from config import LOCAL_TEST, HUD_WIDTH, HUD_HEIGHT, VITAL_MONITOR_WIDTH, VITAL_MONITOR_HEIGHT
from serial_data import data_update,get_screen_index

from hud import (
    load_hud_assets,
    load_hud_fonts,
    draw_default_hud,
    setup_camera,
    get_camera_frame,
    frame_to_pygame_surface,
    draw_thermal_hud,
    draw_blank,

)
from vital_tracker import (
    load_assets as load_vital_assets,
    load_fonts as load_vital_fonts,
    draw_vitals,
    draw_local_wrist_monitor,
    initialize_vital_monitor,
    draw_pi_wrist_monitor
)



def main():
    pygame.init()
    cap = setup_camera()

    if LOCAL_TEST:
        screen = pygame.display.set_mode((HUD_WIDTH, HUD_HEIGHT))
        pygame.display.set_caption("MJOLNIR HUD - Local Test")
    else:
        screen = pygame.display.set_mode((0, 0), pygame.FULLSCREEN)

    clock = pygame.time.Clock()
    running = True

    hud_assets = load_hud_assets()
    hud_fonts = load_hud_fonts()

    vital_assets = load_vital_assets()
    vital_fonts = load_vital_fonts()

    vitals_surface = pygame.Surface((VITAL_MONITOR_WIDTH, VITAL_MONITOR_HEIGHT))
    screen_index = 0
    vital_display = None

    if not LOCAL_TEST:
        vital_display = initialize_vital_monitor()

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    running = False
                elif event.key == pygame.K_1:
                    screen_index = 1
                elif event.key == pygame.K_2:
                    screen_index = 2
                elif event.key == pygame.K_3:
                    screen_index = 3
                else:
                    screen_index = 1
        data_update()
        #screen_index = get_screen_index(screen_index)
        if screen_index == 1:
            draw_default_hud(screen, hud_assets, hud_fonts)
        elif screen_index == 2:
            draw_thermal_hud(cap,screen)
        elif screen_index == 3:
            draw_blank(screen)
        else:
            screen_index = 1


        draw_vitals(vitals_surface, vital_assets, vital_fonts)

        if LOCAL_TEST:
            draw_local_wrist_monitor(screen, vitals_surface)
        else:
           draw_pi_wrist_monitor(vital_display, vitals_surface)
        pygame.display.flip()
        clock.tick(60)

    pygame.quit()


if __name__ == "__main__":
    main()