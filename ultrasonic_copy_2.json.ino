#include <ESP8266WiFi.h>   // Correct for ESP8266
#include "ThingSpeak.h"


// WiFi Credentials
const char* ssid = "V2065";
const char* password = "vivo2065";

// ThingSpeak Settings
unsigned long channelID = 2938261; // Example: 1234567
const char* writeAPIKey = "KQUZV3ZDWGFW640Y";

WiFiClient client;

// Ultrasonic Sensor Pins
const int trigPin = 5;
const int echoPin = 4;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  ThingSpeak.begin(client);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  long duration;
  int distance;

  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;  // Convert to cm

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  String parkingStatus = (distance < 30) ? "Occupied" : "Available";
if (distance>100 )
  {
  ThingSpeak.setField(1, distance);
  }
  int x = ThingSpeak.writeFields(channelID, writeAPIKey);
  
  if (x == 200) {
    Serial.println("Update Successful");
  } else {
    Serial.println("Problem Updating. HTTP error code " + String(x));
  }

  delay(1000); // Wait 15 sec before next reading
}
