#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <Adafruit_PWMServoDriver.h>

// Wi-Fi credentials
const char* ssid = "StarkTech_";
const char* password = "downtillearth";

AsyncWebServer server(80);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

const int SERVO_MIN = 150;
const int SERVO_MAX = 600;

#define HAND_SERVO      0
#define ELBOW_SERVO     2
#define SHOULDER_SERVO  3
#define BASE_SERVO      4

int handAngle = 20;
int elbowAngle = 75;
int shoulderAngle = 75;
int baseAngle = 75;

int angleToPulse(int angle) {
  return map(angle, 0, 180, SERVO_MIN, SERVO_MAX);
}

int clamp(int val, int minVal, int maxVal) {
  return max(min(val, maxVal), minVal);
}

void moveServo(uint8_t channel, int angle) {
  pwm.setPWM(channel, 0, angleToPulse(angle));
}

void setup() {
  Serial.begin(115200);

  pwm.begin();
  pwm.setPWMFreq(50);
  delay(10);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.println(WiFi.localIP());

    // Set all servos to initial position
    moveServo(HAND_SERVO, handAngle);
    moveServo(ELBOW_SERVO, elbowAngle);
    moveServo(SHOULDER_SERVO, shoulderAngle);
    moveServo(BASE_SERVO, baseAngle);

    server.on("/ping", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(200, "text/plain", "Server is alive!");
    });
    
    server.on("/cart/move/right", HTTP_GET, [](AsyncWebServerRequest *request){
      baseAngle = clamp(baseAngle - 10, 0, 180);
      moveServo(BASE_SERVO, baseAngle);
      Serial.printf("Base Right → %d°\n", baseAngle);
      request->send(200, "text/plain", "Base turned right 10 degrees");
    });

    server.on("/cart/move/left", HTTP_GET, [](AsyncWebServerRequest *request){
      baseAngle = clamp(baseAngle + 10, 0, 180);
      moveServo(BASE_SERVO, baseAngle);
      Serial.printf("Base Left → %d°\n", baseAngle);
      request->send(200, "text/plain", "Base turned left 10 degrees");
    });

    server.on("/cart/move/up", HTTP_GET, [](AsyncWebServerRequest *request){
      elbowAngle = clamp(elbowAngle - 10, 0, 180);
      moveServo(ELBOW_SERVO, elbowAngle);
      Serial.printf("Elbow Up → %d°\n", elbowAngle);
      request->send(200, "text/plain", "Elbow moved up 10 degrees");
    });

    server.on("/cart/move/down", HTTP_GET, [](AsyncWebServerRequest *request){
      elbowAngle = clamp(elbowAngle + 10, 0, 180);
      moveServo(ELBOW_SERVO, elbowAngle);
      Serial.printf("Elbow Down → %d°\n", elbowAngle);
      request->send(200, "text/plain", "Elbow moved down 10 degrees");
    });

    server.on("/cart/move/forward", HTTP_GET, [](AsyncWebServerRequest *request){
      shoulderAngle = clamp(shoulderAngle + 10, 0, 180);
      moveServo(SHOULDER_SERVO, shoulderAngle);
      Serial.printf("Shoulder Forward → %d°\n", shoulderAngle);
      request->send(200, "text/plain", "Shoulder moved forward 10 degrees");
    });

    server.on("/cart/move/backward", HTTP_GET, [](AsyncWebServerRequest *request){
      shoulderAngle = clamp(shoulderAngle - 10, 0, 180);
      moveServo(SHOULDER_SERVO, shoulderAngle);
      Serial.printf("Shoulder Backward → %d°\n", shoulderAngle);
      request->send(200, "text/plain", "Shoulder moved backward 10 degrees");
    });
    

    server.on("/cart/grip/close", HTTP_GET, [](AsyncWebServerRequest *request){
      handAngle = clamp(handAngle - 20, 0, 180);
      moveServo(HAND_SERVO, handAngle);
      Serial.printf("Hand close → %d°\n", handAngle);
      request->send(200, "text/plain", "Hand opened 20 degrees");
    });

    server.on("/cart/grip/open", HTTP_GET, [](AsyncWebServerRequest *request){
      handAngle = clamp(handAngle + 20, 0, 180);
      moveServo(HAND_SERVO, handAngle);
      Serial.printf("Hand open → %d°\n", handAngle);
      request->send(200, "text/plain", "Hand closed 20 degrees");
    });

    

    server.on("/cart/reset", HTTP_GET, [](AsyncWebServerRequest *request){
      handAngle = 20;
      baseAngle = elbowAngle = shoulderAngle = 75;
      moveServo(BASE_SERVO, baseAngle);
      moveServo(ELBOW_SERVO, elbowAngle);
      moveServo(SHOULDER_SERVO, shoulderAngle);
      moveServo(HAND_SERVO, handAngle);
      Serial.println("All motors reset to 75° and hand to 20° ");
      request->send(200, "text/plain", "All motors reset to 75 degrees and hand to 20 degrees");
    });

    server.begin();
    Serial.println("Server started");
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }
}

void loop() {
  // Nothing needed here
}
