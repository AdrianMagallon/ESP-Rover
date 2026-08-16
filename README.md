WiFi Robot Car
A browser-controlled robot car built on an ESP32. It creates its own WiFi access point, hosts a little web interface, and lets you drive it around using an on-screen directional pad — no internet connection required.

Background
This was built as part of the Google Code Next program, which introduced me to robotics for the first time. Going in, I had basically no experience with robotics and not much with C++ either. It was a lot to take in at first — figuring out hardware, motor control, and a new language all at once was pretty overwhelming. But over the course of the program it started clicking, and by the end I was a lot more comfortable with C++ than I expected to be. This project is the result of that whole journey, and it's something I'm genuinely proud of.

How It Works
When the ESP32 boots up, it spins up a WiFi access point (default SSID: robot, password: 12345678) and starts a basic HTTP server on port 80. You connect your phone or laptop to that network, open a browser, and navigate to the ESP32's IP address (printed to Serial on startup — usually 192.168.4.1).

The web page shows a 3x3 grid of direction buttons: forward, backward, left, right, and four diagonals, plus a stop button in the middle. Pressing a button sends an HTTP POST request to a matching endpoint (like /forward or /leftBackward). The ESP32 reads that request and calls drive(), a throttle/steering mixer that computes matching left/right motor speeds — see MotorControl.cpp.

Hardware
The build uses an ESP32 development board, 2 TT gear DC motors (the yellow ones, one per driven wheel — left and right), an L298N H-bridge motor driver, an LM2596 buck converter (steps the battery voltage down to 3.3V for the ESP32), a battery pack (the original single 9V battery is fine for 2 motors), and a front-mounted ball caster in place of a third and fourth wheel — it's a passive wheel with no motor, so it just bolts on and needs no wiring. The firmware also supports an optional 4WD layout (no caster, four driven motors) if you'd rather build that instead; see MOTORS_PER_SIDE below.

Wiring
The battery splits two ways: straight into the L298N board(s) to drive the motors, and into the buck converter which steps it down to 3.3V for the ESP32.

MotorControl.h defines MOTORS_PER_SIDE, set to 1 for this two-wheel-drive-plus-caster build (one motor per side), and MotorControl.cpp lists the actual pin numbers for every motor. The single-L298N wiring is:

L298N Signal	ESP32 Pin
ENA (PWM)	12
IN1	14
IN2	27
IN3	26
IN4	25
ENB (PWM)	33
The ball caster needs no wiring at all — it's a passive wheel with no motor, so it just bolts onto the front of the chassis. Mount it roughly matched to the driven wheel height so the chassis sits level, and centered under the battery pack's weight for stability. If you'd rather build the optional 4WD layout instead, set MOTORS_PER_SIDE to 2 and wire a second L298N to the rear-motor pins in MotorControl.cpp (update the placeholder pins there to match your wiring). Avoid GPIO12 if you run into boot problems, since it's one of the ESP32's boot "strapping" pins.

Motor speed is set to 225/255 by default. Diagonal movements slow down one side to produce a turning arc; drive() handles the actual mixing so this works the same whether a side has one motor or two.

File Structure
Robot-main.ino # Main sketch — WiFi setup, HTTP server loop, request routing
MotorControl.cpp/h # Motor pin setup, the drive() throttle/steering mixer, and movement functions
WiFiSetup.cpp/h # Starts the ESP32 as a WiFi access point (SoftAP)
HtmlResponse.cpp/h # Serves the control UI as an HTTP response
Setup & Flashing
Open the project in the Arduino IDE (or PlatformIO) and make sure the ESP32 board package is installed. If you want, change the WiFi credentials at the top of Robot-main.ino:

const char *ssid = "robot";
const char *password = "12345678";
Then select your ESP32 board and the correct COM port, flash it, and open Serial Monitor at 115200 baud to see the AP IP address after boot.

Connecting & Driving
Connect your device to the robot WiFi network, then open a browser and go to http://192.168.4.1 (confirm the IP in Serial Monitor if it's different). The control pad should load immediately, and it works on mobile too since the buttons respond to touch events.

Known Issues / Things to Fix
Touch release now correctly sends /stop. An earlier bug had it calling toggleCheckbox('stop'), a function that was never defined, so lifting your finger did nothing on mobile; that's been fixed.

The WiFi credentials are hardcoded, so definitely change the password before using this around other people.

The HTTP server handles one client at a time and closes the connection after each request, so there's a small delay between commands. Holding a direction button now sends continuous input: the browser resends that command every 150ms for as long as the button stays pressed, and releasing it sends /stop — see the script in HtmlResponse.cpp.
