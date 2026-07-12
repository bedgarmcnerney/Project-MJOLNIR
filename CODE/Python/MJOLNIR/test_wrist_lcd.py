from gpiozero import Button
from signal import pause

def on_button_press():
    print("Button pressed!")

button = Button(17)  # Use the correct GPIO pin number
button.when_pressed = on_button_press

pause()  # Keep the program running