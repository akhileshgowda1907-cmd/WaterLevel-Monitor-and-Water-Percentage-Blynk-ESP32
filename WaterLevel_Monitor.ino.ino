#define BLYNK_TEMPLATE_ID "TMPL3_zIGHGA9"
#define BLYNK_TEMPLATE_NAME "Smart automation"
#define BLYNK_AUTH_TOKEN "U1DWMibQqNZ4yQBRSSt0ItTpJg_mKzHX"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi Credentials
char ssid[] = "Akhil";
char pass[] = "11111111";

// Ultrasonic Sensor Pins
#define TRIG_PIN 5
#define ECHO_PIN 18

BlynkTimer timer;

// Enter your tank height in cm
float tankHeight = 200.0;

// Minimum valid distance
float minDistance = 20.0;

// Maximum valid distance
float maxDistance = 200.0;

void measureWaterLevel()
{
  long duration;
  float distance;
  float waterLevel;
  float waterPercent;

  // Clear trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send ultrasonic pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo time
  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // If no echo received
  if (duration == 0)
  {
    Serial.println("No Echo Received");
    return;
  }

  // Calculate distance in cm
  distance = duration * 0.0343 / 2;

  // Remove invalid readings
  if (distance < minDistance || distance > maxDistance)
  {
    Serial.println("Invalid Distance");
    return;
  }

  // Water level calculation
  waterLevel = tankHeight - distance;

  // Limit water level
  if (waterLevel < 0)
    waterLevel = 0;

  if (waterLevel > tankHeight)
    waterLevel = tankHeight;

  // Calculate percentage
  waterPercent = (waterLevel / tankHeight) * 100;

  // Send to Blynk
  Blynk.virtualWrite(V0, waterLevel);     // Water level in cm
  Blynk.virtualWrite(V1, waterPercent);   // Water percentage

  // Serial Monitor Output
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println(" cm");

  Serial.print("Water Percentage: ");
  Serial.print(waterPercent);
  Serial.println(" %");

  Serial.println("---------------------");
}

void setup()
{
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Run every 1 second
  timer.setInterval(1000L, measureWaterLevel);
}

void loop()
{
  Blynk.run();
  timer.run();
}