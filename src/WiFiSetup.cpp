// WiFiSetup.cpp
// Helper for configuring the device as a Wi‑Fi access point (SoftAP).
// Exposes `setupWiFi` which starts an access point with the provided SSID and password.

#include <WiFi.h> // ESP32/ESP8266 WiFi library providing SoftAP functionality
#include "WiFiSetup.h" // Declaration of setupWiFi

/**
 * Starts the device in SoftAP (Access Point) mode.
 *
 * @param ssid     The network name (SSID) to advertise. Must be a null-terminated C string.
 * @param password The WPA2 password for the AP. Minimum length typically 8 chars; pass nullptr or empty for open AP (not recommended).
 *
 * Prints progress to the serial console. If AP creation fails, the function blocks execution
 * in an infinite loop to prevent the program from continuing in an undefined state.
 */
void setupWiFi(const char *ssid, const char *password)
{
    Serial.println("Configuring access point..."); // Inform via serial that AP setup is starting
    if (!WiFi.softAP(ssid, password)) // Attempt to start AP; returns false on failure
    {
        Serial.println("Error creating access point"); // Log failure reason to serial
        while (1)               // Halt here to avoid proceeding without a valid AP
            ; // Stop if AP creation fails
    }
}
