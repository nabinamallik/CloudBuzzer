#include <WiFi.h>
#include <FirebaseESP32.h>

// Replace these with your network credentials
#define WIFI_SSID "Mr Lee"
#define WIFI_PASSWORD "mrlee2.0"

// Firebase project credentials
FirebaseConfig config;
FirebaseAuth auth;

// Firebase data object
FirebaseData firebaseData;

const int buzzerPin = 19;

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi.");

  // Set Firebase configuration
  config.host = "buzzer-72273-default-rtdb.asia-southeast1.firebasedatabase.app";
  config.api_key = "AIzaSyClmdSDU0xx8xQ94fZ8IFxjYY6-QJeOn4I";
  
  // Optional: Assign the user authentication data if needed
  auth.user.email = "nabinamallik2003@gmail.com";
  auth.user.password = "nabinamallik";
  
  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Start listening to changes in the "/buzzerStatus" node
  if (!Firebase.beginStream(firebaseData, "/buzzerStatus")) {
    Serial.println("Could not start stream");
    Serial.println(firebaseData.errorReason());
  }

  // Set callback functions
  Firebase.setStreamCallback(firebaseData, streamCallback, streamTimeoutCallback);
}

void loop() {
  // Nothing needed here as the stream callback will handle everything
}

// This will be triggered when the Firebase value changes
void streamCallback(StreamData data) {
  if (data.dataType() == "int") {
    int buzzerState = data.intData();
    digitalWrite(buzzerPin, buzzerState);
    Serial.println(buzzerState == HIGH ? "Buzzer ON" : "Buzzer OFF");
  }
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) {
    Serial.println("Stream timeout, reconnecting...");
    Firebase.beginStream(firebaseData, "/buzzerStatus");
  }
}
