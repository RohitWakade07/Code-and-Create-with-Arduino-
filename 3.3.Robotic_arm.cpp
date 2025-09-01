// Wireless Bluetooth Robot using HC-05 + Arduino + L298N Motor Driver

char command;  // Variable to store received Bluetooth command

// Motor driver pins
int ENA = 9;    // Enable pin for Motor A (speed)
int IN1 = 8;    // Motor A direction pin 1
int IN2 = 7;    // Motor A direction pin 2
int ENB = 10;   // Enable pin for Motor B (speed)
int IN3 = 6;    // Motor B direction pin 1
int IN4 = 5;    // Motor B direction pin 2

void setup() {
  Serial.begin(9600); // Start serial communication with HC-05 at 9600 baud

  // Set motor pins as output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Stop motors initially
  stopMotors();
}

void loop() {
  // Check if data is received from Bluetooth
  if (Serial.available() > 0) {
    command = Serial.read();  // Read the character

    // Match command and take action
    if (command == 'F') { forward(); }
    else if (command == 'B') { backward(); }
    else if (command == 'L') { left(); }
    else if (command == 'R') { right(); }
    else if (command == 'S') { stopMotors(); }
  }
}

// ----------------- Functions -----------------
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);  // Speed (0-255)
  analogWrite(ENB, 200);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
