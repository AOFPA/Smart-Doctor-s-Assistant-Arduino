#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "virus"
#define WIFI_PASSWORD "11111111"

#define FIREBASE_HOST "tranquil-rite-321405-default-rtdb.firebaseio.com"
#define FIREBASE_KEY "oJxQ6DVfBcXLOwknyzXr8Ku91nJcMIw8KIN8dvEz"

#define TRIGGER_PIN  D1
#define ECHO_PIN     D2


FirebaseData firebaseData;


void setup() {
  connectWifi();
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
}

void loop() {
  delay(1000);
  double duration, distance;
  digitalWrite(TRIGGER_PIN, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print(distance);
  Serial.println(" cm");

  FirebaseJson data;
  data.set("distance", distance);

  if (Firebase.setJSON(firebaseData, "/wio-link-01", data)) {
    Serial.println("Added");
  } else {
    Serial.println("Error : " + firebaseData.errorReason());
  }
}

void connectWifi() {
  Serial.begin(115200);
  Serial.println(WiFi.localIP());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
