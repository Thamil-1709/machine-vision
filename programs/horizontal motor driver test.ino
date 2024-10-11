/*
MOTORA    IN1   |   IN2   |   ENA
------------------------------------
Forward   HIGH  |   LOW   |   255
------------------------------------
Reverse   LOW   |   HIGH  |   255
------------------------------------
STOP      LOW   |   LOW   |   0
------------------------------------

MOTORB    IN3   |   IN4   |   ENB
------------------------------------
Forward   HIGH  |   LOW   |   255
------------------------------------
Reverse   LOW   |   HIGH  |   255
------------------------------------
STOP      LOW   |   LOW   |   0
------------------------------------
*/


// Motor A, Left Side
const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;
// Motor B, Right Side
const int IN3 = 6;
const int IN4 = 5;
const int ENB = 3;

void stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void moveForward(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void moveReverse(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}

void increaseForward(){
  for (int i=0; i<256; i++){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, i);
    analogWrite(ENB, i);
    delay(20);
  }
}

void setup() {
  pinMode(ENA, OUTPUT); //ENA Enable Pin
  pinMode(IN1, OUTPUT); //IN1
  pinMode(IN2, OUTPUT); //IN2
  pinMode(IN3, OUTPUT); //IN3
  pinMode(IN4, OUTPUT); //IN4
  pinMode(ENB, OUTPUT); //ENB Enable Pin
}

void loop(){
  stop(); // Stop DC Motors
  delay(3000);
  moveForward(); // Drive DC Motors Forward
  delay(3000);
  moveReverse(); // Drive DC Motors Reverse
  delay(3000);
  increaseForward(); // Drive DC Motors 0 to 100 Forward
  delay(3000);
}
