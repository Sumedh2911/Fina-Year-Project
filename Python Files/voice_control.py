import speech_recognition as sr
import requests
import time

# ESP32 Base URL
ESP32_IP = "http://192.168.69.217"  # Change to your ESP32 IP

# Function to send robotic arm commands to ESP32
def control_robot_arm(command):
    url = f"{ESP32_IP}/cart/{command}"
    try:
        response = requests.get(url)
        print(f"Sent command: {command}, ESP32 Response: {response.text}")
    except Exception as e:
        print(f"Failed to send command: {command}, Error: {e}")

# Recognize voice and return command
def recognize_speech():
    recognizer = sr.Recognizer()
    mic = sr.Microphone()

    with mic as source:
        print("Say a command (left, right, up, down, forward, backward, open, close, reset)...")
        recognizer.adjust_for_ambient_noise(source)
        audio = recognizer.listen(source)

    try:
        command = recognizer.recognize_google(audio).lower()
        print(f"Recognized: {command}")
        return command
    except sr.UnknownValueError:
        print("Could not understand audio")
    except sr.RequestError as e:
        print(f"Could not request results; {e}")
    return None

# Mapping of voice commands to ESP32 endpoints
command_map = {
    "left": "move/left",
    "right": "move/right",
    "up": "move/up",
    "down": "move/down",
    "forward": "move/forward",
    "backward": "move/backward",
    "close": "grip/close",
    "open": "grip/open",
    "reset": "reset"
}

# Main loop
while True:
    command = recognize_speech()
    if command:
        for keyword, endpoint in command_map.items():
            if keyword in command:
                control_robot_arm(endpoint)
                break
    time.sleep(1)
