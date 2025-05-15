#define BLYNK_TEMPLATE_ID           "TMPL3jSMR7ZRP"
#define BLYNK_TEMPLATE_NAME         "servos control"
#define BLYNK_PRINT Serial
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

Adafruit_PWMServoDriver srituhobby = Adafruit_PWMServoDriver(0x40);

#define servoMIN 150
#define servoMAX 600


// Enter your Auth token
char auth[] = "dmRQRO0gsAZIHUm5VOPQ03hFjcR2Piwo";

//Enter your WIFI SSID and password
char ssid[] = "StarkTech_";
char pass[] = "downtillearth";

void setup() {
  Serial.begin(9600);
  srituhobby.begin();
  srituhobby.setPWMFreq(60);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  value = map(value, 0, 180, servoMIN, servoMAX);
  srituhobby.setPWM(0, 0, value);
  delay(1);
}
BLYNK_WRITE(V1) {
  int value = param.asInt();
  value = map(value, 0, 180, servoMIN, servoMAX);
  srituhobby.setPWM(1, 0, value);
  delay(1);
}
BLYNK_WRITE(V2) {
  int value = param.asInt();
  value = map(value, 0, 180, servoMIN, servoMAX);
  srituhobby.setPWM(2, 0, value);
  delay(1);
}
BLYNK_WRITE(V3) {
  int value = param.asInt();
  value = map(value, 0, 180, servoMIN, servoMAX);
  srituhobby.setPWM(3, 0, value);
  delay(1);
}

BLYNK_WRITE(V4) {
  int value = param.asInt();
  value = map(value, 0, 180, servoMIN, servoMAX);
  srituhobby.setPWM(4, 0, value);
  delay(1);
}
void loop() {
  Blynk.run();
}