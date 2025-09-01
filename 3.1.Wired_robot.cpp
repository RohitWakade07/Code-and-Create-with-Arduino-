// Wired Two-Wheel Robot with Joystick

// Motor driver pins
int ENA = 9;    // Enable pin for Motor A (speed control)
int IN1 = 8;    // Motor A direction pin 1
int IN2 = 7;    // Motor A direction pin 2
int ENB = 10;   // Enable pin for Motor B (speed control)
int IN3 = 6;    // Motor B direction pin 1
int IN4 = 5;    // Motor B direction pin 2

// Joystick pins
int joyX = A0;  // Joystick X-axis
int joyY = A1;  // Joystick Y-axis

void setup() {
  // Motor pins as OUTPUT
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize motors OFF
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  int xValue = analogRead(joyX); // Read X-axis
  int yValue = analogRead(joyY); // Read Y-axis

  int motorSpeed = map(abs(yValue - 512), 0, 512, 0, 255); // speed control

  // Forward
  if (yValue > 600) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed);
  }
  // Backward
  else if (yValue < 400) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, motorSpeed);
    analogWrite(ENB, motorSpeed);
  }
  // Left
  else if (xValue < 400) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 200);
    analogWrite(ENB, 200);
  }
  // Right
  else if (xValue > 600) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, 200);
    analogWrite(ENB, 200);
  }
  // Stop (joystick at center)
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}
