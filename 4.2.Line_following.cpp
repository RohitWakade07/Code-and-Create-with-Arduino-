// ================= MOTOR DRIVER (L298N) PIN DEFINITIONS =================
#define ENA 5    // Enable pin for Motor A (PWM speed control)
#define IN1 6    // Motor A input 1
#define IN2 7    // Motor A input 2

#define ENB 9    // Enable pin for Motor B (PWM speed control)
#define IN3 10   // Motor B input 1
#define IN4 11   // Motor B input 2

// ================= IR SENSOR PINS =================
#define LEFT_IR A0   // Left IR sensor digital output
#define RIGHT_IR A1  // Right IR sensor digital output

// ================= SETUP =================
void setup() {
  // Motor pins as OUTPUT
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // IR sensor pins as INPUT
  pinMode(LEFT_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);

  // Set motor speeds (0–255)
  analogWrite(ENA, 150);  // Motor A speed
  analogWrite(ENB, 150);  // Motor B speed

  Serial.begin(9600);     // Debugging
}

// ================= MOTOR CONTROL FUNCTIONS =================
void moveForward() {
  // Both motors forward
  digitalWrite(IN1, HIGH);  
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnLeft() {
  // Left motor stop, right motor forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  // Right motor stop, left motor forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  // Both motors stop
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ================= MAIN LOOP =================
void loop() {
  int leftSensor = digitalRead(LEFT_IR);    // Read left IR sensor
  int rightSensor = digitalRead(RIGHT_IR);  // Read right IR sensor

  Serial.print("Left: ");
  Serial.print(leftSensor);
  Serial.print(" | Right: ");
  Serial.println(rightSensor);

  // Assume: IR sensor gives 0 on BLACK line, 1 on WHITE surface
  if (leftSensor == 0 && rightSensor == 0) {
    moveForward();   // Both sensors on line → go forward
  }
  else if (leftSensor == 0 && rightSensor == 1) {
    turnLeft();      // Left sensor on line, right off → turn left
  }
  else if (leftSensor == 1 && rightSensor == 0) {
    turnRight();     // Right sensor on line, left off → turn right
  }
  else {
    stopMotors();    // No line detected → stop
  }
}
