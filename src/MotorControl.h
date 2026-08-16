#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

// Number of motors wired per side. 1 = original 2WD build (one motor
// left, one motor right). 2 = 4WD build (front + rear motor per side,
// both always driven identically -- see MotorControl.cpp).
#define MOTORS_PER_SIDE 1

// Sets up pin modes for every configured motor. Call once from setup().
void setupMotorPins();

// Tank-style mixer used by all the movement helpers below.
//   throttle: -255 (full reverse) .. 255 (full forward)
//   steering: -255 (full left) .. 255 (full right)
// Combines throttle + steering into per-side motor speeds, so it scales
// to any MOTORS_PER_SIDE value without needing one function per wheel.
void drive(int throttle, int steering);

// Immediately stops all motors.
void stopCar();

// Convenience wrappers kept for the existing web UI (Robot-main.ino /
// HtmlResponse.cpp). Each one just calls drive() with fixed values.
void forward();
void backward();
void right();
void left();
void rightForward();
void leftForward();
void rightBackward();
void leftBackward();

#endif // MOTOR_CONTROL_H
