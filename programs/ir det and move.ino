//USES AN ESP32!!!
// Define motor control pins
int motor_forward = 5;
int motor_reverse = 6;

// Define IR sensor input pin
int irSensor = 4;

void setup() {
  // Set motor pins as OUTPUT
  pinMode(motor_forward, OUTPUT);
  pinMode(motor_reverse, OUTPUT);

  // Set IR sensor pin as INPUT
  pinMode(irSensor, INPUT);

  // Initialize motor to be stopped
  stopMotor();
}

void loop() {
  // Read IR sensor state
  int irSensorState = digitalRead(irSensor);

  // If an object is detected (sensor HIGH), move motor forward
  if (irSensorState == HIGH) {
    driveMotorForward();
  } else {
    stopMotor(); // Otherwise, stop the motor
  }

  delay(100); // Small delay for stability
}

void driveMotorForward() {
  digitalWrite(motor_forward, HIGH);  // Turn Motor forward pin HIGH
  digitalWrite(motor_reverse, LOW);   // Ensure reverse is LOW
}

void stopMotor() {
  digitalWrite(motor_forward, LOW);   // Turn off forward pin
  digitalWrite(motor_reverse, LOW);   // Ensure reverse pin is LOW
}

