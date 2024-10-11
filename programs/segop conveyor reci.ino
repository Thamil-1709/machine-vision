//USES AN ARDUINO UNO!!!
// Define motor pins
int motorA_forward = 5;
int motorA_reverse = 6;
int motorB_forward = 7;
int motorB_reverse = 8;

// Define input pins
int input1 = 2; // Control input for Motor A
int input2 = 3; // Control input for Motor B

void setup() {
  // Set motor pins as OUTPUT
  pinMode(motorA_forward, OUTPUT);
  pinMode(motorA_reverse, OUTPUT);
  pinMode(motorB_forward, OUTPUT);
  pinMode(motorB_reverse, OUTPUT);

  // Set input pins as INPUT
  pinMode(input1, INPUT);
  pinMode(input2, INPUT);

  // Initialize motors to be off
  stopMotors();
}

void loop() {
  // Read input states
  int input1State = digitalRead(input1);
  int input2State = digitalRead(input2);

  // Control Motor A based on Input 1
  if (input1State == HIGH) {
    driveMotorAForward(); // Input 1 HIGH -> Motor A forward
  } else {
    driveMotorAReverse(); // Input 1 LOW -> Motor A reverse
  }

  // Control Motor B based on Input 2
  if (input2State == HIGH) {
    driveMotorBForward(); // Input 2 HIGH -> Motor B forward
  } else {
    driveMotorBReverse(); // Input 2 LOW -> Motor B reverse
  }

  delay(100); // Small delay for stability
}

void driveMotorAForward() {
  digitalWrite(motorA_forward, HIGH);  // Turn Motor A forward pin HIGH
  digitalWrite(motorA_reverse, LOW);   // Ensure reverse is LOW
}

void driveMotorAReverse() {
  digitalWrite(motorA_forward, LOW);   // Turn Motor A forward pin LOW
  digitalWrite(motorA_reverse, HIGH);  // Turn reverse pin HIGH
}

void driveMotorBForward() {
  digitalWrite(motorB_forward, HIGH);  // Turn Motor B forward pin HIGH
  digitalWrite(motorB_reverse, LOW);   // Ensure reverse is LOW
}

void driveMotorBReverse() {
  digitalWrite(motorB_forward, LOW);   // Turn Motor B forward pin LOW
  digitalWrite(motorB_reverse, HIGH);  // Turn reverse pin HIGH
}

void stopMotors() {
  // Stop both motors initially
  digitalWrite(motorA_forward, LOW);
  digitalWrite(motorA_reverse, LOW);
  digitalWrite(motorB_forward, LOW);
  digitalWrite(motorB_reverse, LOW);
}

