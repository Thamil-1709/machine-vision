//USES ESP32 AND RaspberryPi!!!(IT'S A PLACEHOLDER DON'T WORRY ;})
#include <esp_now.h>
#include <WiFi.h>

// Define motor control pins
int motor_forward = 5;
int motor_reverse = 6;

// Define IR sensor input pin
int irSensor = 4;

// Variable to store Raspberry Pi result
String raspberryPiResult = "";

// ESP-NOW peer info
esp_now_peer_info_t peerInfo;
uint8_t peerAddress[] = {0x24, 0x0A, 0xC4, 0xB8, 0x4F, 0x21}; // Replace with MAC address of the other ESP32

void setup() {
  // Set motor pins as OUTPUT
  pinMode(motor_forward, OUTPUT);
  pinMode(motor_reverse, OUTPUT);

  // Set IR sensor pin as INPUT
  pinMode(irSensor, INPUT);

  // Initialize Serial communication for Raspberry Pi
  Serial.begin(115200);

  // Initialize motor to be stopped
  stopMotor();

  // Initialize WiFi and ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Add the other ESP32 as a peer
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Read IR sensor state
  int irSensorState = digitalRead(irSensor);

  // If an object is detected, request image detection from Raspberry Pi
  if (irSensorState == HIGH) {
    Serial.println("Requesting image detection from Raspberry Pi...");
    Serial.println("DETECT_IMAGE"); // Command sent to Raspberry Pi for detection

    // Wait for Raspberry Pi to respond
    if (Serial.available()) {
      raspberryPiResult = Serial.readStringUntil('\n');
      Serial.println("Raspberry Pi Result: " + raspberryPiResult);

      // Based on the result, send message to the other ESP32 using ESP-NOW
      if (raspberryPiResult == "fresh") {
        sendESPNowMessage("fresh");
        driveMotorForward();  // Motor drives forward if fresh
      } else if (raspberryPiResult == "not fresh") {
        sendESPNowMessage("not fresh");
        stopMotor();  // Stop motor if not fresh
      }
    }
  } else {
    stopMotor();
  }

  delay(100); // Small delay for stability
}

// Function to drive motor forward
void driveMotorForward() {
  digitalWrite(motor_forward, HIGH);  // Turn Motor forward pin HIGH
  digitalWrite(motor_reverse, LOW);   // Ensure reverse is LOW
}

// Function to stop the motor
void stopMotor() {
  digitalWrite(motor_forward, LOW);   // Turn off forward pin
  digitalWrite(motor_reverse, LOW);   // Ensure reverse pin is LOW
}

// Function to send a message using ESP-NOW
void sendESPNowMessage(String message) {
  const char *msg = message.c_str();
  esp_err_t result = esp_now_send(peerAddress, (uint8_t *)msg, strlen(msg));

  if (result == ESP_OK) {
    Serial.println("Sent ESP-NOW message: " + message);
  } else {
    Serial.println("Error sending ESP-NOW message");
  }
}

// ESP-NOW callback (optional, used for debugging)
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\nLast Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
