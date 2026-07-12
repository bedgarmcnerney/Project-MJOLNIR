from picamera2 import Picamera2
import cv2

# Initialize the camera
picam2 = Picamera2()

config = picam2.create_preview_configuration(
    main={"size": (1280, 720), "format": "RGB888"}
)

picam2.configure(config)
picam2.start()

print("Camera started.")
print("Press 'q' to quit.")

while True:
    frame = picam2.capture_array()

    cv2.imshow("Pi Camera 3 NoIR", frame)

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break

picam2.stop()
cv2.destroyAllWindows()