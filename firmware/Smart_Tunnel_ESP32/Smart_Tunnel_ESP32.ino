#define BLYNK_TEMPLATE_ID "TMPLxxxxxxxxxxx"  // BLYNK Configuration
#define BLYNK_TEMPLATE_NAME "Tunnelxxxxxxx"   // BLYNK Configuration
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"  // BLYNK Configuration

#define BLYNK_PRINT Serial

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <BH1750.h>

BlynkTimer timer;

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// Pin Configuration

// MQ2 analog output
#define AIR_SENSOR_PIN 34

// L298N motor driver
// ENA/PWM pin
#define FAN_PWM_PIN 15

// IN1 direction pin
#define FAN_IN1_PIN 2

// If your L298N uses IN2, define it here.
// Leave -1 if IN2 is not connected.
#define FAN_IN2_PIN -1


// Blynk Virtual Pins

// V0 = Air quality sensor value
// V1 = Auto / Manual mode switch
// V2 = Light intensity
// V3 = Current fan speed
// V4 = Manual fan speed slider

#define VPIN_AIR_QUALITY V0
#define VPIN_MODE        V1
#define VPIN_LIGHT       V2
#define VPIN_FAN_SPEED   V3
#define VPIN_MANUAL      V4

// IMPORTANT:
// These values are raw ADC readings from the MQ2.
// They must be calibrated according to your sensor
// and actual tunnel environment.
//
// ESP32 ADC range: approximately 0 - 4095
//
// 0 - 490       : Clean
// 491 - 900     : Moderate
// 901 - 1500    : Poor
// > 1500        : Very poor
//
// Change these values after calibration.


const int AIR_CLEAN_THRESHOLD   = 490;
const int AIR_MODERATE_THRESHOLD = 900;
const int AIR_POOR_THRESHOLD     = 1500;

const int FAN_OFF      = 0;
const int FAN_LOW      = 90;
const int FAN_MEDIUM   = 170;
const int FAN_HIGH     = 255;


const float LIGHT_THRESHOLD = 1000.0;

int sensorValue = 0;

int currentFanSpeed = 0;
int manualFanSpeed = 0;

// true  = automatic fan control
// false = manual fan control
bool automaticMode = true;


unsigned long lastPollutionAlert = 0;
unsigned long lastLightAlert = 0;

const unsigned long ALERT_COOLDOWN = 60000UL; // 60 seconds


BH1750 lightMeter;

void setFanSpeed(int speed)
{
  speed = constrain(speed, 0, 255);

  currentFanSpeed = speed;

  // Set motor direction
  digitalWrite(FAN_IN1_PIN, HIGH);

  // Optional IN2
  if (FAN_IN2_PIN != -1)
  {
    digitalWrite(FAN_IN2_PIN, LOW);
  }

  // PWM speed control
  analogWrite(FAN_PWM_PIN, currentFanSpeed);

  // Display current fan speed on Blynk
  Blynk.virtualWrite(VPIN_FAN_SPEED, currentFanSpeed);

  Serial.print("Fan PWM: ");
  Serial.println(currentFanSpeed);
}


void automaticFanControl(int airValue)
{
  int newFanSpeed;


  if (airValue <= AIR_CLEAN_THRESHOLD)
  {
    newFanSpeed = FAN_OFF;
  }

  else if (airValue <= AIR_MODERATE_THRESHOLD)
  {
    newFanSpeed = FAN_LOW;
  }

  else if (airValue <= AIR_POOR_THRESHOLD)

  {
    newFanSpeed = FAN_MEDIUM;
  }
  else
  {
    newFanSpeed = FAN_HIGH;
  }

  // Only update PWM when speed actually changes
  if (newFanSpeed != currentFanSpeed)
  {
    setFanSpeed(newFanSpeed);
  }

  // Debug information
  Serial.print("Air Quality ADC: ");
  Serial.print(airValue);

  Serial.print(" | Automatic Fan Speed: ");
  Serial.println(newFanSpeed);
}

void checkPollutionAlert(int airValue)
{
  if (airValue > AIR_CLEAN_THRESHOLD)
  {
    unsigned long currentTime = millis();

    if (currentTime - lastPollutionAlert >= ALERT_COOLDOWN)
    {
      Blynk.logEvent(
        "polution_alert",
        "Poor air quality detected in tunnel!"
      );

      lastPollutionAlert = currentTime;

      Serial.println("Pollution alert sent.");
    }
  }
}

void checkLightLevel(float lux)
{
  if (lux < LIGHT_THRESHOLD)
  {
    unsigned long currentTime = millis();

    if (currentTime - lastLightAlert >= ALERT_COOLDOWN)
    {
      Blynk.logEvent(
        "lightdamage",
        "Tunnel light level is below the configured threshold."
      );

      lastLightAlert = currentTime;

      Serial.println("Low-light alert sent.");
    }
  }
}


void sendSensorData()
{
  sensorValue = analogRead(AIR_SENSOR_PIN);

  Serial.println("--------------------------------");

  Serial.print("Air Quality Sensor: ");
  Serial.println(sensorValue);

  Blynk.virtualWrite(
    VPIN_AIR_QUALITY,
    sensorValue
  );

  if (automaticMode)
  {
    automaticFanControl(sensorValue);
  }

  checkPollutionAlert(sensorValue);

  float lux = lightMeter.readLightLevel();

  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");

  Blynk.virtualWrite(
    VPIN_LIGHT,
    lux
  );

  checkLightLevel(lux);

  Serial.println("--------------------------------");
}

// Blynk Automatic / Manual Mode
// Blynk V1:
// 1 = Automatic
// 0 = Manual

BLYNK_WRITE(V1)
{
  int mode = param.asInt();

  if (mode == 1)
  {
    automaticMode = true;

    Serial.println("Fan mode: AUTOMATIC");

    automaticFanControl(sensorValue);
  }
  else
  {
    automaticMode = false;

    Serial.println("Fan mode: MANUAL");

    
    setFanSpeed(manualFanSpeed);
  }
}


// Blynk Manual Fan Speed


// Blynk V4 should be configured from 0 to 255.
// This control works only in MANUAL mode.


BLYNK_WRITE(V4)
{
  manualFanSpeed = constrain(param.asInt(), 0, 255);

  Serial.print("Manual fan speed: ");
  Serial.println(manualFanSpeed);

  if (!automaticMode)
  {
    setFanSpeed(manualFanSpeed);
  }
}


void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.println("=================================");
  Serial.println(" Smart Tunnel Monitoring System");
  Serial.println("=================================");

  pinMode(AIR_SENSOR_PIN, INPUT);

  pinMode(FAN_PWM_PIN, OUTPUT);

  pinMode(FAN_IN1_PIN, OUTPUT);

  if (FAN_IN2_PIN != -1)
  {
    pinMode(FAN_IN2_PIN, OUTPUT);
  }

  digitalWrite(FAN_IN1_PIN, HIGH);

  if (FAN_IN2_PIN != -1)
  {
    digitalWrite(FAN_IN2_PIN, LOW);
  }

  analogWrite(FAN_PWM_PIN, 0);

  Wire.begin();

  if (lightMeter.begin())
  {
    Serial.println("BH1750 initialized successfully.");
  }
  else
  {
    Serial.println("ERROR: BH1750 initialization failed!");
  }

  Serial.println("Connecting to Blynk...");

  Blynk.begin(
    BLYNK_AUTH_TOKEN,
    ssid,
    pass
  );

  Serial.println("Blynk connected.");

  setFanSpeed(0);


  timer.setInterval(
    1000L,
    sendSensorData
  );

  Serial.println("System started.");
}

void loop()
{
  Blynk.run();

  timer.run();
}