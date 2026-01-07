/*********** BLYNK DEFINES (MUST BE FIRST) ***********/
#define BLYNK_TEMPLATE_ID   "TMPL3B6SFBwPJ"
#define BLYNK_TEMPLATE_NAME "fishfeeder"
#define BLYNK_AUTH_TOKEN    "jKa1YkvRMHaIuDVatI5KCbsQQ59Uq7bf"

/*********** INCLUDES ***********/
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>
#include <time.h>

/*********** WIFI ***********/
char ssid[] = "Airtel_CNR";
char pass[] = "Nanikanni@3";

/*********** SERVO ***********/
Servo feederServo;
const int servoPin = 18;

/*********** BUZZER ***********/
const int buzzerPin = 27;

/*********** TIME ***********/
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;   // IST
const int daylightOffset_sec = 0;

bool morningDone = false;
bool nightDone   = false;

/*********** BEEP ***********/
void beep(int d) {
  digitalWrite(buzzerPin, HIGH);
  delay(d);
  digitalWrite(buzzerPin, LOW);
}

/*********** FEED FUNCTION ***********/
void feedFish(String source) {
  for (int i = 0; i < 3; i++) {
    feederServo.write(0);
    beep(200);
    delay(600);

    feederServo.write(180);
    beep(200);
    delay(600);
  }

  feederServo.write(0);
  beep(500);

  Blynk.logEvent("feeding_done", "Fish fed (" + source + ")");
}

/*********** MANUAL BUTTON ***********/
BLYNK_WRITE(V0) {
  if (param.asInt() == 1) {
    feedFish("Manual");
  }
}

/*********** SETUP ***********/
void setup() {
  Serial.begin(115200);

  pinMode(buzzerPin, OUTPUT);
  feederServo.attach(servoPin);
  feederServo.write(0);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

/*********** LOOP ***********/
void loop() {
  Blynk.run();

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  int h = timeinfo.tm_hour;
  int m = timeinfo.tm_min;

  if (h == 7 && m == 0 && !morningDone) {
    feedFish("Morning 7:00 AM");
    morningDone = true;
  }

  if (h == 20 && m == 20 && !nightDone) {
    feedFish("Night 8:20 PM");
    nightDone = true;
  }

  if (h == 0 && m == 0) {
    morningDone = false;
    nightDone = false;
  }

  delay(1000);
}
