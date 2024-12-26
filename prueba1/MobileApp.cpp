// MobileApp.cpp
#include "MobileApp.h"

// Constructor initializes the app with default SSID, password, and network configurations
MobileApp::MobileApp(const char* ssid, const char* password, IPAddress ip) {
  strcpy(m_ssid, ssid);
  strcpy(m_password , password);
  m_local_IP = ip;
  m_gateway = m_local_IP;
  m_subnet = IPAddress(255,255,255,0);
  m_server(80); 
  toggle_led_cb = nullptr;
  get_config_cb = nullptr;
}

//Led
#define LED_BUILTIN 2

// Set a new SSID
void MobileApp::setSSID(const char* ssid) {
    strcpy(m_ssid, ssid);
}

// Get the current SSID
char* MobileApp::getSSID() const {
    return m_ssid;
}

// Set a new password
void MobileApp::setPassword(const char* password) {
    strcpy(m_password , password);
}

// Get the current password
const char* MobileApp::getPassword() const {
    return m_password;
}

// Set the callback for handling /rowa_identification requests
void MobileApp::set_toggle_led_cb(void (*callback)(WiFiClient&)) {
    toggle_led_cb = callback;
}

// Set the callback for handling /update_config requests
void MobileApp::set_get_config_cb(void (*callback)(WiFiClient&)) {
    get_config_cb = callback;
}

// Start the Wi-Fi access point and the server
void MobileApp::begin() {
    WiFi.softAP(m_ssid, m_password); // Start the access point
    WiFi.softAPConfig(m_local_IP, m_gateway, m_subnet); // Configure the access point with IP settings
    Serial.print("Init Wi-Fi Ok");
    m_server.begin(); // Start the HTTP server
    Serial.print("HTTP Server Ok");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP()); // Print the access point IP address
    
    Serial.print("Init App Ok");
}

// Handle incoming client requests
void MobileApp::handleClient() {
    NetworkClient client = m_server.available(); // Check if a client is connected

    if (client) {
        Serial.println("New Client.");
        String currentLine = "";

        while (client.connected()) { // Process the client while connected
            if (client.available()) { // Check if data is available to read
                char c = client.read(); // Read a character from the client
                Serial.write(c); // Echo the character to the Serial Monitor
                currentLine += c; // Append the character to the current line

                // If a new line is received
                if (c == '\n') {
                    // An empty line signals the end of the HTTP request header
                    if (currentLine.length() == 1) {
                        // Handle specific HTTP GET and POST requests
                        if (currentLine.indexOf("POST /toggle_led") >= 0 && toggle_led_cb != nullptr) {
                            toggle_led_cb(client);
                        } 
                        else if (currentLine.indexOf("GET /get_config") >= 0 && updateConfigCallback) {
                            get_config_cb(client);
                        } else {
                            // Send a 404 response for unknown endpoints
                            client.println("HTTP/1.1 404 Not Found");
                            client.println("Connection: close");
                            client.println();
                        }
                        break; // Exit the loop after processing the request
                    }
                    currentLine = ""; // Reset the current line
                }
            }
        }
        client.stop(); // Disconnect the client
        Serial.println("Client disconnected.");
    }
}*/
