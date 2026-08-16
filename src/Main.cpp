#include <WiFi.h>
#include "MotorControl.h"
#include "HtmlResponse.h"
#include "WiFiSetup.h"
#include <Arduino.h>

// WiFi credentials for the robot's access point.
// Change these before deploying — especially the password.
const char *ssid = "robot";
const char *password = "12345678";
// How long (ms) to wait for more data before dropping a client connection.
const unsigned long CLIENT_TIMEOUT_MS = 5000;

WiFiServer server(80);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);

    setupMotorPins();
    setupWiFi(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    server.begin();
    Serial.println("Server started");
}

void loop()
{
    WiFiClient client = server.available();

    if (client)
    {
        Serial.println("New Client.");
        String currentLine = "";
        unsigned long timeout = millis();

        while (client.connected() && millis() - timeout < CLIENT_TIMEOUT_MS)
        {
            if (client.available())
            {
                char c = client.read();
                timeout = millis(); // Reset timeout on each received byte
                Serial.write(c);

                if (c == '\n')
                {
                    if (currentLine.length() == 0)
                    {
                        // Blank line = end of HTTP headers; send the control page
                        sendHtmlResponse(client);
                        break;
                    }
                    else
                    {
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {
                    currentLine += c;
                }

                // Route incoming POST requests to motor commands.
                // NOTE: Diagonal/compound routes (e.g. /rightForward) must be checked
                // before their prefix matches (e.g. /right, /forward) to avoid a
                // substring false-match cutting off the more specific route.
                if (currentLine.indexOf("POST /rightForward") >= 0) {
                    rightForward();
                }
                else if (currentLine.indexOf("POST /leftForward") >= 0) {
                    leftForward();
                }
                else if (currentLine.indexOf("POST /rightBackward") >= 0) {
                    rightBackward();
                }
                else if (currentLine.indexOf("POST /leftBackward") >= 0) {
                    leftBackward();
                }
                else if (currentLine.indexOf("POST /forward") >= 0) {
                    forward();
                }
                else if (currentLine.indexOf("POST /backward") >= 0) {
                    backward();
                }
                else if (currentLine.indexOf("POST /right") >= 0) {
                    right();
                }
                else if (currentLine.indexOf("POST /left") >= 0) {
                    left();
                }
                else if (currentLine.indexOf("POST /stop") >= 0) {
                    stopCar();
                }
            }
        }

        // Done with this client — close and log
        client.stop();
        Serial.println("Client Disconnected.");
    }
}