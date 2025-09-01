// Obstacle Avoidance Robot using Ultrasonic Sensor + L298N

// Motor driver pins
int ENA = 9;    // Enable pin for Motor A
int IN1 = 8;    // Motor A direction pin 1
int IN2 = 7;    // Motor A direction pin 2
int ENB = 10;   // Enable pin for Motor B
int IN3 = 6;    // Motor B direction pin 1
int IN4 = 5;    // Motor B direction pin 2

// Ultrasonic Sensor pins
int trigPin = 11; 
int echoPin = 12;

long duration;
int distance;

void setup() {
  // Motor setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Ultrasonic setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);  // Debugging
}

void loop() {
  // Measure distance
  distance = getDistance();
  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance > 20) {
    // No obstacle nearby → move forward
    forward();
  } else {
    // Obstacle detected → stop & turn
    stopMotors();
    delay(300);

    backward();   // Move back a little
    delay(400);
    stopMotors();

    left();       // Turn left
    delay(500);
    stopMotors();
  }
}

// ----------------- Functions -----------------
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
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

int getDistance() {
  // Send trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pulse
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in cm
  int dist = duration * 0.034 / 2;
  return dist;
}
