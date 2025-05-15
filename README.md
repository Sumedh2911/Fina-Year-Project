# Final-Year-Project
Designed and developed a smart robotic arm integrating AI, IoT, and embedded systems for industrial and assistive automation applications. The system enables multi-modal control using hand gesture recognition (via MediaPipe) and voice commands.
# 🤖 Industrial Technology 4.0-Based Robotic Arm

A smart robotic arm system integrating Artificial Intelligence, IoT, and embedded hardware for industrial and assistive automation. This project showcases multi-modal control using both **hand gesture recognition** and **voice commands** to perform actions like sorting, lifting, and manipulating objects.

---

## 📌 Features

- ✋ **Gesture Control** using MediaPipe (real-time hand tracking)
- 🎙 **Voice Command Integration** with SpeechRecognition (Python)
- 📡 **Wireless Communication** via ESP32 microcontroller (Wi-Fi)
- 🧠 **Python-based logic** for interpreting commands and controlling movements
- 🎯 **Precision Control** using PCA9685 Servo Driver
- 🔌 **Modular Hardware Setup** for easy expansion and upgrades

---

## 🛠 Technologies & Tools

| Category       | Tools & Components                         |
|----------------|--------------------------------------------|
| Programming    | Python, C++, Arduino IDE                   |
| AI/ML          | MediaPipe (Hand Gesture Detection)         |
| Hardware       | ESP32, PCA9685, Servo Motors               |
| Communication  | Wi-Fi (HTTP GET via ESP32)                |
| Voice Input    | SpeechRecognition Library (Python)         |
| IDEs           | PyCharm, VS Code, Arduino IDE              |

---

## 🧠 System Architecture

graph 
    Camera -->|MediaPipe| Gesture_Module
    Microphone -->|SpeechRecognition| Voice_Module
    Gesture_Module -->|Python| Command_Handler
    Voice_Module -->|Python| Command_Handler
    Command_Handler -->|HTTP Request| ESP32
    ESP32 -->|PWM Signals| PCA9685
    PCA9685 --> Servo_Motors

---
## 👨‍💻 Author

**Sumedh Dhangar**  
[LinkedIn](https://www.linkedin.com/in/sumedh-dhangar) | [GitHub](https://github.com/Sumedh2911)

    

