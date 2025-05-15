import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'  # 0 = all logs, 1 = info, 2 = warnings, 3 = errors

import cv2
import mediapipe as mp
import math
import requests
import time

# ESP32 Base URL
ESP32_IP = "http://192.168.69.217"  # Replace with your ESP32 IP

def control_robot_arm(command):
    url = f"{ESP32_IP}/cart/{command}"
    try:
        response = requests.get(url)
        print(f"Sent command: {command}, ESP32 Response: {response.text}")
    except Exception as e:
        print(f"Failed to send command: {command}, Error: {e}")

# Mediapipe setup
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

# State variables
prev_command = ""
last_hand_detected = time.time()
active_zone = None
pinch_threshold = 40  # pixels (you can tune this)

# Open camera
cap = cv2.VideoCapture(0)

# Define button zones: (x, y, width, height)
arrow_zones = {
    "move/down":        (900, 400, 150, 60),
    "move/up":      (900, 620, 150, 60),
    "move/left":      (755, 510, 60, 100),
    "move/right":     (1110, 510, 60, 100),
    "move/forward":   (900, 230, 150, 50),
    "move/backward":  (900, 800, 150, 50),
    "grip/open":      (100, 830, 100, 50),
    "grip/close":     (420, 830, 100, 50)
}

with mp_hands.Hands(min_detection_confidence=0.7, min_tracking_confidence=0.7) as hands:
    while cap.isOpened():
        success, frame = cap.read()
        if not success:
            continue

        frame = cv2.flip(frame, 1)
        h, w, _ = frame.shape

        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        results = hands.process(frame_rgb)
        frame = cv2.cvtColor(frame_rgb, cv2.COLOR_RGB2BGR)

        # Draw control zones
        for direction, (x, y, box_w, box_h) in arrow_zones.items():
            color = (255, 100, 100) if "grip" in direction else (0, 255, 255)
            cv2.rectangle(frame, (x, y), (x + box_w, y + box_h), color, 2)
            label = direction.split('/')[-1].upper()
            cv2.putText(frame, label, (x + 5, y + box_h // 2 + 5),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.7, color, 2)

        if results.multi_hand_landmarks:
            hand = results.multi_hand_landmarks[0]
            landmarks = hand.landmark

            # Get coordinates
            index_tip = landmarks[mp_hands.HandLandmark.INDEX_FINGER_TIP]
            thumb_tip = landmarks[mp_hands.HandLandmark.THUMB_TIP]

            index_x, index_y = int(index_tip.x * w), int(index_tip.y * h)
            thumb_x, thumb_y = int(thumb_tip.x * w), int(thumb_tip.y * h)

            # Draw tips
            cv2.circle(frame, (index_x, index_y), 8, (0, 255, 0), -1)
            cv2.circle(frame, (thumb_x, thumb_y), 8, (0, 0, 255), -1)

            # Calculate pinch distance
            pinch_distance = math.hypot(index_x - thumb_x, index_y - thumb_y)

            if pinch_distance < pinch_threshold:
                if prev_command != "pinch":
                    control_robot_arm("pinch")
                    prev_command = "pinch"
            else:
                # Zone detection only if not pinching
                new_zone = None
                for dir_cmd, (x, y, box_w, box_h) in arrow_zones.items():
                    if x <= index_x <= x + box_w and y <= index_y <= y + box_h:
                        new_zone = dir_cmd
                        break

                if new_zone != active_zone:
                    if new_zone:
                        control_robot_arm(new_zone)
                        prev_command = new_zone
                    active_zone = new_zone

            mp_drawing.draw_landmarks(frame, hand, mp_hands.HAND_CONNECTIONS)
            last_hand_detected = time.time()

        else:
            if time.time() - last_hand_detected > 1:
                if prev_command != "reset":
                    control_robot_arm("reset")
                    prev_command = "reset"
                active_zone = None

        # Center dot
        cv2.circle(frame, (int(w / 2), int(h / 2)), 10, (255, 255, 0), -1)

        cv2.imshow('Gesture Robot Control', frame)
        if cv2.waitKey(5) & 0xFF == 27:
            break

cap.release()
cv2.destroyAllWindows()
