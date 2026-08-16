#include <Arduino.h>
#include "MotorControl.h"

// Each motor needs two direction pins (H-bridge IN1/IN2) and one
// PWM-capable enable pin. Motors are grouped by side (left/right)
// because a skid-steer chassis always drives every motor on a side
// identically -- this is what lets the same drive() mixer work for
// 2WD (MOTORS_PER_SIDE = 1) or 4WD (MOTORS_PER_SIDE = 2, front + rear)
// without any other code changes.
//
// The original 2-motor build only needs a single L298N (one channel
// per side), and the pin numbers below for "front" match that original
// wiring. A 4WD build needs a second driver board (or a 4-channel
// driver) wired to the "rear" pins -- update those to match your
// actual wiring.
//
// NOTE: GPIO12 (used below for the front-left enable pin) is an ESP32
// boot "strapping" pin (MTDI). If your board fails to boot with a
// driver wired to it, move that connection to a different
// PWM-capable pin instead.

struct MotorPins {
  uint8_t pin1;
  uint8_t pin2;
  uint8_t enablePin;
};

static MotorPins leftMotors[MOTORS_PER_SIDE] = {
{14, 27, 12}, // Front-left (same pins as the original "motor1")
#if MOTORS_PER_SIDE > 1
{32, 15, 13}, // Rear-left -- update to match your second driver board
#endif
};

static MotorPins rightMotors[MOTORS_PER_SIDE] = {
{26, 25, 33}, // Front-right (same pins as the original "motor2")
#if MOTORS_PER_SIDE > 1
{17, 16, 4}, // Rear-right -- update to match your second driver board
#endif
};

// Default drive speed (0-255). Lower this if the robot moves too fast.
static const int MAX_SPEED = 225;

static void setupSide(MotorPins *motors) {
    for (uint8_t i = 0; i < MOTORS_PER_SIDE; i++) {
    pinMode(motors[i].pin1, OUTPUT);
    pinMode(motors[i].pin2, OUTPUT);
    pinMode(motors[i].enablePin, OUTPUT);
    }
}

// Drives every motor on one side at the same signed speed.
// speed > 0 -> forward, speed < 0 -> reverse, 0 -> coast.
static void setSideSpeed(MotorPins *motors, int speed) {
    bool goForward = speed >= 0;
  int magnitude = speed < 0 ? -speed : speed;
  uint8_t pwm = magnitude > 255 ? 255 : (uint8_t)magnitude;

  for (uint8_t i = 0; i < MOTORS_PER_SIDE; i++) {
    digitalWrite(motors[i].pin1, goForward ? LOW : HIGH);
    digitalWrite(motors[i].pin2, goForward ? HIGH : LOW);
    analogWrite(motors[i].enablePin, pwm);
  }
}

void setupMotorPins() {
    setupSide(leftMotors);
  setupSide(rightMotors);
}

void stopCar() {
    Serial.println("Stop");
  setSideSpeed(leftMotors, 0);
  setSideSpeed(rightMotors, 0);
}

// Tank-style mixer: combines a forward/back throttle with a left/right
// steering value into independent left/right side speeds. Every motor
// on a side is always driven identically, so this same function drives
// 2WD or 4WD chassis without changes.
//   throttle: -255 (full reverse) .. 255 (full forward)
//   steering: -255 (full left) .. 255 (full right)
void drive(int throttle, int steering) {
    if (throttle > 255) throttle = 255;
  if (throttle < -255) throttle = -255;
  if (steering > 255) steering = 255;
  if (steering < -255) steering = -255;

  int leftSpeed = throttle + steering;
  int rightSpeed = throttle - steering;

  if (leftSpeed > 255) leftSpeed = 255;
  if (leftSpeed < -255) leftSpeed = -255;
  if (rightSpeed > 255) rightSpeed = 255;
  if (rightSpeed < -255) rightSpeed = -255;

  setSideSpeed(leftMotors, leftSpeed);
  setSideSpeed(rightMotors, rightSpeed);
}

// Compatibility wrappers for the existing web UI (8 fixed-direction
// buttons). Each one just calls drive() with fixed throttle/steering.
// Note: steering is negated for the two backward diagonals relative to
// their forward counterparts -- this reproduces the original hand-written
// behavior, where "right" diagonals always slowed the right-side motor
// and "left" diagonals always slowed the left-side motor, regardless of
// whether the car was moving forward or backward.

void forward() {
    Serial.println("Forward");
  drive(MAX_SPEED, 0);
}

void backward() {
    Serial.println("Backward");
  drive(-MAX_SPEED, 0);
}

void right() {
    Serial.println("right");
  drive(0, MAX_SPEED);
}

void left() {
    Serial.println("left");
  drive(0, -MAX_SPEED);
}

void rightForward() {
    Serial.println("Right Forward");
  drive(MAX_SPEED, MAX_SPEED / 2);
}

void leftForward() {
    Serial.println("Left Forward");
  drive(MAX_SPEED, -MAX_SPEED / 2);
}

void rightBackward() {
    Serial.println("Right Backward");
  drive(-MAX_SPEED, -MAX_SPEED / 2);
}

void leftBackward() {
    Serial.println("Left Backward");
  drive(-MAX_SPEED, MAX_SPEED / 2);
}
